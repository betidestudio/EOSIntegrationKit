// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OnlineSubsystemTypes.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "EOSSharedTypes.h"
#include "EOSShared.h"
#include "IPAddress.h"

#include "OnlineSubsystemEOSPackage.h"

#define EOS_OSS_STRING_BUFFER_LENGTH 256 + 1 // 256 plus null terminator

class FOnlineSubsystemEOS;

#define EOS_ID_SEPARATOR TEXT("|")
#define EMPTY_EASID TEXT("00000000000000000000000000000000")
#define EMPTY_PUID TEXT("00000000000000000000000000000000")
#define ID_HALF_BYTE_SIZE 16
#define EOS_ID_BYTE_SIZE (ID_HALF_BYTE_SIZE * 2)

typedef TSharedPtr<const class FUniqueNetIdEOS> FUniqueNetIdEOSPtr;
typedef TSharedRef<const class FUniqueNetIdEOS> FUniqueNetIdEOSRef;

/**
 * Unique net id wrapper for a EOS account ids. The underlying string is a combination
 * of both account ids concatenated. "<EOS_EpicAccountId>|<EOS_ProductAccountId>"
 */
class FUniqueNetIdEOS :
	public FUniqueNetIdString
{
public:
	template<typename... TArgs>
	static FUniqueNetIdEOSRef Create(TArgs&&... Args)
	{
		return MakeShareable(new FUniqueNetIdEOS(Forward<TArgs>(Args)...));
	}

	static const FUniqueNetIdEOS& Cast(const FUniqueNetId& NetId)
	{
		check(GetTypeStatic() == NetId.GetType());
		return *static_cast<const FUniqueNetIdEOS*>(&NetId);
	}

	friend uint32 GetTypeHash(const FUniqueNetIdEOS& A)
	{
		return ::GetTypeHash(A.UniqueNetIdStr);
	}

	/** global static instance of invalid (zero) id */
	static const FUniqueNetIdEOSRef& EmptyId()
	{
		static const FUniqueNetIdEOSRef EmptyId(Create());
		return EmptyId;
	}

	static FName GetTypeStatic()
	{
		static FName NAME_Eos(TEXT("EOS"));
		return NAME_Eos;
	}

	virtual FName GetType() const override
	{
		return GetTypeStatic();
	}

	virtual const uint8* GetBytes() const override
	{
		return RawBytes;
	}

	virtual int32 GetSize() const override
	{
		return EOS_ID_BYTE_SIZE;
	}

PACKAGE_SCOPE:
	void UpdateNetIdStr(const FString& InNetIdStr)
	{
		UniqueNetIdStr = InNetIdStr;
		ParseAccountIds();
	}

	void ParseAccountIds()
	{
		TArray<FString> AccountIds;
		UniqueNetIdStr.ParseIntoArray(AccountIds, EOS_ID_SEPARATOR, false);
		if (AccountIds.Num() > 0 && AccountIds[0].Len() > 0)
		{
			EpicAccountIdStr = AccountIds[0];
		}
		else
		{
			EpicAccountIdStr = EMPTY_EASID;
		}
		AddToBuffer(RawBytes, EpicAccountIdStr);
		if (AccountIds.Num() > 1 && AccountIds[1].Len() > 0)
		{
			ProductUserIdStr = AccountIds[1];
		}
		else
		{
			ProductUserIdStr = EMPTY_PUID;
		}
		AddToBuffer(RawBytes + ID_HALF_BYTE_SIZE, ProductUserIdStr);
	}

	void AddToBuffer(uint8* Buffer, const FString& Source)
	{
		check(Source.Len() == 32);
		for (int32 ReadOffset = 0, WriteOffset = 0; ReadOffset < 32; ReadOffset += 2, WriteOffset++)
		{
			FString HexStr = Source.Mid(ReadOffset, 2);
			// String is in HEX so use the version that takes a base
			uint8 ToByte = (uint8)FCString::Strtoi(*HexStr, nullptr, 16);
			Buffer[WriteOffset] = ToByte;
		}
	}

	FString EpicAccountIdStr;
	FString ProductUserIdStr;
	uint8 RawBytes[EOS_ID_BYTE_SIZE] = { 0 };
private:
	FUniqueNetIdEOS()
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		: FUniqueNetIdString(EMPTY_EASID EOS_ID_SEPARATOR EMPTY_PUID)
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		Type = FName("EOS");
	}
	
	explicit FUniqueNetIdEOS(uint8* Bytes, int32 Size)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		: FUniqueNetIdString()
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		check(Size == EOS_ID_BYTE_SIZE);
		EpicAccountIdStr = BytesToHex(Bytes, ID_HALF_BYTE_SIZE);
		ProductUserIdStr = BytesToHex(Bytes + ID_HALF_BYTE_SIZE, ID_HALF_BYTE_SIZE);
		UniqueNetIdStr = EpicAccountIdStr + EOS_ID_SEPARATOR + ProductUserIdStr;
		Type = FName("EOS");
	}

	explicit FUniqueNetIdEOS(const FString& InUniqueNetId)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		: FUniqueNetIdString(InUniqueNetId, FName("EOS"))
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		ParseAccountIds();
	}

	explicit FUniqueNetIdEOS(FString&& InUniqueNetId)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		: FUniqueNetIdString(MoveTemp(InUniqueNetId))
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		ParseAccountIds();
		Type = FName("EOS");
	}

	explicit FUniqueNetIdEOS(const FUniqueNetId& Src)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		: FUniqueNetIdString(Src)
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		ParseAccountIds();
		Type = FName("EOS");
	}
};

