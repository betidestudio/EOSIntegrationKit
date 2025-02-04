// Copyright Epic Games, Inc. All Rights Reserved.

#include "EOSShared.h"
#include "EOSSharedTypes.h"

#include "eos_auth_types.h"
#include "eos_friends_types.h"
#include "eos_presence_types.h"
#include "eos_types.h"

DEFINE_LOG_CATEGORY(LogEIKSDK);

FString EIK_LexToString(const EOS_EResult EosResult)
{
	return UTF8_TO_TCHAR(EOS_EResult_ToString(EosResult));
}

FString EIK_LexToString(const EOS_ProductUserId UserId)
{
	FString Result;

	char ProductIdString[EOS_PRODUCTUSERID_MAX_LENGTH + 1];
	ProductIdString[0] = '\0';
	int32_t BufferSize = sizeof(ProductIdString);
	if (EOS_ProductUserId_IsValid(UserId) == EOS_TRUE &&
		EOS_ProductUserId_ToString(UserId, ProductIdString, &BufferSize) == EOS_EResult::EOS_Success)
	{
		Result = UTF8_TO_TCHAR(ProductIdString);
	}

	return Result;
}

void EIK_LexFromString(EOS_ProductUserId& UserId, const TCHAR* String)
{
	UserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(String));
}

FString EIK_LexToString(const EOS_EpicAccountId AccountId)
{
	FString Result;

	char AccountIdString[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
	AccountIdString[0] = '\0';
	int32_t BufferSize = sizeof(AccountIdString);
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_TRUE &&
		EOS_EpicAccountId_ToString(AccountId, AccountIdString, &BufferSize) == EOS_EResult::EOS_Success)
	{
		Result = UTF8_TO_TCHAR(AccountIdString);
	}

	return Result;
}

const TCHAR* EIK_LexToString(const EOS_EApplicationStatus ApplicationStatus)
{
	switch (ApplicationStatus)
	{
		case EOS_EApplicationStatus::EOS_AS_BackgroundConstrained:		return TEXT("BackgroundConstrained");
		case EOS_EApplicationStatus::EOS_AS_BackgroundUnconstrained:	return TEXT("BackgroundUnconstrained");
		case EOS_EApplicationStatus::EOS_AS_BackgroundSuspended:		return TEXT("BackgroundSuspended");
		case EOS_EApplicationStatus::EOS_AS_Foreground:					return TEXT("Foreground");
		default: checkNoEntry();										return TEXT("Unknown");
	}
}

const TCHAR* EIK_LexToString(const EOS_EAuthTokenType AuthTokenType)
{
	switch (AuthTokenType)
	{
		case EOS_EAuthTokenType::EOS_ATT_Client:	return TEXT("Client");
		case EOS_EAuthTokenType::EOS_ATT_User:		return TEXT("User");
		default: checkNoEntry();					return TEXT("Unknown");
	}
}

const TCHAR* EIK_LexToString(const EOS_EDesktopCrossplayStatus DesktopCrossplayStatus)
{
	switch (DesktopCrossplayStatus)
	{
		case EOS_EDesktopCrossplayStatus::EOS_DCS_OK:							return TEXT("OK");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_ApplicationNotBootstrapped:	return TEXT("ApplicationNotBootstrapped");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_ServiceNotInstalled:			return TEXT("ServiceNotInstalled");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_ServiceStartFailed:			return TEXT("ServiceStartFailed");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_ServiceNotRunning:			return TEXT("ServiceNotRunning");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_OverlayDisabled:				return TEXT("OverlayDisabled");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_OverlayNotInstalled:			return TEXT("OverlayNotInstalled");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_OverlayTrustCheckFailed:		return TEXT("OverlayTrustCheckFailed");
		case EOS_EDesktopCrossplayStatus::EOS_DCS_OverlayLoadFailed:			return TEXT("OverlayLoadFailed");
		default: checkNoEntry();												return TEXT("Unknown");
	}
}

