// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EOS_SDK

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

#if defined(EOS_PLATFORM_BASE_FILE_NAME)
#include EOS_PLATFORM_BASE_FILE_NAME
#endif
#include "eos_base.h"
#include "eos_common.h"
#include "eos_version.h"

#if defined(DISABLE_EOSVOICECHAT_ENGINE)
#define WITH_EOS_RTC 0
#else
#define WITH_EOS_RTC WITH_EOS_SDK && (EOS_MAJOR_VERSION >= 1 && EOS_MINOR_VERSION >= 13)
#endif

#define EOS_ENUM_FORWARD_DECL(name) enum class name : int32_t;
EOS_ENUM_FORWARD_DECL(EOS_EApplicationStatus);
EOS_ENUM_FORWARD_DECL(EOS_EAuthScopeFlags);
EOS_ENUM_FORWARD_DECL(EOS_EAuthTokenType);
EOS_ENUM_FORWARD_DECL(EOS_EDesktopCrossplayStatus);
EOS_ENUM_FORWARD_DECL(EOS_EFriendsStatus);
EOS_ENUM_FORWARD_DECL(EOS_ELoginCredentialType);
EOS_ENUM_FORWARD_DECL(EOS_ENetworkStatus);
EOS_ENUM_FORWARD_DECL(EOS_Presence_EStatus);
#undef EOS_ENUM_FORWARD_DECL

DECLARE_LOG_CATEGORY_EXTERN(LogEIKSDK, Log, All);

EIKSHARED_API FString EIK_LexToString(const EOS_EResult EosResult);
EIKSHARED_API FString EIK_LexToString(const EOS_ProductUserId UserId);
EIKSHARED_API void EIK_LexFromString(EOS_ProductUserId& UserId, const TCHAR* String);
inline EOS_ProductUserId EOSProductUserIdFromString(const TCHAR* String)
{
	EOS_ProductUserId UserId;
	EIK_LexFromString(UserId, String);
	return UserId;
}

EIKSHARED_API FString EIK_LexToString(const EOS_EpicAccountId AccountId);

EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_EApplicationStatus ApplicationStatus);
EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_EAuthTokenType AuthTokenType);
EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_EDesktopCrossplayStatus DesktopCrossplayStatus);
EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_EExternalAccountType ExternalAccountType);
EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_EFriendsStatus FriendStatus);
EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_ELoginStatus LoginStatus);
EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_ENetworkStatus NetworkStatus);
EIKSHARED_API const TCHAR* EIK_LexToString(const EOS_Presence_EStatus PresenceStatus);

EIKSHARED_API bool EIK_LexFromString(EOS_EAuthScopeFlags& OutEnum, const TCHAR* InString);
EIKSHARED_API bool EIK_LexFromString(EOS_EExternalCredentialType& OutEnum, const TCHAR* InString);
EIKSHARED_API bool EIK_LexFromString(EOS_ELoginCredentialType& OutEnum, const TCHAR* InString);

#endif // WITH_EOS_SDK