#ifndef AUTH_ATTR_REFRESH_TOKEN
	#define AUTH_ATTR_REFRESH_TOKEN TEXT("refresh_token")
#endif
#ifndef AUTH_ATTR_ID_TOKEN
	#define AUTH_ATTR_ID_TOKEN TEXT("id_token")
#endif
#ifndef AUTH_ATTR_AUTHORIZATION_CODE
	#define AUTH_ATTR_AUTHORIZATION_CODE TEXT("authorization_code")
#endif

#define USER_ATTR_DISPLAY_NAME TEXT("display_name")
#define USER_ATTR_COUNTRY TEXT("country")
#define USER_ATTR_LANG TEXT("language")

#if WITH_EOS_SDK

/** Used to update all types of FOnlineUser classes, irrespective of leaf most class type */
class IAttributeAccessInterface
{
public:
	virtual void SetInternalAttribute(const FString& AttrName, const FString& AttrValue)
	{
	}

	virtual FUniqueNetIdEOSPtr GetUniqueNetIdEOS() const
	{
		return FUniqueNetIdEOSPtr();
	}
};

typedef TSharedPtr<IAttributeAccessInterface> IAttributeAccessInterfacePtr;
typedef TSharedRef<IAttributeAccessInterface> IAttributeAccessInterfaceRef;

/**
 * Implementation of FOnlineUser that can be shared across multiple class hiearchies
 */
template<class BaseClass, class AttributeAccessClass>
class TOnlineUserEOS
	: public BaseClass
	, public AttributeAccessClass
{
public:
	TOnlineUserEOS(const FUniqueNetIdEOSRef& InNetIdRef)
		: UserIdRef(InNetIdRef)
	{
	}

	virtual ~TOnlineUserEOS()
	{
	}

// FOnlineUser
	virtual FUniqueNetIdRef GetUserId() const override
	{
		return UserIdRef;
	}

	virtual FString GetRealName() const override
	{
		return FString();
	}

	virtual FString GetDisplayName(const FString& Platform = FString()) const override
	{
		FString ReturnValue;
		GetUserAttribute(USER_ATTR_DISPLAY_NAME, ReturnValue);
		return ReturnValue;
	}

	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override
	{
		const FString* FoundAttr = UserAttributes.Find(AttrName);
		if (FoundAttr != nullptr)
		{
			OutAttrValue = *FoundAttr;
			return true;
		}
		return false;
	}
//~FOnlineUser

	virtual void SetInternalAttribute(const FString& AttrName, const FString& AttrValue)
	{
		UserAttributes.Add(AttrName, AttrValue);
	}

	virtual FUniqueNetIdEOSPtr GetUniqueNetIdEOS() const
	{
		return UserIdRef;
	}

protected:
	/** User Id represented as a FUniqueNetId */
	FUniqueNetIdEOSRef UserIdRef;
	/** Additional key/value pair data related to user attribution */
	TMap<FString, FString> UserAttributes;
};