const TCHAR* EIK_LexToString(const EOS_EExternalAccountType ExternalAccountType)
{
	switch (ExternalAccountType)
	{
		case EOS_EExternalAccountType::EOS_EAT_EPIC:		return TEXT("Epic");
		case EOS_EExternalAccountType::EOS_EAT_STEAM:		return TEXT("Steam");
		case EOS_EExternalAccountType::EOS_EAT_PSN:			return TEXT("PSN");
		case EOS_EExternalAccountType::EOS_EAT_XBL:			return TEXT("XBL");
		case EOS_EExternalAccountType::EOS_EAT_DISCORD:		return TEXT("Discord");
		case EOS_EExternalAccountType::EOS_EAT_GOG:			return TEXT("GOG");
		case EOS_EExternalAccountType::EOS_EAT_NINTENDO:	return TEXT("Nintendo");
		case EOS_EExternalAccountType::EOS_EAT_UPLAY:		return TEXT("UPlay");
		case EOS_EExternalAccountType::EOS_EAT_OPENID:		return TEXT("OpenID");
		case EOS_EExternalAccountType::EOS_EAT_APPLE:		return TEXT("Apple");
		case EOS_EExternalAccountType::EOS_EAT_GOOGLE:		return TEXT("Google");
		case EOS_EExternalAccountType::EOS_EAT_OCULUS:		return TEXT("Oculus");
		case EOS_EExternalAccountType::EOS_EAT_ITCHIO:		return TEXT("ItchIO");
		case EOS_EExternalAccountType::EOS_EAT_AMAZON:		return TEXT("Amazon");
		default: checkNoEntry();							return TEXT("Unknown");
	}
}

const TCHAR* EIK_LexToString(const EOS_EFriendsStatus FriendStatus)
{
	switch (FriendStatus)
	{
		default: checkNoEntry(); // Intentional fall through
		case EOS_EFriendsStatus::EOS_FS_NotFriends:		return TEXT("NotFriends");
		case EOS_EFriendsStatus::EOS_FS_InviteSent:		return TEXT("InviteSent");
		case EOS_EFriendsStatus::EOS_FS_InviteReceived: return TEXT("InviteReceived");
		case EOS_EFriendsStatus::EOS_FS_Friends:		return TEXT("Friends");
	}
}

const TCHAR* EIK_LexToString(const EOS_ELoginStatus LoginStatus)
{
	switch (LoginStatus)
	{
		case EOS_ELoginStatus::EOS_LS_NotLoggedIn:			return TEXT("NotLoggedIn");
		case EOS_ELoginStatus::EOS_LS_UsingLocalProfile:	return TEXT("UsingLocalProfile");
		case EOS_ELoginStatus::EOS_LS_LoggedIn:				return TEXT("LoggedIn");
		default: checkNoEntry();							return TEXT("Unknown");
	}
}

const TCHAR* EIK_LexToString(const EOS_ENetworkStatus NetworkStatus)
{
	switch (NetworkStatus)
	{
		case EOS_ENetworkStatus::EOS_NS_Disabled:	return TEXT("Disabled");
		case EOS_ENetworkStatus::EOS_NS_Offline:	return TEXT("Offline");
		case EOS_ENetworkStatus::EOS_NS_Online:		return TEXT("Online");
		default: checkNoEntry();					return TEXT("Unknown");
	}
}

const TCHAR* EIK_LexToString(const EOS_Presence_EStatus PresenceStatus)
{
	switch (PresenceStatus)
	{
		case EOS_Presence_EStatus::EOS_PS_Offline:		return TEXT("Offline");
		case EOS_Presence_EStatus::EOS_PS_Online:		return TEXT("Online");
		case EOS_Presence_EStatus::EOS_PS_Away:			return TEXT("Away");
		case EOS_Presence_EStatus::EOS_PS_ExtendedAway:	return TEXT("ExtendedAway");
		case EOS_Presence_EStatus::EOS_PS_DoNotDisturb:	return TEXT("DoNotDisturb");
		default: checkNoEntry();						return TEXT("Unknown");
	}
}

