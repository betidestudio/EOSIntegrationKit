// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEOSTypes.h"
#if ENGINE_MAJOR_VERSION == 5
#include "NboSerializerOSS.h"
#else
#include "NboSerializer.h"
#endif

#if WITH_EOS_SDK

/**
 * Serializes data in network byte order form into a buffer
 */
#if ENGINE_MAJOR_VERSION == 5
class FNboSerializeToBufferEOS : public FNboSerializeToBufferOSS
#else
class FNboSerializeToBufferEOS : public FNboSerializeToBuffer
#endif
{
public:
	/** Default constructor zeros num bytes*/
	FNboSerializeToBufferEOS() :
#if ENGINE_MAJOR_VERSION == 5
	FNboSerializeToBufferOSS(512)
	{
	}

	/** Constructor specifying the size to use */
	FNboSerializeToBufferEOS(uint32 Size) :
		FNboSerializeToBufferOSS(Size)
	{
	}
#else
		FNboSerializeToBuffer(512)
	{
	}

	/** Constructor specifying the size to use */
	FNboSerializeToBufferEOS(uint32 Size) :
		FNboSerializeToBuffer(Size)
	{
	}
#endif

	/**
	 * Adds EOS session info to the buffer
	 */
 	friend inline FNboSerializeToBufferEOS& operator<<(FNboSerializeToBufferEOS& Ar, const FOnlineSessionInfoEOS& SessionInfo)
 	{
		check(SessionInfo.HostAddr.IsValid());
		// Skip SessionType (assigned at creation)
		Ar << *SessionInfo.SessionId;
		((FNboSerializeToBuffer&)Ar) << *SessionInfo.HostAddr;
		return Ar;
 	}

	/**
	 * Adds string Unique Id to the buffer
	 */
	friend inline FNboSerializeToBufferEOS& operator<<(FNboSerializeToBufferEOS& Ar, const FUniqueNetIdString& UniqueId)
	{
		((FNboSerializeToBuffer&)Ar) << UniqueId.UniqueNetIdStr;
		return Ar;
	}
};

/**
 * Class used to write data into packets for sending via system link
 */
#if ENGINE_MAJOR_VERSION == 5
class FNboSerializeFromBufferEOS : public FNboSerializeFromBufferOSS
#else
class FNboSerializeFromBufferEOS : public FNboSerializeFromBuffer
#endif
{
public:
	/**
	 * Initializes the buffer, size, and zeros the read offset
	 */
	FNboSerializeFromBufferEOS(uint8* Packet,int32 Length) :
#if ENGINE_MAJOR_VERSION == 5
	FNboSerializeFromBufferOSS(Packet,Length)
#else
		FNboSerializeFromBuffer(Packet,Length)
#endif
	{
	}

	/**
	 * Reads EOS session info from the buffer
	 */
 	friend inline FNboSerializeFromBufferEOS& operator>>(FNboSerializeFromBufferEOS& Ar, FOnlineSessionInfoEOS& SessionInfo)
 	{
		check(SessionInfo.HostAddr.IsValid());

		FString SessionId;
		Ar >> SessionId;
		// Skip SessionType (assigned at creation)
		Ar >> *SessionInfo.HostAddr;

		SessionInfo.SessionId = FUniqueNetIdString::Create(MoveTemp(SessionId), FName("EOS"));

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