/**
 * Implementation of FUserOnlineAccount methods that adds in the online user template to complete the interface
 */
template<class BaseClass>
class TUserOnlineAccountEOS :
	public TOnlineUserEOS<BaseClass, IAttributeAccessInterface>
{
public:
	TUserOnlineAccountEOS(const FUniqueNetIdEOSRef& InNetIdRef)
		: TOnlineUserEOS<BaseClass, IAttributeAccessInterface>(InNetIdRef)
	{
	}

// FUserOnlineAccount
	virtual FString GetAccessToken() const override
	{
		FString Token;
		GetAuthAttribute(AUTH_ATTR_ID_TOKEN, Token);
		return Token;
	}

	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override
	{
		const FString* FoundAttr = AdditionalAuthData.Find(AttrName);
		if (FoundAttr != nullptr)
		{
			OutAttrValue = *FoundAttr;
			return true;
		}
		return false;
	}

	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override
	{
		const FString* FoundAttr = this->UserAttributes.Find(AttrName);
		if (FoundAttr == nullptr || *FoundAttr != AttrValue)
		{
			this->UserAttributes.Add(AttrName, AttrValue);
			return true;
		}
		return false;
	}
//~FUserOnlineAccount

	void SetAuthAttribute(const FString& AttrName, const FString& AttrValue)
	{
		AdditionalAuthData.Add(AttrName, AttrValue);
	}

protected:
	/** Additional key/value pair data related to auth */
	TMap<FString, FString> AdditionalAuthData;
};

typedef TSharedRef<FOnlineUserPresence> FOnlineUserPresenceRef;

/**
 * Implementation of FOnlineFriend methods that adds in the online user template to complete the interface
 */
template<class BaseClass>
class TOnlineFriendEOS :
	public TOnlineUserEOS<BaseClass, IAttributeAccessInterface>
{
public:
	TOnlineFriendEOS(const FUniqueNetIdEOSRef& InNetIdRef)
		: TOnlineUserEOS<BaseClass, IAttributeAccessInterface>(InNetIdRef)
	{
	}

// FOnlineFriend
	/**
	 * @return the current invite status of a friend wrt to user that queried
	 */
	virtual EInviteStatus::Type GetInviteStatus() const override
	{
		return InviteStatus;
	}

	/**
	 * @return presence info for an online friend
	 */
	virtual const FOnlineUserPresence& GetPresence() const override
	{
		return Presence;
	}
//~FOnlineFriend

	void SetInviteStatus(EInviteStatus::Type InStatus)
	{
		InviteStatus = InStatus;
	}

	void SetPresence(FOnlineUserPresenceRef InPresence)
	{
		// Copy the data over since the friend shares it as a const&
		Presence = *InPresence;
	}

protected:
	FOnlineUserPresence Presence;
	EInviteStatus::Type InviteStatus;
};

/**
 * Implementation of FOnlineBlockedPlayer methods that adds in the online user template to complete the interface
 */
template<class BaseClass>
class TOnlineBlockedPlayerEOS :
	public TOnlineUserEOS<BaseClass, IAttributeAccessInterface>
{
public:
	TOnlineBlockedPlayerEOS(const FUniqueNetIdEOSRef& InNetIdRef)
		: TOnlineUserEOS<BaseClass, IAttributeAccessInterface>(InNetIdRef)
	{
	}
};

/**
 * Implementation of FOnlineRecentPlayer methods that adds in the online user template to complete the interface
 */
