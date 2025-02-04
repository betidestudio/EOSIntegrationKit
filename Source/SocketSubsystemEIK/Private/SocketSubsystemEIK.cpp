// Copyright Epic Games, Inc. All Rights Reserved.

#include "SocketSubsystemEIK.h"
#include "InternetAddrEIK.h"
#include "SocketEIK.h"
#include "SocketTypes.h"
#include "Containers/Ticker.h"
#include "Misc/ConfigCacheIni.h"
#include "SocketSubsystemModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/OutputDeviceRedirector.h"
#include "OnlineSubsystemUtils.h"

#if WITH_EOS_SDK
	#include "eos_sdk.h"
#endif

TArray<FSocketSubsystemEIK*> FSocketSubsystemEIK::SocketSubsystemEOSInstances;
TMap<UWorld*, FSocketSubsystemEIK*> FSocketSubsystemEIK::SocketSubsystemEOSPerWorldMap;

FSocketSubsystemEIK::FSocketSubsystemEIK(IEIKPlatformHandlePtr InPlatformHandle, ISocketSubsystemEOSUtilsPtr InUtils)
	: P2PHandle(nullptr)
	, Utils(InUtils)
	, LastSocketError(ESocketErrors::SE_NO_ERROR)
{
#if WITH_EOS_SDK
	P2PHandle = EOS_Platform_GetP2PInterface(*InPlatformHandle);
	if (P2PHandle == nullptr)
	{
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("FOnlineSubsystemEOS: failed to init EOS platform, couldn't get p2p handle"));
	}
#endif
}

FSocketSubsystemEIK::~FSocketSubsystemEIK()
{
	Utils = nullptr;
}

FSocketSubsystemEIK* FSocketSubsystemEIK::GetSocketSubsystemForWorld(UWorld* InWorld)
{
	FSocketSubsystemEIK** Result = SocketSubsystemEOSPerWorldMap.Find(InWorld);

	if(!Result)
	{
		for (FSocketSubsystemEIK* SocketSubsystem : SocketSubsystemEOSInstances)
		{
			const UWorld* NewWorld = GetWorldForOnline(SocketSubsystem->Utils->GetSubsystemInstanceName());

			if (NewWorld == InWorld)
			{
				SocketSubsystemEOSPerWorldMap.Add(InWorld, SocketSubsystem);

				Result = &SocketSubsystem;

				break;
			}
		}
	}

	return Result ? *Result : nullptr;
}

bool FSocketSubsystemEIK::Init(FString& Error)
{
	SocketSubsystemEOSInstances.Add(this);

	FSocketSubsystemModule& SocketSubsystem = FModuleManager::LoadModuleChecked<FSocketSubsystemModule>("Sockets");
	SocketSubsystem.RegisterSocketSubsystem(EOS_SOCKETSUBSYSTEM, this, false);

	return true;
}

void FSocketSubsystemEIK::Shutdown()
{
	RemoveFromStaticContainers();

	// Destruct our sockets before we finish destructing, as they maintain a reference to us
	TrackedSockets.Reset();

	if (FSocketSubsystemModule* SocketSubsystem = FModuleManager::GetModulePtr<FSocketSubsystemModule>("Sockets"))
	{
		SocketSubsystem->UnregisterSocketSubsystem(EOS_SOCKETSUBSYSTEM);
	}
}

void FSocketSubsystemEIK::RemoveFromStaticContainers()
{
	for (TMap<UWorld*, FSocketSubsystemEIK*>::TIterator Iter(SocketSubsystemEOSPerWorldMap); Iter; ++Iter)
	{
		if (Iter.Value() == this)
		{
			Iter.RemoveCurrent();
		}
	}

	SocketSubsystemEOSInstances.Remove(this);
}

FSocket* FSocketSubsystemEIK::CreateSocket(const FName& SocketTypeName, const FString& SocketDescription, const FName& /*unused*/)
{
	return TrackedSockets.Emplace_GetRef(MakeUnique<FSocketEOS>(*this, SocketDescription)).Get();
}

FResolveInfoCached* FSocketSubsystemEIK::CreateResolveInfoCached(TSharedPtr<FInternetAddr> Addr) const
{
	return nullptr;
}

void FSocketSubsystemEIK::DestroySocket(FSocket* Socket)
{
	for (auto It = TrackedSockets.CreateIterator(); It; ++It)
	{
		if (It->IsValid() && It->Get() == Socket)
		{
			It.RemoveCurrent();
			return;
		}
	}
}

FAddressInfoResult FSocketSubsystemEIK::GetAddressInfo(const TCHAR* HostName, const TCHAR* ServiceName, EAddressInfoFlags /*unused*/, const FName /*unused*/, ESocketType /*unused*/)
{
	return FAddressInfoResult(HostName, ServiceName);
}

bool FSocketSubsystemEIK::RequiresChatDataBeSeparate()
{
	return false;
}

bool FSocketSubsystemEIK::RequiresEncryptedPackets()
{
	return false;
}

