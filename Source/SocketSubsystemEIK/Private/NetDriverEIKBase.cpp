// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetDriverEIKBase.h"
#include "OnlineBeaconHost.h"
#include "OnlineBeaconClient.h"
#include "EngineUtils.h"
#include "NetConnectionEIK.h"
#include "SocketEIK.h"
#include "SocketSubsystemEIK.h"
#include "Misc/EngineVersionComparison.h"
#include "EOSSharedTypes.h"
#include "Engine/Engine.h"

#if ENGINE_MAJOR_VERSION >= 5
#include UE_INLINE_GENERATED_CPP_BY_NAME(NetDriverEIKBase)
#endif

bool UNetDriverEIKBase::IsAvailable() const
{
	// Use passthrough sockets if we are a dedicated server
	if (IsRunningDedicatedServer())
	{
		return false;
	}

	if (ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(EOS_SOCKETSUBSYSTEM))
	{
		return true;
	}

	return false;
}

bool UNetDriverEIKBase::InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error)
{
	if (bIsPassthrough)
	{
		UE_LOG(LogTemp, Verbose, TEXT("Running as pass-through"));
		return Super::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error);
	}
	if (!UNetDriver::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to init driver base"));
		return false;
	}

	FSocketSubsystemEIK* const SocketSubsystem = static_cast<FSocketSubsystemEIK*>(GetSocketSubsystem());
	if (!SocketSubsystem)
	{
		if(!GetSocketSubsystem())
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get socket subsystem that is the base of EOS"));
		}
		UE_LOG(LogTemp, Warning, TEXT("Could not get socket subsystem"));
		return false;
	}

	// We don't care if our world is null, everything we uses handles it fine
	const UWorld* const MyWorld = FindWorld();

	// Get our local address (proves we're logged in)
	TSharedRef<FInternetAddr> LocalAddress = SocketSubsystem->GetLocalBindAddr(MyWorld, *GLog);
	if (!LocalAddress->IsValid())
	{
		// Not logged in?
		Error = TEXT("Could not bind local address");
		UE_LOG(LogTemp, Warning, TEXT("Could not bind local address"));
		return false;
	}


	FUniqueSocket NewSocket = SocketSubsystem->CreateUniqueSocket(NAME_DGram, TEXT("UE4"), NAME_None);
	TSharedPtr<FSocket> SharedSocket(NewSocket.Release(), FSocketDeleter(NewSocket.GetDeleter()));

	SetSocketAndLocalAddress(SharedSocket);

	if (GetSocket() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create socket"));
		return false;
	}

	// Store our local address and set our port
	TSharedRef<FInternetAddrEOS> EOSLocalAddress = StaticCastSharedRef<FInternetAddrEOS>(LocalAddress);
	if(IsBeaconDriver())
	{
		//Till we have a better solution, we will use a hardcoded port for the beacon driver
		EOSLocalAddress->SetSocketName(TEXT("BeaconSession"));
		// We will also use a hardcoded channel for the beacon driver
		EOSLocalAddress->SetChannel(71);
	}
	else
	{
		// Because some platforms remap ports, we will use the ID of the name of the net driver to be our channel
		EOSLocalAddress->SetChannel(GetTypeHash(NetDriverName.ToString()));
		// Set our net driver name so we don't accept connections across net driver types
		EOSLocalAddress->SetSocketName(NetDriverName.ToString());
	}

	static_cast<FSocketEOS*>(GetSocket())->SetLocalAddress(*EOSLocalAddress);

	LocalAddr = LocalAddress;

	return true;
}

bool UNetDriverEIKBase::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	if (!bIsUsingP2PSockets || !IsAvailable() || !ConnectURL.Host.StartsWith(EOS_CONNECTION_URL_PREFIX, ESearchCase::IgnoreCase))
	{
		UE_LOG(LogTemp, Verbose, TEXT("Connecting using IPNetDriver passthrough. ConnectUrl = (%s)"), *ConnectURL.ToString());

		bIsPassthrough = true;
		return Super::InitConnect(InNotify, ConnectURL, Error);
	}

	bool bIsValid = false;
	TSharedRef<FInternetAddrEOS> RemoteHost = MakeShared<FInternetAddrEOS>();
	RemoteHost->SetIp(*ConnectURL.Host, bIsValid);
	if (!bIsValid || ConnectURL.Port < 0)
	{
		Error = TEXT("Invalid remote address");
		UE_LOG(LogTemp, Warning, TEXT("Invalid Remote Address. ConnectUrl = (%s)"), *ConnectURL.ToString());
		return false;
	}

	UE_LOG(LogTemp, Verbose, TEXT("Connecting using EOSNetDriver. ConnectUrl = (%s)"), *ConnectURL.ToString());

	if (!InitBase(true, InNotify, ConnectURL, false, Error))
	{
		return false;
	}

	// Set the address to what was parsed (us + remote)
	LocalAddr = RemoteHost;

	// Reference to our newly created socket
	FSocket* CurSocket = GetSocket();

	// Bind our local port
	FSocketSubsystemEIK* const SocketSubsystem = static_cast<FSocketSubsystemEIK*>(GetSocketSubsystem());
	check(SocketSubsystem);
	if (!SocketSubsystem->BindNextPort(CurSocket, *LocalAddr, MaxPortCountToTry + 1, 1))
	{
		// Failure
		Error = TEXT("Could not bind local port");
		UE_LOG(LogTemp, Warning, TEXT("Could not bind local port in %d attempts"), MaxPortCountToTry);
		return false;
	}

	// Create an unreal connection to the server
	UNetConnectionEIK* Connection = NewObject<UNetConnectionEIK>(NetConnectionClass);
	check(Connection);

	// Set it as the server connection before anything else so everything knows this is a client
	ServerConnection = Connection;
	Connection->InitLocalConnection(this, CurSocket, ConnectURL, USOCK_Pending);

	CreateInitialClientChannels();

	return true;
}

