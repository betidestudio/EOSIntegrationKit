// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEOSTypes.h"
#include "NboSerializer.h"

#if WITH_EOS_SDK

/**
 * Serializes data in network byte order form into a buffer
 */
class FNboSerializeToBufferEOS : public FNboSerializeToBuffer
{
public:
	/** Default constructor zeros num bytes*/
	FNboSerializeToBufferEOS() :
		FNboSerializeToBuffer(512)
	{
	}

	/** Constructor specifying the size to use */
	FNboSerializeToBufferEOS(uint32 Size) :
		FNboSerializeToBuffer(Size)
	{
	}

	/**
	 * Adds EOS session info to the buffer
	 */
 	friend inline FNboSerializeToBufferEOS& operator<<(FNboSerializeToBufferEOS& Ar, const FOnlineSessionInfoEOS& SessionInfo)
 	{
		check(SessionInfo.HostAddr.IsValid());
		// Skip SessionType (assigned at creation)
		Ar << *SessionInfo.SessionId;
		Ar << *SessionInfo.HostAddr;
		return Ar;
 	}

	/**
	 * Adds EOS Unique Id to the buffer
	 */
	friend inline FNboSerializeToBufferEOS& operator<<(FNboSerializeToBufferEOS& Ar, const FUniqueNetIdEOS& UniqueId)
	{
		Ar << UniqueId.UniqueNetIdStr;
		Ar << UniqueId.EpicAccountIdStr;
		Ar << UniqueId.ProductUserIdStr;
		return Ar;
	}

	/**
	 * Adds string Unique Id to the buffer
	 */
	friend inline FNboSerializeToBufferEOS& operator<<(FNboSerializeToBufferEOS& Ar, const FUniqueNetIdString& UniqueId)
	{
		Ar << UniqueId.UniqueNetIdStr;
		return Ar;
	}
};

/**
 * Class used to write data into packets for sending via system link
 */
class FNboSerializeFromBufferEOS : public FNboSerializeFromBuffer
{
public:
	/**
	 * Initializes the buffer, size, and zeros the read offset
	 */
	FNboSerializeFromBufferEOS(uint8* Packet,int32 Length) :
		FNboSerializeFromBuffer(Packet,Length)
	{
	}

	/**
	 * Reads EOS session info from the buffer
	 */
 	friend inline FNboSerializeFromBufferEOS& operator>>(FNboSerializeFromBufferEOS& Ar, FOnlineSessionInfoEOS& SessionInfo)
 	{
		check(SessionInfo.HostAddr.IsValid());
		// Skip SessionType (assigned at creation)
		SessionInfo.SessionId = FUniqueNetIdEOS::Create();
		Ar >> *ConstCastSharedRef<FUniqueNetIdString>(SessionInfo.SessionId);
		Ar >> *SessionInfo.HostAddr;
		return Ar;
 	}

	/**
	 * Reads EOS Unique Id from the buffer
	 */
	friend inline FNboSerializeFromBufferEOS& operator>>(FNboSerializeFromBufferEOS& Ar, FUniqueNetIdEOS& UniqueId)
	{
		Ar >> UniqueId.UniqueNetIdStr;
		Ar >> UniqueId.EpicAccountIdStr;
		Ar >> UniqueId.ProductUserIdStr;
		return Ar;
	}

	/**
	 * Reads string Unique Id from the buffer
	 */
	friend inline FNboSerializeFromBufferEOS& operator>>(FNboSerializeFromBufferEOS& Ar, FUniqueNetIdString& UniqueId)
	{
		Ar >> UniqueId.UniqueNetIdStr;
		return Ar;
	}
};

#endif