bool FSocketSubsystemEIK::GetHostName(FString& HostName)
{
	return false;
}

TSharedRef<FInternetAddr> FSocketSubsystemEIK::CreateInternetAddr()
{
	return MakeShared<FInternetAddrEOS>();
}

TSharedPtr<FInternetAddr> FSocketSubsystemEIK::GetAddressFromString(const FString& InString)
{
	bool bUnused;
	TSharedPtr<FInternetAddrEOS> NewAddress = StaticCastSharedRef<FInternetAddrEOS>(CreateInternetAddr());
	NewAddress->SetIp(*InString, bUnused);
	return NewAddress;
}

bool FSocketSubsystemEIK::HasNetworkDevice()
{
	return true;
}

const TCHAR* FSocketSubsystemEIK::GetSocketAPIName() const
{
	return TEXT("p2pSocketsEOS");
}

ESocketErrors FSocketSubsystemEIK::GetLastErrorCode()
{
	return TranslateErrorCode(LastSocketError);
}

ESocketErrors FSocketSubsystemEIK::TranslateErrorCode(int32 Code)
{
	return static_cast<ESocketErrors>(Code);
}

bool FSocketSubsystemEIK::GetLocalAdapterAddresses(TArray<TSharedPtr<FInternetAddr>>& OutAddresses)
{
	TSharedRef<FInternetAddr> AdapterAddress = GetLocalBindAddr(nullptr, *GLog);
	OutAddresses.Add(AdapterAddress);
	return true;
}

TArray<TSharedRef<FInternetAddr>> FSocketSubsystemEIK::GetLocalBindAddresses()
{
	TArray<TSharedRef<FInternetAddr>> OutAddresses;
	OutAddresses.Add(GetLocalBindAddr(nullptr, *GLog));
	return OutAddresses;
}

TSharedRef<FInternetAddr> FSocketSubsystemEIK::GetLocalBindAddr(FOutputDevice& Out)
{
	return GetLocalBindAddr(nullptr, Out);
}

#if WITH_EOS_SDK
EOS_HP2P FSocketSubsystemEIK::GetP2PHandle()
{
	check(P2PHandle != nullptr);
	return P2PHandle;
}

EOS_ProductUserId FSocketSubsystemEIK::GetLocalUserId()
{
	return Utils->GetLocalUserId();
}
#endif

TSharedRef<FInternetAddr> FSocketSubsystemEIK::GetLocalBindAddr(const UWorld* const OwningWorld, FOutputDevice& Out)
{
	TSharedRef<FInternetAddrEOS> BoundAddr = MakeShared<FInternetAddrEOS>();

#if WITH_EOS_SDK
	EOS_ProductUserId LocalUserId = GetLocalUserId();
	if (LocalUserId == nullptr)
	{
		UE_LOG(LogSocketSubsystemEOS, Error, TEXT("No local user to send p2p packets with"));
		return BoundAddr;
	}
	BoundAddr->SetLocalUserId(LocalUserId);
#else
	return BoundAddr;
#endif

	FString SessionId = Utils->GetSessionId();

	if (SessionId.IsEmpty())
	{
		SessionId = FName(NAME_GameSession).ToString();
	}

	BoundAddr->SetSocketName(SessionId);

	return BoundAddr;
}

bool FSocketSubsystemEIK::IsSocketWaitSupported() const
{
	return false;
}

void FSocketSubsystemEIK::SetLastSocketError(const ESocketErrors NewSocketError)
{
	LastSocketError = NewSocketError;
}

bool FSocketSubsystemEIK::BindChannel(const FInternetAddrEOS& Address)
{
	if (!Address.IsValid())
	{
		SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	const uint8 Channel = Address.GetChannel();

	FChannelSet& ExistingBoundPorts = BoundAddresses.FindOrAdd(Address.GetSocketName());
	if (ExistingBoundPorts.Contains(Channel))
	{
		SetLastSocketError(ESocketErrors::SE_EADDRINUSE);
		return false;
	}

	ExistingBoundPorts.Add(Channel);
	return true;
}

bool FSocketSubsystemEIK::UnbindChannel(const FInternetAddrEOS& Address)
{
	if (!Address.IsValid())
	{
		SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	const FString SocketName = Address.GetSocketName();
	const uint8 Channel = Address.GetChannel();

	// Find our sessions collection of ports
	FChannelSet* ExistingBoundPorts = BoundAddresses.Find(SocketName);
	if (!ExistingBoundPorts)
	{
		SetLastSocketError(ESocketErrors::SE_ENOTSOCK);
		return false;
	}

	// Remove our port and check if we had it bound
	if (ExistingBoundPorts->Remove(Channel) == 0)
	{
		SetLastSocketError(ESocketErrors::SE_ENOTSOCK);
		return false;
	}

	// Remove any empty sets
	if (ExistingBoundPorts->Num() == 0)
	{
		BoundAddresses.Remove(SocketName);
		ExistingBoundPorts = nullptr;
	}

	return true;
}