bool UNetDriverEIKBase::InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error)
{
	UE_LOG(LogTemp, Verbose, TEXT("InitListen with LocalURL = (%s)"), *LocalURL.ToString());
	if (!bIsUsingP2PSockets || !IsAvailable() || LocalURL.HasOption(TEXT("bIsLanMatch")) || LocalURL.HasOption(TEXT("bUseIPSockets")))
	{
		UE_LOG(LogTemp, Verbose, TEXT("Init as IPNetDriver listen server. LocalURL = (%s)"), *LocalURL.ToString());

		bIsPassthrough = true;
		return Super::InitListen(InNotify, LocalURL, bReuseAddressAndPort, Error);
	}

	UE_LOG(LogTemp, Verbose, TEXT("Init as EOSNetDriver listen server. LocalURL = (%s)"), *LocalURL.ToString());

	if (!InitBase(false, InNotify, LocalURL, bReuseAddressAndPort, Error))
	{
		return false;
	}

	// Bind our specified port if provided
	FSocket* CurSocket = GetSocket();
	if (!CurSocket->Listen(0))
	{
		Error = TEXT("Could not listen");
		UE_LOG(LogTemp, Warning, TEXT("Could not listen on socket"));
		return false;
	}

	InitConnectionlessHandler();

	UE_LOG(LogTemp, Verbose, TEXT("Initialized as an EOSP2P listen server"));

	// Ensure we have a valid world context
	UWorld* TWorld = FindWorld();
	if (!TWorld)
	{
		Error = TEXT("Invalid world context");
		return false;
	}
	
	// Check if a Beacon Host already exists
	bool bBeaconHostExists = false;
    for (TActorIterator<AOnlineBeaconHost> It(TWorld); It; ++It)
	{
		if (*It)
		{
			bBeaconHostExists = true;
			break;
		}
	}

	/* Initialize Beacon Host if it doesn't already exist
	if (!bBeaconHostExists)
	{
        AOnlineBeaconHost* BeaconHost = TWorld->SpawnActor<AOnlineBeaconHost>();
		if (BeaconHost)
		{
			UE_LOG(LogTemp, Error, TEXT("Spawned Beacon Host"));
			BeaconHost->SetNetDriverName(NAME_BeaconNetDriver);
			BeaconHost->InitHost();
			BeaconHost->PauseBeaconRequests(false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Beacon Host"));
		}
	}
	*/

	return true;
}

ISocketSubsystem* UNetDriverEIKBase::GetSocketSubsystem()
{
	if (bIsPassthrough)
	{
		return ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	}
	UWorld* CurrentWorld = FindWorld();
	FSocketSubsystemEIK* DefaultSocketSubsystem = static_cast<FSocketSubsystemEIK*>(ISocketSubsystem::Get(EOS_SOCKETSUBSYSTEM));
	return DefaultSocketSubsystem->GetSocketSubsystemForWorld(CurrentWorld);
}

void UNetDriverEIKBase::Shutdown()
{
	Super::Shutdown();

	// Kill our P2P sessions now, instead of when garbage collection kicks in later
	if (!bIsPassthrough)
	{
		if(ServerConnection)
		{
			if (UNetConnectionEIK* const EOSServerConnection = Cast<UNetConnectionEIK>(ServerConnection))
			{
				EOSServerConnection->DestroyEOSConnection();
			}
		}
		for (UNetConnection* Client : ClientConnections)
		{
			if(Client)
			{
				if (UNetConnectionEIK* const EOSClient = Cast<UNetConnectionEIK>(Client))
				{
					EOSClient->DestroyEOSConnection();
				}
			}
		}
	}
}

int UNetDriverEIKBase::GetClientPort()
{
	if (bIsPassthrough)
	{
		return Super::GetClientPort();
	}

	// Starting range of dynamic/private/ephemeral ports
	return 49152;
}

bool UNetDriverEIKBase::IsBeaconDriver() const
{
	if (!GEngine) return false;

	for (const auto &WorldContext : GEngine->GetWorldContexts())
	{
		if (UWorld *ItWorld = WorldContext.World())
		{
			for (AOnlineBeacon* Beacon : TActorRange<AOnlineBeacon>(ItWorld))
			{
				if (Beacon->GetNetDriver() == this)
				{
					return true;
				}
			}
		}
	}
	return false;
}

	

UWorld* UNetDriverEIKBase::FindWorld() const
{
	UWorld* MyWorld = GetWorld();
	
	// If we don't have a world, we may be a pending net driver
	if (!MyWorld && GEngine)
	{
		if (FWorldContext* WorldContext = GEngine->GetWorldContextFromPendingNetGameNetDriver(this))
		{
			MyWorld = WorldContext->World();
		}
	}

	if(!MyWorld)
	{
		if (GEngine != nullptr)
		{
			for (const auto &WorldContext : GEngine->GetWorldContexts())
			{
				UWorld *ItWorld = WorldContext.World();
				if (ItWorld != nullptr)
				{
					for (TActorIterator<AOnlineBeacon> It(ItWorld); It; ++It)
					{
						if (It->GetNetDriver() == this)
						{
							return ItWorld;
						}
					}
				}
			}
		}
	}

	return MyWorld;
}