template<class BaseClass>
class TOnlineRecentPlayerEOS :
	public TOnlineUserEOS<BaseClass, IAttributeAccessInterface>
{
public:
	TOnlineRecentPlayerEOS(const FUniqueNetIdEOSRef& InNetIdRef)
		: TOnlineUserEOS<BaseClass, IAttributeAccessInterface>(InNetIdRef)
	{
	}

// FOnlineRecentPlayer
	/**
	 * @return last time the player was seen by the current user
	 */
	virtual FDateTime GetLastSeen() const override
	{
		return LastSeenTime;
	}
//~FOnlineRecentPlayer

	void SetLastSeen(const FDateTime& InLastSeenTime)
	{
		LastSeenTime = InLastSeenTime;
	}

protected:
	FDateTime LastSeenTime;
};

static inline FString MakeNetIdStringFromIds(EOS_EpicAccountId AccountId, EOS_ProductUserId UserId)
{
	FString NetId = LexToString(AccountId);

	// Only add this when the product user id is valid for more consistent net id string generation
	// across different code paths
	const FString ProductIdStr = LexToString(UserId);
	if (!ProductIdStr.IsEmpty())
	{
		NetId += EOS_ID_SEPARATOR;
		NetId += ProductIdStr;
	}

	return NetId;
}

/** Class to handle all callbacks generically using a lambda to process callback results */
template<typename CallbackFuncType, typename CallbackType>
class TEOSCallback :
	public FCallbackBase
{
public:
	TFunction<void(const CallbackType*)> CallbackLambda;

	TEOSCallback()
	{

	}
	virtual ~TEOSCallback() = default;


	CallbackFuncType GetCallbackPtr()
	{
		return &CallbackImpl;
	}

private:
	static void EOS_CALL CallbackImpl(const CallbackType* Data)
	{
		if (EOS_EResult_IsOperationComplete(Data->ResultCode) == EOS_FALSE)
		{
			// Ignore
			return;
		}
		check(IsInGameThread());

		TEOSCallback* CallbackThis = (TEOSCallback*)Data->ClientData;
		check(CallbackThis);

		if (FCallbackBase::ShouldCancelAllCallbacks())
		{
			delete CallbackThis;
			return;
		}

		check(CallbackThis->CallbackLambda);
		CallbackThis->CallbackLambda(Data);

		delete CallbackThis;
	}
};

namespace OSSInternalCallback
{
	/** Create a callback for a non-SDK function that is tied to the lifetime of an arbitrary shared pointer. */
	template <typename DelegateType, typename OwnerType, typename... CallbackArgs>
	UE_NODISCARD DelegateType Create(const TSharedPtr<OwnerType, ESPMode::ThreadSafe>& InOwner,
		const TFunction<void(CallbackArgs...)>& InUserCallback)
	{
		const DelegateType& CheckOwnerThenExecute = DelegateType::CreateLambda(
			[WeakOwner = TWeakPtr<OwnerType, ESPMode::ThreadSafe>(InOwner), InUserCallback](CallbackArgs... Payload) {
				check(IsInGameThread());
				TSharedPtr<OwnerType, ESPMode::ThreadSafe> Owner = WeakOwner.Pin();
				if (Owner.IsValid())
				{
					InUserCallback(Payload...);
				}
		});

		return CheckOwnerThenExecute;
	}
}

/**
 * Class to handle nested callbacks (callbacks that are tied to an external callback's lifetime,
 * e.g. file chunkers) generically using a lambda to process callback results
 */
template<typename CallbackFuncType, typename CallbackType,
	typename Nested1CallbackFuncType, typename Nested1CallbackType, typename Nested1ReturnType>
class TEOSCallbackWithNested1 :
	public TEOSCallback<CallbackFuncType, CallbackType>
{
public:
	TEOSCallbackWithNested1() = default;
	virtual ~TEOSCallbackWithNested1() = default;


	Nested1CallbackFuncType GetNested1CallbackPtr()
	{
		return &Nested1CallbackImpl;
	}

	void SetNested1CallbackLambda(TFunction<Nested1ReturnType(const Nested1CallbackType*)> InLambda)
	{
		Nested1CallbackLambda = InLambda;
	}

private:
	TFunction<Nested1ReturnType(const Nested1CallbackType*)> Nested1CallbackLambda;

	static Nested1ReturnType EOS_CALL Nested1CallbackImpl(const Nested1CallbackType* Data)
	{
		check(IsInGameThread());
		if (FCallbackBase::ShouldCancelAllCallbacks())
		{
			return Nested1ReturnType();
		}

		TEOSCallbackWithNested1* CallbackThis = (TEOSCallbackWithNested1*)Data->ClientData;
		check(CallbackThis);

		check(CallbackThis->CallbackLambda);
		return CallbackThis->Nested1CallbackLambda(Data);
	}
};

