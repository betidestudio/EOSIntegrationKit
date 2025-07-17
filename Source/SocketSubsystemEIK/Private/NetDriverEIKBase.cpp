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
#include "Misc/ConfigCacheIni.h"

#if ENGINE_MAJOR_VERSION >= 5
#include UE_INLINE_GENERATED_CPP_BY_NAME(NetDriverEIKBase)
#endif

UNetDriverEIKBase::UNetDriverEIKBase(const FObjectInitializer& ObjectInitializer)
	: UIpNetDriver(ObjectInitializer)
{
	bIsPassthrough = false;
	
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
	// Check for deprecated config in UE 5.6+
	bool bUnused;
	if (GConfig->GetBool(TEXT("/Script/SocketSubsystemEIK.NetDriverEIKBase"), TEXT("bIsUsingP2PSockets"), bUnused, GEngineIni))
	{
		UE_LOG(LogTemp, Warning, TEXT("EIK NetDriver: bIsUsingP2PSockets is deprecated, please remove any related config values"));
	}
#else
	// For UE 5.5 and below, initialize bIsUsingP2PSockets from config
	if (!GConfig->GetBool(TEXT("/Script/SocketSubsystemEIK.NetDriverEIKBase"), TEXT("bIsUsingP2PSockets"), bIsUsingP2PSockets, GEngineIni))
	{
		bIsUsingP2PSockets = true; // Default to true
	}
#endif
}

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
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
		// UE 5.6+ uses NetDriverDefinition instead of NetDriverName
		FString NetDriverDefinitionStr = GetNetDriverDefinition().ToString();
		EOSLocalAddress->SetChannel(GetTypeHash(NetDriverDefinitionStr));
		EOSLocalAddress->SetSocketName(NetDriverDefinitionStr);
#else
		// UE 5.5 and below use NetDriverName
		EOSLocalAddress->SetChannel(GetTypeHash(NetDriverName.ToString()));
		EOSLocalAddress->SetSocketName(NetDriverName.ToString());
#endif
	}

	static_cast<FSocketEOS*>(GetSocket())->SetLocalAddress(*EOSLocalAddress);

	LocalAddr = LocalAddress;

	return true;
}

bool UNetDriverEIKBase::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	
	bool bIsEOSURL = ConnectURL.Host.StartsWith(EOS_CONNECTION_URL_PREFIX, ESearchCase::IgnoreCase);
	bool bIsAvailableResult = IsAvailable();
	
	
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
	// UE 5.6+ removed bIsUsingP2PSockets checks - engine now always uses EOS sockets when available
	if (!bIsAvailableResult || !bIsEOSURL)
#else
	// UE 5.5 and below still check bIsUsingP2PSockets
	if (!bIsUsingP2PSockets || !bIsAvailableResult || !bIsEOSURL)
#endif
	{

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
	
	bool bIsAvailableResult = IsAvailable();
	bool bHasLanMatch = LocalURL.HasOption(TEXT("bIsLanMatch"));
	bool bUseIPSockets = LocalURL.HasOption(TEXT("bUseIPSockets"));
	
	
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
	// UE 5.6+ removed bIsUsingP2PSockets checks - engine now always uses EOS sockets when available
	if (!bIsAvailableResult || bHasLanMatch || bUseIPSockets)
#else
	// UE 5.5 and below still check bIsUsingP2PSockets
	if (!bIsUsingP2PSockets || !bIsAvailableResult || bHasLanMatch || bUseIPSockets)
#endif
	{

		bIsPassthrough = true;
		return Super::InitListen(InNotify, LocalURL, bReuseAddressAndPort, Error);
	}


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


