// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SocketSubsystem.h"
#include "IEOSSDKManager.h"
#include "SocketSubsystemEIKUtils.h"

#if WITH_EOS_SDK
	#if defined(EOS_PLATFORM_BASE_FILE_NAME)
	#include EOS_PLATFORM_BASE_FILE_NAME
	#endif

	#include "eos_p2p_types.h"
#endif

#ifndef EOS_SOCKETSUBSYSTEM
#define EOS_SOCKETSUBSYSTEM FName(TEXT("EOS"))
#endif

class UNetConnectionEIK;
class FSocket;
class FResolveInfoCached;
class FResolveInfo;
class FInternetAddr;
class FInternetAddrEOS;
class FSocketEOS;

typedef TSet<uint8> FChannelSet;

class SOCKETSUBSYSTEMEIK_API FSocketSubsystemEIK
	: public ISocketSubsystem
{
public:
	FSocketSubsystemEIK(IEIKPlatformHandlePtr InPlatformHandle, ISocketSubsystemEOSUtilsPtr InUtils);
	virtual ~FSocketSubsystemEIK();

//~ Begin ISocketSubsystem Interface
	virtual bool Init(FString& Error) override;
	virtual void Shutdown() override;
	virtual FSocket* CreateSocket(const FName& SocketType, const FString& SocketDescription, const FName& ProtocolType) override;
	virtual FResolveInfoCached* CreateResolveInfoCached(TSharedPtr<FInternetAddr> Addr) const override;
	virtual void DestroySocket(FSocket* Socket) override;
	virtual FAddressInfoResult GetAddressInfo(const TCHAR* HostName, const TCHAR* ServiceName = nullptr, EAddressInfoFlags QueryFlags = EAddressInfoFlags::Default, const FName ProtocolTypeName = NAME_None, ESocketType SocketType = ESocketType::SOCKTYPE_Unknown) override;
	virtual bool RequiresChatDataBeSeparate() override;
	virtual bool RequiresEncryptedPackets() override;
	virtual bool GetHostName(FString& HostName) override;
	virtual TSharedRef<FInternetAddr> CreateInternetAddr() override;
	virtual TSharedPtr<FInternetAddr> GetAddressFromString(const FString& InString) override;
	virtual bool HasNetworkDevice() override;
	virtual const TCHAR* GetSocketAPIName() const override;
	virtual ESocketErrors GetLastErrorCode() override;
	virtual ESocketErrors TranslateErrorCode(int32 Code) override;
	virtual bool GetLocalAdapterAddresses(TArray<TSharedPtr<FInternetAddr>>& OutAdresses) override;
	virtual TSharedRef<FInternetAddr> GetLocalBindAddr(FOutputDevice& Out) override;
	virtual TArray<TSharedRef<FInternetAddr>> GetLocalBindAddresses() override;
	TSharedRef<FInternetAddr> GetLocalBindAddr(const UWorld* const OwningWorld, FOutputDevice& Out);
	virtual bool IsSocketWaitSupported() const override;
//~ End ISocketSubsystem Interface

	void SetLastSocketError(const ESocketErrors NewSocketError);

#if WITH_EOS_SDK
	EOS_HP2P GetP2PHandle();
	EOS_ProductUserId GetLocalUserId();
#endif

	/**
	 * Bind our socket name & channel and ensure no other connections are using this combination
	 *
	 * @param Address The address to bind to
	 * @return True if we bound successfully, false if not and LastSocketError will be set with a reason
	 */
	bool BindChannel(const FInternetAddrEOS& Address);

	/**
	 * Unbind a previously bound socket name and channel.
	 *
	 * @param Address The Address to unbind
	 * @return True if we released the port successfully, false if not and LastSocketError will be set with a reason
	 */
	bool UnbindChannel(const FInternetAddrEOS& Address);

	/**
	 * Returns the FSocketSubsytemEOS instance corresponding with the UWorld* parameter.
	 * If no entry exists for the given UWorld*, it will add it if a valid instance is found.
	 * 
	 * @param InWorld UWorld* to be used in the search
	 * @return The FSocketSubsystemEIK instance associated with InWorld
	 */
	FSocketSubsystemEIK* GetSocketSubsystemForWorld(UWorld* InWorld);

private:
	/** Removes the FSocketSubsystemEIK instance from SocketSubsystemEOSInstances and all related entries from SocketSubsystemEOSPerWorldMap */
	void RemoveFromStaticContainers();

private:
#if WITH_EOS_SDK
	EOS_HP2P P2PHandle;
#endif
	ISocketSubsystemEOSUtilsPtr Utils;

	/** All sockets allocated by this subsystem */
	TArray<TUniquePtr<FSocketEOS>> TrackedSockets;

	/** All bound socket addresses */
	TMap<FString, FChannelSet> BoundAddresses;

	/** The last error we received */
	ESocketErrors LastSocketError;

	/** Static array of all instances of FSocketSubsystemEIK running concurrently (can be more than one in PIE) */
	static TArray<FSocketSubsystemEIK*> SocketSubsystemEOSInstances;

	/** Static map of world to corresponding FSocketSubsystemEIK instance */
	static TMap<UWorld*, FSocketSubsystemEIK*> SocketSubsystemEOSPerWorldMap;
};