/**
 * Class to handle 2 nested callbacks (callbacks that are tied to an external callback's lifetime,
 * e.g. file chunkers) generically using a lambda to process callback results
 */
template<typename CallbackFuncType, typename CallbackType,
	typename Nested1CallbackFuncType, typename Nested1CallbackType, typename Nested1ReturnType,
	typename Nested2CallbackFuncType, typename Nested2CallbackType>
class TEOSCallbackWithNested2 :
	public TEOSCallbackWithNested1<CallbackFuncType, CallbackType, Nested1CallbackFuncType, Nested1CallbackType, Nested1ReturnType>
{
public:
	TEOSCallbackWithNested2() = default;
	virtual ~TEOSCallbackWithNested2() = default;


	Nested2CallbackFuncType GetNested2CallbackPtr()
	{
		return &Nested2CallbackImpl;
	}

	void SetNested2CallbackLambda(TFunction<void(const Nested2CallbackType*)> InLambda)
	{
		Nested2CallbackLambda = InLambda;
	}

private:
	TFunction<void(const Nested2CallbackType*)> Nested2CallbackLambda;

	static void EOS_CALL Nested2CallbackImpl(const Nested2CallbackType* Data)
	{
		check(IsInGameThread());
		if (FCallbackBase::ShouldCancelAllCallbacks())
		{
			return;
		}

		TEOSCallbackWithNested2* CallbackThis = (TEOSCallbackWithNested2*)Data->ClientData;
		check(CallbackThis);

		check(CallbackThis->CallbackLambda);
		CallbackThis->Nested2CallbackLambda(Data);
	}
};

/**
 * Class to handle nested callbacks (callbacks that are tied to an external callback's lifetime,
 * e.g. file chunkers) generically using a lambda to process callback results
 */
template<typename CallbackFuncType, typename CallbackType,
	typename Nested1CallbackFuncType, typename Nested1CallbackType, typename Nested1ReturnType>
	class TEOSCallbackWithNested1Param3 :
	public TEOSCallback<CallbackFuncType, CallbackType>
{
public:
	TEOSCallbackWithNested1Param3() = default;
	virtual ~TEOSCallbackWithNested1Param3() = default;


	Nested1CallbackFuncType GetNested1CallbackPtr()
	{
		return (Nested1CallbackFuncType)&Nested1CallbackImpl;
	}

	void SetNested1CallbackLambda(TFunction<Nested1ReturnType(const Nested1CallbackType*, void*, uint32_t*)> InLambda)
	{
		Nested1CallbackLambda = InLambda;
	}

private:
	TFunction<Nested1ReturnType(const Nested1CallbackType*, void*, uint32_t*)> Nested1CallbackLambda;

	static Nested1ReturnType EOS_CALL Nested1CallbackImpl(const Nested1CallbackType* Data, void* OutDataBuffer, uint32_t* OutDataWritten)
	{
		check(IsInGameThread());
		if (FCallbackBase::ShouldCancelAllCallbacks())
		{
			return Nested1ReturnType();
		}

		TEOSCallbackWithNested1Param3* CallbackThis = (TEOSCallbackWithNested1Param3*)Data->ClientData;
		check(CallbackThis);

		check(CallbackThis->CallbackLambda);
		return CallbackThis->Nested1CallbackLambda(Data, OutDataBuffer, OutDataWritten);
	}
};

/**
 * Class to handle 2 nested callbacks (callbacks that are tied to an external callback's lifetime,
 * e.g. file chunkers) generically using a lambda to process callback results
 */