bool EIK_LexFromString(EOS_EExternalCredentialType& OutEnum, const TCHAR* InString)
{
	if (FCString::Stricmp(InString, TEXT("Steam")) == 0)
	{
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_STEAM_APP_TICKET;
	}
	else if (FCString::Stricmp(InString, TEXT("PSN")) == 0)
	{
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_PSN_ID_TOKEN;
	}
	else if (FCString::Stricmp(InString, TEXT("Xbox")) == 0)
	{
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_XBL_XSTS_TOKEN;
	}
	else if (FCString::Stricmp(InString, TEXT("Nintendo")) == 0)
	{
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_NINTENDO_ID_TOKEN;
	}
	else if (FCString::Stricmp(InString, TEXT("NSA")) == 0)
	{
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_NINTENDO_NSA_ID_TOKEN;
	}
	else if (FCString::Stricmp(InString, TEXT("Apple")) == 0)
	{
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_APPLE_ID_TOKEN;
	}
	else if (FCString::Stricmp(InString, TEXT("Google")) == 0)
	{
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_GOOGLE_ID_TOKEN;
	}
	else
	{
		// Unknown means OpenID
		OutEnum = EOS_EExternalCredentialType::EOS_ECT_OPENID_ACCESS_TOKEN;
	}

	return true;
}

bool EIK_LexFromString(EOS_EAuthScopeFlags& OutEnum, const TCHAR* InString)
{
	if (FCString::Stricmp(InString, TEXT("BasicProfile")) == 0)
	{
		OutEnum = EOS_EAuthScopeFlags::EOS_AS_BasicProfile;
	}
	else if (FCString::Stricmp(InString, TEXT("FriendsList")) == 0)
	{
		OutEnum = EOS_EAuthScopeFlags::EOS_AS_FriendsList;
	}
	else if (FCString::Stricmp(InString, TEXT("Presence")) == 0)
	{
		OutEnum = EOS_EAuthScopeFlags::EOS_AS_Presence;
	}
	else if (FCString::Stricmp(InString, TEXT("FriendsManagement")) == 0)
	{
		OutEnum = EOS_EAuthScopeFlags::EOS_AS_FriendsManagement;
	}
	else if (FCString::Stricmp(InString, TEXT("Email")) == 0)
	{
		OutEnum = EOS_EAuthScopeFlags::EOS_AS_Email;
	}
	else if (FCString::Stricmp(InString, TEXT("NoFlags")) == 0 || FCString::Stricmp(InString, TEXT("None")) == 0)
	{
		OutEnum = EOS_EAuthScopeFlags::EOS_AS_NoFlags;
	}
	else
	{
		return false;
	}
	return true;
}

bool EIK_LexFromString(EOS_ELoginCredentialType& OutEnum, const TCHAR* InString)
{
	if (FCString::Stricmp(InString, TEXT("ExchangeCode")) == 0)
	{
		OutEnum = EOS_ELoginCredentialType::EOS_LCT_ExchangeCode;
	}
	else if (FCString::Stricmp(InString, TEXT("PersistentAuth")) == 0)
	{
		OutEnum = EOS_ELoginCredentialType::EOS_LCT_PersistentAuth;
	}
	else if (FCString::Stricmp(InString, TEXT("Password")) == 0)
	{
		OutEnum = EOS_ELoginCredentialType::EOS_LCT_Password;
	}
	else if (FCString::Stricmp(InString, TEXT("Developer")) == 0)
	{
		OutEnum = EOS_ELoginCredentialType::EOS_LCT_Developer;
	}
	else if (FCString::Stricmp(InString, TEXT("RefreshToken")) == 0)
	{
		OutEnum = EOS_ELoginCredentialType::EOS_LCT_RefreshToken;
	}
	else if (FCString::Stricmp(InString, TEXT("AccountPortal")) == 0)
	{
		OutEnum = EOS_ELoginCredentialType::EOS_LCT_AccountPortal;
	}
	else if (FCString::Stricmp(InString, TEXT("ExternalAuth")) == 0)
	{
		OutEnum = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
	}
	else
	{
		return false;
	}
	return true;
}