template<typename CallbackFuncType, typename CallbackType,
	typename Nested1CallbackFuncType, typename Nested1CallbackType, typename Nested1ReturnType,
	typename Nested2CallbackFuncType, typename Nested2CallbackType>
	class TEOSCallbackWithNested2ForNested1Param3 :
	public TEOSCallbackWithNested1Param3<CallbackFuncType, CallbackType, Nested1CallbackFuncType, Nested1CallbackType, Nested1ReturnType>
{
public:
	TEOSCallbackWithNested2ForNested1Param3() = default;
	virtual ~TEOSCallbackWithNested2ForNested1Param3() = default;


	Nested2CallbackFuncType GetNested2CallbackPtr()
	{
		return &Nested2CallbackImpl;
	}

	void SetNested2CallbackLambda(TFunction<void(const Nested2CallbackType*)> InLambda)
	{
		Nested2CallbackLambda = InLambda;
	}

private:
	TFunction<void(const Nested2CallbackType*)> Nested2CallbackLambda;

	static void EOS_CALL Nested2CallbackImpl(const Nested2CallbackType* Data)
	{
		check(IsInGameThread());
		if (FCallbackBase::ShouldCancelAllCallbacks())
		{
			return;
		}

		TEOSCallbackWithNested2ForNested1Param3* CallbackThis = (TEOSCallbackWithNested2ForNested1Param3*)Data->ClientData;
		check(CallbackThis);

		check(CallbackThis->CallbackLambda);
		CallbackThis->Nested2CallbackLambda(Data);
	}
};

#include "eos_sessions_types.h"

/**
 * Implementation of session information
 */
class FOnlineSessionInfoEOS :
	public FOnlineSessionInfo
{
protected:
	/** Hidden on purpose */
	FOnlineSessionInfoEOS& operator=(const FOnlineSessionInfoEOS& Src)
	{
		return *this;
	}

PACKAGE_SCOPE:
	/** Constructor */
	FOnlineSessionInfoEOS();

	FOnlineSessionInfoEOS(const FOnlineSessionInfoEOS& Src)
		: FOnlineSessionInfo(Src)
		, HostAddr(Src.HostAddr)
		, SessionId(Src.SessionId)
		, SessionHandle(Src.SessionHandle)
		, bIsFromClone(true)
	{
	}

	FOnlineSessionInfoEOS(const FString& InHostIp, const FString& InSessionId, EOS_HSessionDetails InSessionHandle);

	FOnlineSessionInfoEOS(const FString& InHostIp, FUniqueNetIdEOSRef UniqueNetId, EOS_HSessionDetails InSessionHandle);

	/**
	 * Initialize LAN session
	 */
	void InitLAN(FOnlineSubsystemEOS* Subsystem);

	FString EOSAddress;
	/** The ip & port that the host is listening on (valid for LAN/GameServer) */
	TSharedPtr<class FInternetAddr> HostAddr;
	/** Unique Id for this session */
	FUniqueNetIdStringRef SessionId;
	/** EOS session handle. Note: this needs to be released by the SDK */
	EOS_HSessionDetails SessionHandle;
	/** Whether we should delete this handle or not */
	bool bIsFromClone;

public:
	virtual ~FOnlineSessionInfoEOS();
	bool operator==(const FOnlineSessionInfoEOS& Other) const
	{
		return false;
	}
	virtual const uint8* GetBytes() const override
	{
		return nullptr;
	}
	virtual int32 GetSize() const override
	{
		return sizeof(uint64) + sizeof(TSharedPtr<class FInternetAddr>);
	}
	virtual bool IsValid() const override
	{
		// LAN case
		return HostAddr.IsValid() && HostAddr->IsValid();
	}
	virtual FString ToString() const override
	{
		return SessionId->ToString();
	}
	virtual FString ToDebugString() const override
	{
		return FString::Printf(TEXT("HostIP: %s SessionId: %s"),
			HostAddr.IsValid() ? *HostAddr->ToString(true) : TEXT("INVALID"),
			*SessionId->ToDebugString());
	}
	virtual const FUniqueNetId& GetSessionId() const override
	{
		return *SessionId;
	}
};

#endif
