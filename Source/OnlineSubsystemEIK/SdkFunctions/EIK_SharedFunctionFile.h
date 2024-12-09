// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
THIRD_PARTY_INCLUDES_START
#include "eos_common.h"
#include "eos_ecom_types.h"
#include "eos_leaderboards_types.h"
#include "eos_ecom.h"
#include "eos_connect_types.h"
#include "eos_achievements_types.h"
#include "eos_auth_types.h"
#include "eos_friends_types.h"
#include "eos_sessions_types.h"
#include "eos_rtc_admin_types.h"
#include "eos_presence_types.h"
#include "eos_p2p.h"
#include "eos_p2p_types.h"
#include "eos_rtc.h"
#include "eos_stats.h"
#include "eos_stats_types.h"
#include "eos_rtc_audio.h"
#include "eos_rtc_types.h"
#include "eos_lobby_types.h"
#include "eos_lobby.h"
#include "eos_playerdatastorage.h"
#include "eos_sanctions.h"
#include "eos_sanctions_types.h"
#include "eos_playerdatastorage_types.h"
#include "eos_ui_types.h"
#include "eos_userinfo.h"
#include "eos_userinfo_types.h"
#include "eos_sessions_types.h"
THIRD_PARTY_INCLUDES_END
#include "UObject/Object.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "EIK_SharedFunctionFile.generated.h"

USTRUCT(BlueprintType)
struct FEIK_ProductUserId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS|Connect")
	FString ProductUserId;

private:
	EOS_ProductUserId ProductUserIdBasic;
public:
	FEIK_ProductUserId(): ProductUserIdBasic(nullptr)
	{
		ProductUserId = "";
	}

	FEIK_ProductUserId(EOS_ProductUserId InProductUserId)
	{
		ProductUserIdBasic = InProductUserId;
		char ProductIdAnsi[EOS_PRODUCTUSERID_MAX_LENGTH+1];
		int32 ProductIdLen;
		EOS_ProductUserId_ToString(InProductUserId, ProductIdAnsi, &ProductIdLen);
		ProductUserId = FString(UTF8_TO_TCHAR(ProductIdAnsi));
	}

	EOS_ProductUserId GetValueAsEosType()
	{
		if (EOS_ProductUserId_IsValid(ProductUserIdBasic))
		{
			return ProductUserIdBasic;
		}
		EOS_ProductUserId ProductUserIdSec = EOS_ProductUserId_FromString(TCHAR_TO_ANSI(*ProductUserId));
		return ProductUserIdSec;
	}
};

USTRUCT(BlueprintType)
struct FEIK_EpicAccountId 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString EpicAccountId;
	
private:
	EOS_EpicAccountId Ref;
public:
	FEIK_EpicAccountId(): Ref(nullptr)
	{
		EpicAccountId = "";
	}

	FEIK_EpicAccountId(EOS_EpicAccountId InEpicAccountId)
	{
		Ref = InEpicAccountId;

		char AccountIdString[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
		AccountIdString[0] = '\0';
		int32_t BufferSize = sizeof(AccountIdString);
		if (EOS_EpicAccountId_IsValid(InEpicAccountId) == EOS_TRUE &&
			EOS_EpicAccountId_ToString(InEpicAccountId, AccountIdString, &BufferSize) == EOS_EResult::EOS_Success)
		{
			EpicAccountId = UTF8_TO_TCHAR(AccountIdString);
		}
	}
	EOS_EpicAccountId GetValueAsEosType()
	{
		if (EOS_EpicAccountId_IsValid(Ref))
		{
			return Ref;
		}
		EOS_EpicAccountId EpicAccountIdSec = EOS_EpicAccountId_FromString(TCHAR_TO_ANSI(*EpicAccountId));
		return EpicAccountIdSec;
	}
	
};



UENUM(BlueprintType)
enum EIK_ELoginStatus
{
	EIK_LS_NotLoggedIn = 0 UMETA(DisplayName = "Not Logged In"),
	EIK_LS_UsingLocalProfile = 1 UMETA(DisplayName = "Using Local Profile"),
	EIK_LS_LoggedIn = 2 UMETA(DisplayName = "Logged In")
};

USTRUCT(BlueprintType)
struct FEIK_ContinuanceToken
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS|Connect")
	FString ContinuanceToken;

private:
	EOS_ContinuanceToken ContinuanceTokenBasic;
public:
	FEIK_ContinuanceToken(): ContinuanceTokenBasic(nullptr)
	{
		ContinuanceToken = "";
	}

	FEIK_ContinuanceToken(EOS_ContinuanceToken InContinuanceToken)
	{
		char* OutBuffer = new char[EOS_PRODUCTUSERID_MAX_LENGTH + 1];
		int32 OutBufferLen;
		EOS_ContinuanceToken_ToString(InContinuanceToken, OutBuffer, &OutBufferLen);
		ContinuanceToken = FString(UTF8_TO_TCHAR(OutBuffer));
		ContinuanceTokenBasic = InContinuanceToken;
	}
	
	EOS_ContinuanceToken GetValueAsEosType()
	{
		return ContinuanceTokenBasic;
	}
};


UENUM(BlueprintType)
enum EEIK_Result
{
    EOS_Success = 0,
    EOS_NoConnection = 1,
    EOS_InvalidCredentials = 2,
    EOS_InvalidUser = 3,
    EOS_InvalidAuth = 4,
    EOS_AccessDenied = 5,
    EOS_MissingPermissions = 6,
    EOS_Token_Not_Account = 7,
    EOS_TooManyRequests = 8,
    EOS_AlreadyPending = 9,
    EOS_InvalidParameters = 10,
    EOS_InvalidRequest = 11,
    EOS_UnrecognizedResponse = 12,
    EOS_IncompatibleVersion = 13,
    EOS_NotConfigured = 14,
    EOS_AlreadyConfigured = 15,
    EOS_NotImplemented = 16,
    EOS_Canceled = 17,
    EOS_NotFound = 18,
    EOS_OperationWillRetry = 19,
    EOS_NoChange = 20,
    EOS_VersionMismatch = 21,
    EOS_LimitExceeded = 22,
    EOS_Disabled = 23,
    EOS_DuplicateNotAllowed = 24,
    EOS_MissingParameters_DEPRECATED = 25,
    EOS_InvalidSandboxId = 26,
    EOS_TimedOut = 27,
    EOS_PartialResult = 28,
    EOS_Missing_Role = 29,
    EOS_Missing_Feature = 30,
    EOS_Invalid_Sandbox = 31,
    EOS_Invalid_Deployment = 32,
    EOS_Invalid_Product = 33,
    EOS_Invalid_ProductUserID = 34,
    EOS_ServiceFailure = 35,
    EOS_CacheDirectoryMissing = 36,
    EOS_CacheDirectoryInvalid = 37,
    EOS_InvalidState = 38,
    EOS_RequestInProgress = 39,
    EOS_ApplicationSuspended = 40,
    EOS_NetworkDisconnected = 41,
    EOS_Auth_AccountLocked = 1001,
    EOS_Auth_AccountLockedForUpdate = 1002,
    EOS_Auth_InvalidRefreshToken = 1003,
    EOS_Auth_InvalidToken = 1004,
    EOS_Auth_AuthenticationFailure = 1005,
    EOS_Auth_InvalidPlatformToken = 1006,
    EOS_Auth_WrongAccount = 1007,
    EOS_Auth_WrongClient = 1008,
    EOS_Auth_FullAccountRequired = 1009,
    EOS_Auth_HeadlessAccountRequired = 1010,
    EOS_Auth_PasswordResetRequired = 1011,
    EOS_Auth_PasswordCannotBeReused = 1012,
    EOS_Auth_Expired = 1013,
    EOS_Auth_ScopeConsentRequired = 1014,
    EOS_Auth_ApplicationNotFound = 1015,
    EOS_Auth_ScopeNotFound = 1016,
    EOS_Auth_AccountFeatureRestricted = 1017,
    EOS_Auth_AccountPortalLoadError = 1018,
    EOS_Auth_CorrectiveActionRequired = 1019,
    EOS_Auth_PinGrantCode = 1020,
    EOS_Auth_PinGrantExpired = 1021,
    EOS_Auth_PinGrantPending = 1022,
    EOS_Auth_ExternalAuthNotLinked = 1030,
    EOS_Auth_ExternalAuthRevoked = 1032,
    EOS_Auth_ExternalAuthInvalid = 1033,
    EOS_Auth_ExternalAuthRestricted = 1034,
    EOS_Auth_ExternalAuthCannotLogin = 1035,
    EOS_Auth_ExternalAuthExpired = 1036,
    EOS_Auth_ExternalAuthIsLastLoginType = 1037,
    EOS_Auth_ExchangeCodeNotFound = 1040,
    EOS_Auth_OriginatingExchangeCodeSessionExpired = 1041,
    EOS_Auth_AccountNotActive = 1050,
    EOS_Auth_MFARequired = 1060,
    EOS_Auth_ParentalControls = 1070,
    EOS_Auth_NoRealId = 1080,
    EOS_Auth_UserInterfaceRequired = 1090,
    EOS_Friends_InviteAwaitingAcceptance = 2000,
    EOS_Friends_NoInvitation = 2001,
    EOS_Friends_AlreadyFriends = 2003,
    EOS_Friends_NotFriends = 2004,
    EOS_Friends_TargetUserTooManyInvites = 2005,
    EOS_Friends_LocalUserTooManyInvites = 2006,
    EOS_Friends_TargetUserFriendLimitExceeded = 2007,
    EOS_Friends_LocalUserFriendLimitExceeded = 2008,
    EOS_Presence_DataInvalid = 3000,
    EOS_Presence_DataLengthInvalid = 3001,
    EOS_Presence_DataKeyInvalid = 3002,
    EOS_Presence_DataKeyLengthInvalid = 3003,
    EOS_Presence_DataValueInvalid = 3004,
    EOS_Presence_DataValueLengthInvalid = 3005,
    EOS_Presence_RichTextInvalid = 3006,
    EOS_Presence_RichTextLengthInvalid = 3007,
    EOS_Presence_StatusInvalid = 3008,
    EOS_Ecom_EntitlementStale = 4000,
    EOS_Ecom_CatalogOfferStale = 4001,
    EOS_Ecom_CatalogItemStale = 4002,
    EOS_Ecom_CatalogOfferPriceInvalid = 4003,
    EOS_Ecom_CheckoutLoadError = 4004,
    EOS_Ecom_PurchaseProcessing = 4005,
    EOS_Sessions_SessionInProgress = 5000,
    EOS_Sessions_TooManyPlayers = 5001,
    EOS_Sessions_NoPermission = 5002,
    EOS_Sessions_SessionAlreadyExists = 5003,
    EOS_Sessions_InvalidLock = 5004,
    EOS_Sessions_InvalidSession = 5005,
    EOS_Sessions_SandboxNotAllowed = 5006,
    EOS_Sessions_InviteFailed = 5007,
    EOS_Sessions_InviteNotFound = 5008,
    EOS_Sessions_UpsertNotAllowed = 5009,
    EOS_Sessions_AggregationFailed = 5010,
    EOS_Sessions_HostAtCapacity = 5011,
    EOS_Sessions_SandboxAtCapacity = 5012,
    EOS_Sessions_SessionNotAnonymous = 5013,
    EOS_Sessions_OutOfSync = 5014,
    EOS_Sessions_TooManyInvites = 5015,
    EOS_Sessions_PresenceSessionExists = 5016,
    EOS_Sessions_DeploymentAtCapacity = 5017,
    EOS_Sessions_NotAllowed = 5018,
    EOS_Sessions_PlayerSanctioned = 5019,
    EOS_PlayerDataStorage_FilenameInvalid = 6000,
    EOS_PlayerDataStorage_FilenameLengthInvalid = 6001,
    EOS_PlayerDataStorage_FilenameInvalidChars = 6002,
    EOS_PlayerDataStorage_FileSizeTooLarge = 6003,
    EOS_PlayerDataStorage_FileSizeInvalid = 6004,
    EOS_PlayerDataStorage_FileHandleInvalid = 6005,
    EOS_PlayerDataStorage_DataInvalid = 6006,
    EOS_PlayerDataStorage_DataLengthInvalid = 6007,
    EOS_PlayerDataStorage_StartIndexInvalid = 6008,
    EOS_PlayerDataStorage_RequestInProgress = 6009,
    EOS_PlayerDataStorage_UserThrottled = 6010,
    EOS_PlayerDataStorage_EncryptionKeyNotSet = 6011,
    EOS_PlayerDataStorage_UserErrorFromDataCallback = 6012,
    EOS_PlayerDataStorage_FileHeaderHasNewerVersion = 6013,
    EOS_PlayerDataStorage_FileCorrupted = 6014,
    EOS_Connect_ExternalTokenValidationFailed = 7000,
    EOS_Connect_UserAlreadyExists = 7001,
    EOS_Connect_AuthExpired = 7002,
    EOS_Connect_InvalidToken = 7003,
    EOS_Connect_UnsupportedTokenType = 7004,
    EOS_Connect_LinkAccountFailed = 7005,
    EOS_Connect_ExternalServiceUnavailable = 7006,
    EOS_Connect_ExternalServiceConfigurationFailure = 7007,
    EOS_Connect_LinkAccountFailedMissingNintendoIdAccount_DEPRECATED = 7008,
    EOS_UI_SocialOverlayLoadError = 8000,
    EOS_UI_InconsistentVirtualMemoryFunctions = 8001,
    EOS_Lobby_NotOwner = 9000,
    EOS_Lobby_InvalidLock = 9001,
    EOS_Lobby_LobbyAlreadyExists = 9002,
    EOS_Lobby_SessionInProgress = 9003,
    EOS_Lobby_TooManyPlayers = 9004,
    EOS_Lobby_NoPermission = 9005,
    EOS_Lobby_InvalidSession = 9006,
    EOS_Lobby_SandboxNotAllowed = 9007,
    EOS_Lobby_InviteFailed = 9008,
    EOS_Lobby_InviteNotFound = 9009,
    EOS_Lobby_UpsertNotAllowed = 9010,
    EOS_Lobby_AggregationFailed = 9011,
    EOS_Lobby_HostAtCapacity = 9012,
    EOS_Lobby_SandboxAtCapacity = 9013,
    EOS_Lobby_TooManyInvites = 9014,
    EOS_Lobby_DeploymentAtCapacity = 9015,
    EOS_Lobby_NotAllowed = 9016,
    EOS_Lobby_MemberUpdateOnly = 9017,
    EOS_Lobby_PresenceLobbyExists = 9018,
    EOS_Lobby_VoiceNotEnabled = 9019,
    EOS_Lobby_PlatformNotAllowed = 9020,
    EOS_TitleStorage_UserErrorFromDataCallback = 10000,
    EOS_TitleStorage_EncryptionKeyNotSet = 10001,
    EOS_TitleStorage_FileCorrupted = 10002,
    EOS_TitleStorage_FileHeaderHasNewerVersion = 10003,
    EOS_Mods_ModSdkProcessIsAlreadyRunning = 11000,
    EOS_Mods_ModSdkCommandIsEmpty = 11001,
    EOS_Mods_ModSdkProcessCreationFailed = 11002,
    EOS_Mods_CriticalError = 11003,
    EOS_Mods_ToolInternalError = 11004,
    EOS_Mods_IPCFailure = 11005,
    EOS_Mods_InvalidIPCResponse = 11006,
    EOS_Mods_URILaunchFailure = 11007,
    EOS_Mods_ModIsNotInstalled = 11008,
    EOS_Mods_UserDoesNotOwnTheGame = 11009,
    EOS_Mods_OfferRequestByIdInvalidResult = 11010,
    EOS_Mods_CouldNotFindOffer = 11011,
    EOS_Mods_OfferRequestByIdFailure = 11012,
    EOS_Mods_PurchaseFailure = 11013,
    EOS_Mods_InvalidGameInstallInfo = 11014,
    EOS_Mods_CannotGetManifestLocation = 11015,
    EOS_Mods_UnsupportedOS = 11016,
    EOS_AntiCheat_ClientProtectionNotAvailable = 12000,
    EOS_AntiCheat_InvalidMode = 12001,
    EOS_AntiCheat_ClientProductIdMismatch = 12002,
    EOS_AntiCheat_ClientSandboxIdMismatch = 12003,
    EOS_AntiCheat_ProtectMessageSessionKeyRequired = 12004,
    EOS_AntiCheat_ProtectMessageValidationFailed = 12005,
    EOS_AntiCheat_ProtectMessageInitializationFailed = 12006,
    EOS_AntiCheat_PeerAlreadyRegistered = 12007,
    EOS_AntiCheat_PeerNotFound = 12008,
    EOS_AntiCheat_PeerNotProtected = 12009,
    EOS_AntiCheat_ClientDeploymentIdMismatch = 12010,
    EOS_AntiCheat_DeviceIdAuthIsNotSupported = 12011,
    EOS_RTC_TooManyParticipants = 13000,
    EOS_RTC_RoomAlreadyExists = 13001,
    EOS_RTC_UserKicked = 13002,
    EOS_RTC_UserBanned = 13003,
    EOS_RTC_RoomWasLeft = 13004,
    EOS_RTC_ReconnectionTimegateExpired = 13005,
    EOS_RTC_ShutdownInvoked = 13006,
    EOS_RTC_UserIsInBlocklist = 13007,
    EOS_ProgressionSnapshot_SnapshotIdUnavailable = 14000,
    EOS_KWS_ParentEmailMissing = 15000,
    EOS_KWS_UserGraduated = 15001,
    EOS_Android_JavaVMNotStored = 17000,
    EOS_Android_ReservedMustReferenceLocalVM = 17001,
    EOS_Android_ReservedMustBeNull = 17002,
    EOS_Permission_RequiredPatchAvailable = 18000,
    EOS_Permission_RequiredSystemUpdate = 18001,
    EOS_Permission_AgeRestrictionFailure = 18002,
    EOS_Permission_AccountTypeFailure = 18003,
    EOS_Permission_ChatRestriction = 18004,
    EOS_Permission_UGCRestriction = 18005,
    EOS_Permission_OnlinePlayRestricted = 18006,
    EOS_DesktopCrossplay_ApplicationNotBootstrapped = 19000,
    EOS_DesktopCrossplay_ServiceNotInstalled = 19001,
    EOS_DesktopCrossplay_ServiceStartFailed = 19002,
    EOS_DesktopCrossplay_ServiceNotRunning = 19003,
    EOS_CustomInvites_InviteFailed = 20000,
    EOS_UserInfo_BestDisplayNameIndeterminate = 22000,
    EOS_ConsoleInit_OnNetworkRequestedDeprecatedCallbackNotSet = 23000,
    EOS_ConsoleInit_CacheStorage_SizeKBNotMultipleOf16 = 23001,
    EOS_ConsoleInit_CacheStorage_SizeKBBelowMinimumSize = 23002,
    EOS_ConsoleInit_CacheStorage_SizeKBExceedsMaximumSize = 23003,
    EOS_ConsoleInit_CacheStorage_IndexOutOfRangeRange = 23004,
    EOS_UnexpectedError = 0x7FFFFFFF

};

USTRUCT(BlueprintType)
struct FEIK_NotificationId
{
	GENERATED_BODY()

	UPROPERTY()
	int64 NotificationId;

	FEIK_NotificationId()
	{
		NotificationId = 0;
	}

	FEIK_NotificationId(EOS_NotificationId InNotificationId)
	{
		NotificationId = InNotificationId;
	}

	EOS_NotificationId GetValueAsEosType()
	{
		return NotificationId;
	}
};


USTRUCT(BlueprintType)
struct FEIK_Connect_IdToken
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FEIK_ProductUserId UserId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FString Token;

	FEIK_Connect_IdToken()
	{
		UserId = FEIK_ProductUserId();
		Token = "";
	}

	FEIK_Connect_IdToken(EOS_Connect_IdToken InIdToken)
	{
		UserId = InIdToken.ProductUserId;
		Token = UTF8_TO_TCHAR(InIdToken.JsonWebToken);
	}

	EOS_Connect_IdToken GetValueAsEosType()
	{
		EOS_Connect_IdToken IdToken;
		IdToken.ApiVersion = EOS_CONNECT_IDTOKEN_API_LATEST;
		IdToken.ProductUserId = UserId.GetValueAsEosType();
		IdToken.JsonWebToken = TCHAR_TO_ANSI(*Token);
		return IdToken;
	}
	
};

UENUM(BlueprintType)
enum EEIK_EExternalCredentialType
{
	/**
	 * Epic Account Services Token
	 *
	 * Using ID Token is preferred, retrieved with EOS_Auth_CopyIdToken that returns EOS_Auth_IdToken::JsonWebToken.
	 * Using Auth Token is supported for backwards compatibility, retrieved with EOS_Auth_CopyUserAuthToken that returns EOS_Auth_Token::AccessToken.
	 *
	 * Supported with EOS_Connect_Login.
	 *
	 * @see EOS_Auth_CopyIdToken
	 * @see EOS_Auth_CopyUserAuthToken
	 */
	EIK_ECT_EPIC = 0 UMETA(DisplayName = "Epic Games"),
	/**
	 * Steam Encrypted App Ticket
	 * Note that EOS_ECT_STEAM_APP_TICKET is deprecated for use with EOS_Auth_Login. Use EOS_ECT_STEAM_SESSION_TICKET instead.
	 *
	 * Generated using the ISteamUser::RequestEncryptedAppTicket API of Steamworks SDK.
	 * For ticket generation parameters, use pDataToInclude(NULL) and cbDataToInclude(0).
	 *
	 * The retrieved App Ticket byte buffer needs to be converted into a hex-encoded UTF-8 string (e.g. "FA87097A..") before passing it to the EOS_Connect_Login API.
	 * EOS_ByteArray_ToString can be used for this conversion.
	 *
	 * Supported with EOS_Connect_Login.
	 *
	 * @see EOS_ECT_STEAM_SESSION_TICKET
	 */
	EIK_ECT_STEAM_APP_TICKET = 1 UMETA(DisplayName = "Steam"),
	/**
	 * PlayStation(TM)Network ID Token
	 *
	 * Retrieved from the PlayStation(R) SDK. Please see first-party documentation for additional information.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EIK_ECT_PSN_ID_TOKEN = 2 UMETA(DisplayName = "PlayStation Network"),
	/**
	 * Xbox Live XSTS Token
	 *
	 * Retrieved from the GDK and XDK. Please see first-party documentation for additional information.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EIK_ECT_XBL_XSTS_TOKEN = 3 UMETA(DisplayName = "Xbox Live"),
	/**
	 * Discord Access Token
	 *
	 * Retrieved using the ApplicationManager::GetOAuth2Token API of Discord SDK.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_DISCORD_ACCESS_TOKEN = 4 UMETA(DisplayName = "Discord"),
	/**
	 * GOG Galaxy Encrypted App Ticket
	 *
	 * Generated using the IUser::RequestEncryptedAppTicket API of GOG Galaxy SDK.
	 * For ticket generation parameters, use data(NULL) and dataSize(0).
	 *
	 * The retrieved App Ticket byte buffer needs to be converted into a hex-encoded UTF-8 string (e.g. "FA87097A..") before passing it to the EOS_Connect_Login API.
	 * For C/C++ API integration, use the EOS_ByteArray_ToString API for the conversion.
	 * For C# integration, you can use <see cref="Helper.ToHexString" /> for the conversion.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_GOG_SESSION_TICKET = 5 UMETA(DisplayName = "GOG Galaxy"),
	/**
	 * Nintendo Account ID Token
	 *
	 * Identifies a Nintendo user account and is acquired through web flow authentication where the local user logs in using their email address/sign-in ID and password.
	 * This is the common Nintendo account that users login with outside the Nintendo Switch device.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EIK_ECT_NINTENDO_ID_TOKEN = 6 UMETA(DisplayName = "Nintendo"),
	/**
	 * Nintendo Service Account ID Token (NSA ID)
	 *
	 * The NSA ID identifies uniquely the local Nintendo Switch device. The authentication token is acquired locally without explicit user credentials.
	 * As such, it is the primary authentication method for seamless login on Nintendo Switch.
	 *
	 * The NSA ID is not exposed directly to the user and does not provide any means for login outside the local device.
	 * Because of this, Nintendo Switch users will need to link their Nintendo Account or another external user account
	 * to their Product User ID in order to share their game progression across other platforms. Otherwise, the user will
	 * not be able to login to their existing Product User ID on another platform due to missing login credentials to use.
	 * It is recommended that the game explicitly communicates this restriction to the user so that they will know to add
	 * the first linked external account on the Nintendo Switch device and then proceed with login on another platform.
	 *
	 * In addition to sharing cross-platform game progression, linking the Nintendo Account or another external account
	 * will allow preserving the game progression permanently. Otherwise, the game progression will be tied only to the
	 * local device. In case the user loses access to their local device, they will not be able to recover the game
	 * progression if it is only associated with this account type.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EIK_ECT_NINTENDO_NSA_ID_TOKEN = 7 UMETA(DisplayName = "Nintendo Service"),
	/**
	 * Uplay Access Token
	 */
	EIK_ECT_UPLAY_ACCESS_TOKEN = 8 UMETA(DisplayName = "Uplay"),
	/**
	 * OpenID Provider Access Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_OPENID_ACCESS_TOKEN = 9 UMETA(DisplayName = "OpenID Provider"),
	/**
	 * Device ID access token that identifies the current locally logged in user profile on the local device.
	 * The local user profile here refers to the operating system user login, for example the user's Windows Account
	 * or on a mobile device the default active user profile.
	 *
	 * This credential type is used to automatically login the local user using the EOS Connect Device ID feature.
	 *
	 * The intended use of the Device ID feature is to allow automatically logging in the user on a mobile device
	 * and to allow playing the game without requiring the user to necessarily login using a real user account at all.
	 * This makes a seamless first-time experience possible and allows linking the local device with a real external
	 * user account at a later time, sharing the same EOS_ProductUserId that is being used with the Device ID feature.
	 *
	 * Supported with EOS_Connect_Login.
	 *
	 * @see EOS_Connect_CreateDeviceId
	 */
	EIK_ECT_DEVICEID_ACCESS_TOKEN = 10 UMETA(DisplayName = "Device ID"),
	/**
	 * Apple ID Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_APPLE_ID_TOKEN = 11,
	/**
	 * Google ID Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_GOOGLE_ID_TOKEN = 12 UMETA(DisplayName = "Google"),
	/**
	 * Oculus User ID and Nonce
	 *
	 * Call ovr_User_GetUserProof(), or Platform.User.GetUserProof() if you are using Unity, to retrieve the nonce.
	 * Then pass the local User ID and the Nonce as a "{UserID}|{Nonce}" formatted string for the EOS_Connect_Login Token parameter.
	 *
	 * Note that in order to successfully retrieve a valid non-zero id for the local user using ovr_User_GetUser(),
	 * your Oculus App needs to be configured in the Oculus Developer Dashboard to have the User ID feature enabled.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_OCULUS_USERID_NONCE = 13 UMETA(DisplayName = "Oculus"),
	/**
	 * itch.io JWT Access Token
	 *
	 * Use the itch.io app manifest to receive a JWT access token for the local user via the ITCHIO_API_KEY process environment variable.
	 * The itch.io access token is valid for 7 days after which the game needs to be restarted by the user as otherwise EOS Connect
	 * authentication session can no longer be refreshed.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_ITCHIO_JWT = 14 UMETA(DisplayName = "itch.io JWT"),
	/**
	 * itch.io Key Access Token
	 *
	 * This access token type is retrieved through the OAuth 2.0 authentication flow for the itch.io application.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_ITCHIO_KEY = 15 UMETA(DisplayName = "itch.io Key"),
	/**
	 * Epic Games ID Token
	 *
	 * Acquired using EOS_Auth_CopyIdToken that returns EOS_Auth_IdToken::JsonWebToken.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_EPIC_ID_TOKEN = 16 UMETA(DisplayName = "Epic Games ID Token"),
	/**
	 * Amazon Access Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_AMAZON_ACCESS_TOKEN = 17 UMETA(DisplayName = "Amazon Access Token"),
	/**
	 * Steam Auth Session Ticket
	 *
	 * Generated using the ISteamUser::GetAuthTicketForWebApi API of Steamworks SDK.
	 *
	 * @attention
	 * The pchIdentity input parameter of GetAuthTicketForWebApi API must be set to a valid non-empty string value.
	 * The string value used by the game client must match identically to the backend-configured value in EOS Dev Portal.
	 * The recommended value to use is "epiconlineservices" in lowercase, matching the default value for new Steam identity provider credentials in EOS Dev Portal.
	 * This identifier is important for security reasons to prevent session hijacking. Applications must use a dedicated unique identity identifier for Session Tickets passed to the EOS SDK APIs.
	 * Session Tickets using the EOS-assigned identifier must not be used with anything else than the EOS SDK APIs. You must use a different identifier when generating Session Tickets to authenticate with other parties.
	 *
	 * @warning
	 * To update an already live game to use the new GetAuthTicketForWebApi API instead of the deprecated GetAuthSessionTicket API, follow these steps in this order to prevent breaking the live game for players:
	 * 1. Update your game client code to use the new ISteamUser::GetAuthTicketForWebApi API.
	 * 2. Publish the new game client update to end-users.
	 * 3. Update the existing Steam identity provider credentials entry in EOS Dev Portal to use the same identity string identifier as the game client.
	 *
	 * @example
	 * SteamUser()->GetAuthTicketForWebApi("epiconlineservices");
	 *
	 * The retrieved Auth Session Ticket byte buffer needs to be converted into a hex-encoded UTF-8 string (e.g. "FA87097A..") before passing it to the EOS_Auth_Login or EOS_Connect_Login APIs.
	 * EOS_ByteArray_ToString can be used for this conversion.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 *
	 * @version 1.15.1+
	 */
	EIK_ECT_STEAM_SESSION_TICKET = 18 UMETA(DisplayName = "Steam Session Ticket"),
	/**
	 * VIVEPORT User Session Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_VIVEPORT_USER_TOKEN = 19 UMETA(DisplayName = "VIVEPORT User Token"),

};
UENUM(BlueprintType)
enum EEIK_EExternalAccountType
{
	/** External account is associated with Epic Games */
	EIK_EAT_EPIC = 0 UMETA(DisplayName = "Epic Games"),
	/** External account is associated with Steam */
	EIK_EAT_STEAM = 1 UMETA(DisplayName = "Steam"),
	/** External account is associated with PlayStation(TM)Network */
	EIK_EAT_PSN = 2 UMETA(DisplayName = "PlayStation Network"),
	/** External account is associated with Xbox Live */
	EIK_EAT_XBL = 3 UMETA(DisplayName = "Xbox Live"),
	/** External account is associated with Discord */
	EIK_EAT_DISCORD = 4	UMETA(DisplayName = "Discord"),
	/** External account is associated with GOG */
	EIK_EAT_GOG = 5 UMETA(DisplayName = "GOG"),
	/**
	 * External account is associated with Nintendo
	 *
	 * With both EOS Connect and EOS UserInfo APIs, the associated account type is Nintendo Service Account ID.
	 * Local user authentication is possible using Nintendo Account ID, while the account type does not get exposed to the SDK in queries related to linked accounts information.
	 */
	EIK_EAT_NINTENDO = 6 UMETA(DisplayName = "Nintendo"),
	/** External account is associated with Uplay */
	EIK_EAT_UPLAY = 7 UMETA(DisplayName = "Uplay"),
	/** External account is associated with an OpenID Provider */
	EIK_EAT_OPENID = 8 UMETA(DisplayName = "OpenID"),
	/** External account is associated with Apple */
	EIK_EAT_APPLE = 9 UMETA(DisplayName = "Apple"),
	/** External account is associated with Google */
	EIK_EAT_GOOGLE = 10 UMETA(DisplayName = "Google"),
	/** External account is associated with Oculus */
	EIK_EAT_OCULUS = 11 UMETA(DisplayName = "Oculus"),
	/** External account is associated with itch.io */
	EIK_EAT_ITCHIO = 12 UMETA(DisplayName = "itch.io"),
	/** External account is associated with Amazon */
	EIK_EAT_AMAZON = 13 UMETA(DisplayName = "Amazon"),
	/** External account is associated with Viveport */
	EIK_EAT_VIVEPORT = 14 UMETA(DisplayName = "Viveport"),
};



USTRUCT(BlueprintType)
struct FEIK_Connect_ExternalAccountInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FEIK_ProductUserId UserId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FString AccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	int64 LastLoginTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	TEnumAsByte<EEIK_EExternalAccountType> AccountType;
	
	FEIK_Connect_ExternalAccountInfo()
	{
		UserId = FEIK_ProductUserId();
		AccountId = "";
		DisplayName = "";
		LastLoginTime = 0;
		AccountType = EIK_EAT_EPIC;
	}
	FEIK_Connect_ExternalAccountInfo(EOS_Connect_ExternalAccountInfo InExternalAccountInfo)
	{
		UserId = InExternalAccountInfo.ProductUserId;
		AccountId = FString(UTF8_TO_TCHAR(InExternalAccountInfo.AccountId));
		DisplayName = FString(UTF8_TO_TCHAR(InExternalAccountInfo.DisplayName));
		LastLoginTime = InExternalAccountInfo.LastLoginTime;
		AccountType = static_cast<EEIK_EExternalAccountType>(InExternalAccountInfo.AccountIdType);
	}
	EOS_Connect_ExternalAccountInfo GetValueAsEosType()
	{
		EOS_Connect_ExternalAccountInfo ExternalAccountInfo;
		ExternalAccountInfo.ApiVersion = EOS_CONNECT_EXTERNALACCOUNTINFO_API_LATEST;
		ExternalAccountInfo.ProductUserId = UserId.GetValueAsEosType();
		if(AccountId.Len() > 0)
		{
			ExternalAccountInfo.AccountId = TCHAR_TO_ANSI(*AccountId);
		}
		else
		{
			ExternalAccountInfo.AccountId = nullptr;
		}
		if(DisplayName.Len() > 0)
		{
			ExternalAccountInfo.DisplayName = TCHAR_TO_ANSI(*DisplayName);
		}
		else
		{
			ExternalAccountInfo.DisplayName = nullptr;
		}
		ExternalAccountInfo.LastLoginTime = LastLoginTime;
		ExternalAccountInfo.AccountIdType = static_cast<EOS_EExternalAccountType>(AccountType.GetValue());
		return ExternalAccountInfo;
	}
	
};


USTRUCT(BlueprintType)
struct FEIK_Achievements_DefinitionV2
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString AchievementId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString UnlockedDisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString UnlockedDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString LockedDisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString LockedDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString FlavorText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString UnlockedIconURL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString LockedIconURL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	bool bIsHidden;
private:
	EOS_Achievements_DefinitionV2* DefinitionRef;
public:
	FEIK_Achievements_DefinitionV2(): bIsHidden(false), DefinitionRef(nullptr)
	{
	}

	FEIK_Achievements_DefinitionV2(EOS_Achievements_DefinitionV2 InDefinition)
	{
		DefinitionRef = &InDefinition;
		AchievementId = FString(UTF8_TO_TCHAR(InDefinition.AchievementId));
		UnlockedDisplayName = FString(UTF8_TO_TCHAR(InDefinition.UnlockedDisplayName));
		UnlockedDescription = FString(UTF8_TO_TCHAR(InDefinition.UnlockedDescription));
		LockedDisplayName = FString(UTF8_TO_TCHAR(InDefinition.LockedDisplayName));
		LockedDescription = FString(UTF8_TO_TCHAR(InDefinition.LockedDescription));
		FlavorText = FString(UTF8_TO_TCHAR(InDefinition.FlavorText));
		UnlockedIconURL = FString(UTF8_TO_TCHAR(InDefinition.UnlockedIconURL));
		LockedIconURL = FString(UTF8_TO_TCHAR(InDefinition.LockedIconURL));
		bIsHidden = InDefinition.bIsHidden == EOS_TRUE;
	}
	EOS_Achievements_DefinitionV2* GetValueAsEosType()
	{
		return DefinitionRef;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Achievements_PlayerStatInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	int32 CurrentValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	int32 ThresholdValue;

	FEIK_Achievements_PlayerStatInfo()
	{
		Name = "";
		CurrentValue = 0;
		ThresholdValue = 0;
	}
	FEIK_Achievements_PlayerStatInfo(EOS_Achievements_PlayerStatInfo InStatInfo)
	{
		Name = UTF8_TO_TCHAR(InStatInfo.Name);
		CurrentValue = InStatInfo.CurrentValue;
		ThresholdValue = InStatInfo.ThresholdValue;
	}
	
};

USTRUCT(BlueprintType)
struct FEIK_Achievements_PlayerAchievement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString AchievementId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	float Progress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	int64 UnlockTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	int32 StatInfoCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	TArray<FEIK_Achievements_PlayerStatInfo> StatInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString IconURL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Achievements Interface")
	FString FlavorText;

	EOS_Achievements_PlayerAchievement* PlayerAchievementRef;

	FEIK_Achievements_PlayerAchievement(): PlayerAchievementRef(nullptr)
	{
		AchievementId = "";
		Progress = 0;
		UnlockTime = 0;
		StatInfoCount = 0;
		StatInfo = TArray<FEIK_Achievements_PlayerStatInfo>();
		DisplayName = "";
		Description = "";
		IconURL = "";
		FlavorText = "";
	}

	FEIK_Achievements_PlayerAchievement(EOS_Achievements_PlayerAchievement InPlayerAchievement)
	{
		PlayerAchievementRef = &InPlayerAchievement;
		AchievementId = UTF8_TO_TCHAR(InPlayerAchievement.AchievementId);
		Progress = InPlayerAchievement.Progress;
		UnlockTime = InPlayerAchievement.UnlockTime;
		StatInfoCount = InPlayerAchievement.StatInfoCount;
		for (int32 i = 0; i < InPlayerAchievement.StatInfoCount; i++)
		{
			StatInfo.Add(InPlayerAchievement.StatInfo[i]);
		}
		DisplayName = UTF8_TO_TCHAR(InPlayerAchievement.DisplayName);
		Description = UTF8_TO_TCHAR(InPlayerAchievement.Description);
		IconURL = UTF8_TO_TCHAR(InPlayerAchievement.IconURL);
		FlavorText = UTF8_TO_TCHAR(InPlayerAchievement.FlavorText);
	}

	EOS_Achievements_PlayerAchievement* GetValueAsEosType()
	{
		return PlayerAchievementRef;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Auth_IdToken
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FEIK_EpicAccountId AccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString JsonWebToken;

	EOS_Auth_IdToken TokenRef;

	FEIK_Auth_IdToken(): TokenRef()
	{
		AccountId = FEIK_EpicAccountId();
		JsonWebToken = "";
	}

	FEIK_Auth_IdToken(EOS_Auth_IdToken InIdToken)
	{
		TokenRef = InIdToken;
		AccountId = InIdToken.AccountId;
		JsonWebToken = UTF8_TO_TCHAR(InIdToken.JsonWebToken);
	}
	EOS_Auth_IdToken GetValueAsEosType()
	{
		return TokenRef;
	}
};

UENUM(BlueprintType)
enum EEIK_EAuthTokenType
{
	EIK_ATT_Client = 0 UMETA(DisplayName = "Client"),
	EIK_ATT_User = 1 UMETA(DisplayName = "User")
};

USTRUCT(BlueprintType)
struct FEIK_Auth_Token
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString App;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString ClientId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FEIK_EpicAccountId AccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString AccessToken;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	float ExpiresIn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString ExpiresAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	TEnumAsByte<EEIK_EAuthTokenType> AuthType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString RefreshToken;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	float RefreshExpiresIn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString RefreshExpiresAt;

	EOS_Auth_Token TokenRef;

	FEIK_Auth_Token(): TokenRef()
	{
		App = "";
		ClientId = "";
		AccountId = FEIK_EpicAccountId();
		AccessToken = "";
		ExpiresIn = 0;
		ExpiresAt = "";
		AuthType = EIK_ATT_Client;
		RefreshToken = "";
		RefreshExpiresIn = 0;
		RefreshExpiresAt = "";
	}

	FEIK_Auth_Token(EOS_Auth_Token InToken): TokenRef()
	{
		TokenRef = InToken;
		App = UTF8_TO_TCHAR(InToken.App);
		ClientId = UTF8_TO_TCHAR(InToken.ClientId);
		AccountId = InToken.AccountId;
		AccessToken = UTF8_TO_TCHAR(InToken.AccessToken);
		ExpiresIn = InToken.ExpiresIn;
		ExpiresAt = UTF8_TO_TCHAR(InToken.ExpiresAt);
		AuthType = static_cast<EEIK_EAuthTokenType>(InToken.AuthType);
		RefreshToken = UTF8_TO_TCHAR(InToken.RefreshToken);
		RefreshExpiresIn = InToken.RefreshExpiresIn;
		RefreshExpiresAt = UTF8_TO_TCHAR(InToken.RefreshExpiresAt);
	}
	EOS_Auth_Token GetValueAsEosType()
	{
		return TokenRef;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Auth_PinGrantInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString UserCode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString VerificationURI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	float ExpiresIn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString VerificationURIComplete;

	EOS_Auth_PinGrantInfo PinGrantInfo;

	FEIK_Auth_PinGrantInfo(): ExpiresIn(0), PinGrantInfo()
	{
		UserCode = "";
		VerificationURI = "";
		VerificationURIComplete = "";
	}

	FEIK_Auth_PinGrantInfo(EOS_Auth_PinGrantInfo InPinGrantInfo)
	{
		PinGrantInfo = InPinGrantInfo;
		UserCode = UTF8_TO_TCHAR(InPinGrantInfo.UserCode);
		VerificationURI = UTF8_TO_TCHAR(InPinGrantInfo.VerificationURI);
		ExpiresIn = InPinGrantInfo.ExpiresIn;
		VerificationURIComplete = UTF8_TO_TCHAR(InPinGrantInfo.VerificationURIComplete);
	}

	EOS_Auth_PinGrantInfo GetValueAsEosType()
	{
		return PinGrantInfo;
	}
	
};

UENUM(BlueprintType)
enum EEIK_ELinkAccountFlags
{
	EIK_LA_NoFlags = 0x0,
	EIK_LA_NintendoNsaId = 0x1
};

UENUM(BlueprintType)
enum EEIK_ELoginCredentialType
{
	//Login using account email address and password.
	EIK_LCT_Password = 0 UMETA(DisplayName = "Password"),
	//A short-lived one-time use exchange code to login the local user. When started, the application is expected to consume the exchange code by using the EOS_Auth_Login API as soon as possible. This is needed in order to authenticate the local user before the exchange code would expire. Attempting to consume an already expired exchange code will return EOS_EResult::EOS_Auth_ExchangeCodeNotFound error by the EOS_Auth_Login API.
	EIK_LCT_ExchangeCode = 1 UMETA(DisplayName = "Exchange Code"),
	//Used by standalone applications distributed outside the supported game platforms such as Epic Games Store or Steam, and on Nintendo Switch. Persistent Auth is used in conjunction with the EOS_LCT_AccountPortal login method for automatic login of the local user across multiple runs of the application. Standalone applications implement the login sequence as follows: 1. Application calls EOS_Auth_Login with EOS_LCT_PersistentAuth, using a previously stored Epic refresh token for an automatic user login. 2. If automatic login fails, the application discards the Epic refresh token used as defunct, and proceeds to call EOS_Auth_Login with EOS_LCT_AccountPortal to prompt the user for manual login. On Nintendo Switch, after a successful login the refresh token must be retrieved using the EOS_Auth_CopyUserAuthToken API and stored by the application specifically for the active Nintendo Switch user.
	EIK_LCT_PersistentAuth = 2 UMETA(DisplayName = "Persistent Auth"),
	//Not supported. Superseded by EOS_LCT_ExternalAuth login method.
	EIK_LCT_DeviceCode = 3 UMETA(DisplayName = "Device Code"),
	//Login with named credentials hosted by the EOS SDK Developer Authentication Tool.
	EIK_LCT_Developer = 4 UMETA(DisplayName = "Developer"),
	//Refresh token that was retrieved from a previous call to EOS_Auth_Login API in another local process context. Mainly used in conjunction with custom desktop launcher applications. in-between that requires authenticating the user before eventually starting the actual game client application. In such scenario, an intermediate launcher will log in the user by consuming the exchange code it received from the Epic Games Launcher. To allow the game client to also authenticate the user, it can copy the refresh token using the EOS_Auth_CopyUserAuthToken API and pass it via launch parameters to the started game client. The game client can then use the refresh token to log in the user.
	EIK_LCT_RefreshToken = 5 UMETA(DisplayName = "Refresh Token"),
	//Used by standalone applications distributed outside the supported game platforms such as Epic Games Store or Steam, and on Nintendo Switch. Login using the built-in user onboarding experience provided by the SDK, which will automatically store a persistent refresh token to enable automatic user login for consecutive application runs on the local device. Applications are expected to attempt automatic login using the EOS_LCT_PersistentAuth login method, and fall back to EOS_LCT_AccountPortal to prompt users for manual login. and to have the local Epic Online Services redistributable installed on the local system. See EOS_Platform_GetDesktopCrossplayStatus for adding a readiness check prior to calling EOS_Auth_Login.
	EIK_LCT_AccountPortal = 6 UMETA(DisplayName = "Account Portal"),
	//Login using external account provider credentials, such as PlayStation(TM)Network, Steam, and Xbox Live. This is the intended login method on PlayStation® and Xbox console devices. On Desktop and Mobile, used when launched through any of the commonly supported platform clients. If the local platform account is already linked with the user's Epic account, the login will succeed and EOS_EResult::EOS_Success is returned. When the local platform account has not been linked with an Epic account yet, EOS_EResult::EOS_InvalidUser is returned and the EOS_ContinuanceToken will be set in the EOS_Auth_LoginCallbackInfo data. If EOS_EResult::EOS_InvalidUser is returned, the application should proceed to call the EOS_Auth_LinkAccount API with the EOS_ContinuanceToken to continue with the external account login and to link the external account at the end of the login flow. 1. Game calls EOS_Auth_Login with the EOS_LCT_ExternalAuth credential type. 2. EOS_Auth_Login returns EOS_EResult::EOS_InvalidUser with a non-null EOS_ContinuanceToken in the EOS_Auth_LoginCallbackInfo data. 3. Game calls EOS_Auth_LinkAccount with the EOS_ContinuanceToken to initiate the login flow for linking the platform account with the user's Epic account. 4. The user is taken automatically to the Epic accounts user onboarding flow managed by the SDK. 5. Once the user completes the login, cancels it or if the login flow times out, EOS_Auth_LinkAccount invokes the completion callback to the caller. - If the user was logged in successfully, EOS_EResult::EOS_Success is returned in the EOS_Auth_LoginCallbackInfo. Otherwise, an error result code is returned accordingly. and to have the local Epic Online Services redistributable installed on the local system. See EOS_Platform_GetDesktopCrossplayStatus for adding a readiness check prior to calling EOS_Auth_Login.
	EIK_LCT_ExternalAuth = 7 UMETA(DisplayName = "External Auth"),
};

UENUM(BlueprintType)
enum EEIK_EAuthScopeFlags
{
	//Flags that describe user permissions
	EIK_AS_NoFlags = 0x0,
	//Permissions to see your account ID, display name, and language
	EIK_AS_BasicProfile = 0x1,
	//Permissions to see a list of your friends who use this application
	EIK_AS_FriendsList = 0x2,
	//Permissions to set your online presence and see presence of your friends
	EIK_AS_Presence = 0x4,
	//Permissions to manage the Epic friends list. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures.
	EIK_AS_FriendsManagement = 0x8,
	//Permissions to see email in the response when fetching information for a user. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures.
	EIK_AS_Email = 0x10,
	//Permissions to see your country
	EIK_AS_Country = 0x20,
};

USTRUCT(BlueprintType)
struct FEIK_Auth_Credentials
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString Token;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	TEnumAsByte<EEIK_ELoginCredentialType> Type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	TEnumAsByte<EEIK_EExternalCredentialType> ExternalType;

	FEIK_Auth_Credentials()
	{
		Id = "";
		Token = "";
		Type = EIK_LCT_Password;
		ExternalType = EIK_ECT_EPIC;
	}
	EOS_Auth_Credentials GetValueAsEosType()
	{
		EOS_Auth_Credentials Credentials;
		Credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
		if(Id.IsEmpty())
		{
			Credentials.Id = nullptr;
		}
		else
		{
			Credentials.Id = TCHAR_TO_ANSI(*Id);
		}
		if(Token.IsEmpty())
		{
			Credentials.Token = nullptr;
		}
		else
		{
			Credentials.Token = TCHAR_TO_ANSI(*Token);
		}
		Credentials.Type = static_cast<EOS_ELoginCredentialType>(Type.GetValue());
		Credentials.ExternalType = static_cast<EOS_EExternalCredentialType>(ExternalType.GetValue());
		return Credentials;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Ecom_CatalogItemId 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString Value;
	
	EOS_Ecom_CatalogItemId Ref;

	FEIK_Ecom_CatalogItemId(): Ref(nullptr)
	{
	}
	FEIK_Ecom_CatalogItemId(EOS_Ecom_CatalogItemId InCatalogItemId)
	{
		Ref = InCatalogItemId;
		Value = UTF8_TO_TCHAR(InCatalogItemId);
	}
};


USTRUCT(BlueprintType)
struct FEIK_Ecom_EntitlementName 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString Value;
	EOS_Ecom_EntitlementName Ref;

	FEIK_Ecom_EntitlementName(): Ref(nullptr)
	{
	}

	FEIK_Ecom_EntitlementName(EOS_Ecom_EntitlementName InEntitlementName)
	{
		Ref = InEntitlementName;
		Value = UTF8_TO_TCHAR(InEntitlementName);
	}
};

UENUM(BlueprintType)
enum EEIK_EEcomItemType
{
	//This entitlement is intended to persist.
	EIK_EIT_Durable = 0 UMETA(DisplayName = "Durable"),
	//This entitlement is intended to be transient and redeemed.
	EIK_EIT_Consumable = 1 UMETA(DisplayName = "Consumable"),
	//This entitlement has a type that is not currently intended for an in-game store.
	EOS_EIT_Other = 2 UMETA(DisplayName = "Other"),
	
};


USTRUCT(BlueprintType)
struct FEIK_Ecom_CatalogItem
{
	GENERATED_BODY()

	//Product namespace in which this item exists
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString CatalogNamespace;

	//The ID of this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_CatalogItemId Id;

	//The entitlement name associated with this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_EntitlementName EntitlementName;

	//Localized UTF-8 title of this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString TitleText;

	//Localized UTF-8 description of this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString DescriptionText;

	//Localized UTF-8 long description of this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString LongDescriptionText;

	//Localized UTF-8 technical details of this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString TechnicalDetailsText;

	//Localized UTF-8 developer of this item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString DeveloperText;

	//The type of item as defined in the catalog
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TEnumAsByte<EEIK_EEcomItemType> ItemType;

	//If not -1 then this is the POSIX timestamp that the entitlement will end
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int64 EntitlementEndTimestamp;
	
	EOS_Ecom_CatalogItem Ref;

	FEIK_Ecom_CatalogItem(): Ref()
	{
		CatalogNamespace = "";
		Id = FEIK_Ecom_CatalogItemId();
		EntitlementName = FEIK_Ecom_EntitlementName();
		TitleText = "";
		DescriptionText = "";
		LongDescriptionText = "";
		TechnicalDetailsText = "";
		DeveloperText = "";
		ItemType = EIK_EIT_Durable;
		EntitlementEndTimestamp = -1;
	}

	FEIK_Ecom_CatalogItem(EOS_Ecom_CatalogItem InCatalogItem)
	{
		Ref = InCatalogItem;
		CatalogNamespace = UTF8_TO_TCHAR(InCatalogItem.CatalogNamespace);
		Id = InCatalogItem.Id;
		EntitlementName = InCatalogItem.EntitlementName;
		TitleText = FString(UTF8_TO_TCHAR(InCatalogItem.TitleText));
		DescriptionText = FString(UTF8_TO_TCHAR(InCatalogItem.DescriptionText));
		LongDescriptionText = FString(UTF8_TO_TCHAR(InCatalogItem.LongDescriptionText));
		TechnicalDetailsText = FString(UTF8_TO_TCHAR(InCatalogItem.TechnicalDetailsText));
		DeveloperText = FString(UTF8_TO_TCHAR(InCatalogItem.DeveloperText));
		ItemType = static_cast<EEIK_EEcomItemType>(InCatalogItem.ItemType);
		EntitlementEndTimestamp = InCatalogItem.EntitlementEndTimestamp;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Ecom_CatalogOfferId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString OfferId;

	EOS_Ecom_CatalogOfferId CatalogOfferId;
	FEIK_Ecom_CatalogOfferId(): CatalogOfferId(nullptr)
	{
		OfferId = "";
	}

	FEIK_Ecom_CatalogOfferId(EOS_Ecom_CatalogOfferId InCatalogOfferId)
	{
		CatalogOfferId = InCatalogOfferId;
		OfferId = FString(UTF8_TO_TCHAR(InCatalogOfferId));
	}
};


USTRUCT(BlueprintType)
struct FEIK_Ecom_CatalogOffer
{
	GENERATED_BODY()

	//The index of this offer as it exists on the server. This is useful for understanding pagination data.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 ServerIndex;

	//Product namespace in which this offer exists
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString CatalogNamespace;

	//The ID of this offer
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_CatalogOfferId  Id;

	//Localized UTF-8 title of this offer
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString TitleText;

	//Localized UTF-8 description of this offer
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString DescriptionText;

	//Localized UTF-8 long description of this offer
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString LongDescriptionText;

	//The Currency Code for this offer
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString CurrencyCode;

	//If this value is EOS_Success then OriginalPrice, CurrentPrice, and DiscountPercentage contain valid data. Otherwise this value represents the error that occurred on the price query.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TEnumAsByte<EEIK_Result> PriceResult;

	//A value from 0 to 100 define the percentage of the OrignalPrice that the CurrentPrice represents
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 DiscountPercentage;

	//Contains the POSIX timestamp that the offer expires or -1 if it does not expire
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int64 ExpirationTimestamp;

	//The maximum number of times that the offer can be purchased. A negative value implies there is no limit.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 PurchaseLimit;

	//True if the user can purchase this offer.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	bool bAvailableForPurchase;

	//The original price of this offer as a 64-bit number.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int64 OriginalPrice64;

	//The current price including discounts of this offer as a 64-bit number.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int64 CurrentPrice64;

	//The decimal point for the provided price. For example, DecimalPoint '2' and CurrentPrice64 '12345' would be '123.45'.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 DecimalPoint;

	//Timestamp indicating when the time when the offer was released. Can be ignored if set to -1.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int64 ReleaseDateTimestamp;

	//Timestamp indicating the effective date of the offer. Can be ignored if set to -1.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int64 EffectiveDateTimestamp;

	EOS_Ecom_CatalogOffer Ref;

	FEIK_Ecom_CatalogOffer(): Ref()
	{
		ServerIndex = 0;
		CatalogNamespace = "";
		Id = FEIK_Ecom_CatalogOfferId();
		TitleText = "";
		DescriptionText = "";
		LongDescriptionText = "";
		CurrencyCode = "";
		PriceResult = EEIK_Result::EOS_Canceled;
		DiscountPercentage = 0;
		ExpirationTimestamp = -1;
		PurchaseLimit = 0;
		bAvailableForPurchase = false;
		OriginalPrice64 = 0;
		CurrentPrice64 = 0;
		DecimalPoint = 0;
		ReleaseDateTimestamp = -1;
		EffectiveDateTimestamp = -1;
	}

	FEIK_Ecom_CatalogOffer(EOS_Ecom_CatalogOffer InCatalogOffer)
	{
		Ref = InCatalogOffer;
		ServerIndex = InCatalogOffer.ServerIndex;
		CatalogNamespace = FString(UTF8_TO_TCHAR(InCatalogOffer.CatalogNamespace));
		Id = InCatalogOffer.Id;
		TitleText = FString(UTF8_TO_TCHAR(InCatalogOffer.TitleText));
		DescriptionText = FString(UTF8_TO_TCHAR(InCatalogOffer.DescriptionText));
		LongDescriptionText = FString(UTF8_TO_TCHAR(InCatalogOffer.LongDescriptionText));
		CurrencyCode = FString(UTF8_TO_TCHAR(InCatalogOffer.CurrencyCode));
		PriceResult = static_cast<EEIK_Result>(InCatalogOffer.PriceResult);
		DiscountPercentage = InCatalogOffer.DiscountPercentage;
		ExpirationTimestamp = InCatalogOffer.ExpirationTimestamp;
		PurchaseLimit = InCatalogOffer.PurchaseLimit;
		bAvailableForPurchase = InCatalogOffer.bAvailableForPurchase == EOS_TRUE;
		OriginalPrice64 = InCatalogOffer.OriginalPrice64;
		CurrentPrice64 = InCatalogOffer.CurrentPrice64;
		DecimalPoint = InCatalogOffer.DecimalPoint;
		ReleaseDateTimestamp = InCatalogOffer.ReleaseDateTimestamp;
		EffectiveDateTimestamp = InCatalogOffer.EffectiveDateTimestamp;
	}
	EOS_Ecom_CatalogOffer GetValueAsEosType()
	{
		return Ref;
	}
};


USTRUCT(BlueprintType)
struct FEIK_Ecom_CatalogRelease
{
	GENERATED_BODY()

	//The number of APP IDs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 CompatibleAppIdCount;

	//A list of compatible APP IDs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TArray<FString> CompatibleAppIds;

	//The number of platforms
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 CompatiblePlatformCount;

	//A list of compatible Platforms
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TArray<FString> CompatiblePlatforms;
	
	//Release note for compatible versions
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString ReleaseNote;

	EOS_Ecom_CatalogRelease Ref;

	FEIK_Ecom_CatalogRelease(): Ref()
	{
		CompatibleAppIdCount = 0;
		CompatibleAppIds = TArray<FString>();
		CompatiblePlatformCount = 0;
		CompatiblePlatforms = TArray<FString>();
		ReleaseNote = "";
	}

	FEIK_Ecom_CatalogRelease(EOS_Ecom_CatalogRelease InCatalogRelease)
	{
		Ref = InCatalogRelease;
		CompatibleAppIdCount = InCatalogRelease.CompatibleAppIdCount;
		for (int32 i = 0; i < CompatibleAppIdCount; i++)
		{
			CompatibleAppIds.Add(FString(UTF8_TO_TCHAR(InCatalogRelease.CompatibleAppIds[i])));
		}
		CompatiblePlatformCount = InCatalogRelease.CompatiblePlatformCount;
		for (int32 i = 0; i < CompatiblePlatformCount; i++)
		{
			CompatiblePlatforms.Add(FString(UTF8_TO_TCHAR(InCatalogRelease.CompatiblePlatforms[i])));
		}
		ReleaseNote = FString(UTF8_TO_TCHAR(InCatalogRelease.ReleaseNote));
	}
};

USTRUCT(BlueprintType)
struct FEIK_Ecom_CheckoutEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_CatalogItemId OfferId;

	EOS_Ecom_CheckoutEntry Ref;
	FEIK_Ecom_CheckoutEntry(): Ref()
	{
	}

	FEIK_Ecom_CheckoutEntry(EOS_Ecom_CheckoutEntry InCheckoutEntry)
	{
		Ref = InCheckoutEntry;
		OfferId = InCheckoutEntry.OfferId;
	}
	EOS_Ecom_CheckoutEntry EOS_Ecom_CheckoutEntry_FromStruct()
	{
		EOS_Ecom_CheckoutEntry CheckoutEntry;
		CheckoutEntry.ApiVersion = EOS_ECOM_CHECKOUTENTRY_API_LATEST;
		CheckoutEntry.OfferId = OfferId.Ref;
		return CheckoutEntry;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Ecom_EntitlementId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString Value;

	EOS_Ecom_EntitlementId Ref;

	FEIK_Ecom_EntitlementId(): Value(), Ref(nullptr)
	{
	}

	FEIK_Ecom_EntitlementId(EOS_Ecom_EntitlementId InEntitlementId)
	{
		Ref = InEntitlementId;
		Value = UTF8_TO_TCHAR(InEntitlementId);
	}
};

//Contains information about a single entitlement associated with an account. Instances of this structure are created by EOS_Ecom_CopyEntitlementByIndex, EOS_Ecom_CopyEntitlementByNameAndIndex, or EOS_Ecom_CopyEntitlementById. They must be passed to EOS_Ecom_Entitlement_Release.
USTRUCT(BlueprintType)
struct FEIK_Ecom_Entitlement
{
	GENERATED_BODY()

	//Name of the entitlement
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_EntitlementName EntitlementName;

	//ID of the entitlement owned by an account
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_EntitlementId EntitlementId;

	//ID of the item associated with the offer which granted this entitlement
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_CatalogItemId CatalogItemId;

	//If queried using pagination then ServerIndex represents the index of the entitlement as it exists on the server. If not queried using pagination then ServerIndex will be -1.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 ServerIndex;

	//If true then the catalog has this entitlement marked as redeemed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	bool bRedeemed;

	//If not -1 then this is a POSIX timestamp that this entitlement will end
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int64 EndTimestamp;

	EOS_Ecom_Entitlement Ref;
	FEIK_Ecom_Entitlement(): ServerIndex(0), bRedeemed(false), EndTimestamp(-1), Ref()
	{
	}

	FEIK_Ecom_Entitlement(EOS_Ecom_Entitlement InEntitlement)
	{
		Ref = InEntitlement;
		EntitlementName = InEntitlement.EntitlementName;
		EntitlementId = InEntitlement.EntitlementId;
		CatalogItemId = InEntitlement.CatalogItemId;
		ServerIndex = InEntitlement.ServerIndex;
		bRedeemed = InEntitlement.bRedeemed == EOS_TRUE;
		EndTimestamp = InEntitlement.EndTimestamp;
	}
};


/*
 *Contains information about a key image used by the catalog.
 *Instances of this structure are created by EOS_Ecom_CopyItemImageInfoByIndex.
 *They must be passed to EOS_Ecom_KeyImageInfo_Release.
 *A Key Image is defined within Dev Portal and is associated with a Catalog Item.
 *A Key Image is intended to be used to provide imagery for an in-game store.
*/
USTRUCT(BlueprintType)
struct FEIK_Ecom_KeyImageInfo
{
	GENERATED_BODY()

	//Describes the usage of the image (ex: home_thumbnail)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString Type;

	//The URL of the image
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString Url;

	//The expected width in pixels of the image
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 Width;

	//The expected height in pixels of the image
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 Height;

	EOS_Ecom_KeyImageInfo Ref;

	FEIK_Ecom_KeyImageInfo(): Width(0), Height(0), Ref()
	{
		Type = "";
	}

	FEIK_Ecom_KeyImageInfo(EOS_Ecom_KeyImageInfo InKeyImageInfo)
	{
		Ref = InKeyImageInfo;
		Type = FString(UTF8_TO_TCHAR(InKeyImageInfo.Type));
		Url = FString(UTF8_TO_TCHAR(InKeyImageInfo.Url));
		Width = InKeyImageInfo.Width;
		Height = InKeyImageInfo.Height;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Ecom_HTransaction
{
	GENERATED_BODY()

	EOS_Ecom_HTransaction Ref;
	
	FEIK_Ecom_HTransaction(): Ref(nullptr)
	{
	}

	FEIK_Ecom_HTransaction(EOS_Ecom_HTransaction InHTransaction)
	{
		Ref = InHTransaction;
	}
	EOS_Ecom_HTransaction GetReference()
	{
		return Ref;
	}
};

UENUM(BlueprintType)
enum EEIK_EOwnershipStatus
{
	EIK_OS_NotOwned = 0 UMETA(DisplayName = "Not Owned"),
	EIK_OS_Owned = 1 UMETA(DisplayName = "Owned"),
};

//Contains information about a single item ownership associated with an account. This structure is returned as part of the EOS_Ecom_QueryOwnershipCallbackInfo structure.
USTRUCT(BlueprintType)
struct FEIK_Ecom_ItemOwnership
{
	GENERATED_BODY()

	//ID of the catalog item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_CatalogItemId Id;

	//Is this catalog item owned by the local user
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TEnumAsByte<EEIK_EOwnershipStatus> OwnershipStatus;


	EOS_Ecom_ItemOwnership Ref;
	FEIK_Ecom_ItemOwnership(): Ref()
	{
		Id = FEIK_Ecom_CatalogItemId();
		OwnershipStatus = EIK_OS_NotOwned;
	}

	FEIK_Ecom_ItemOwnership(EOS_Ecom_ItemOwnership InItemOwnership)
	{
		Ref = InItemOwnership;
		Id = InItemOwnership.Id;
		OwnershipStatus = static_cast<EEIK_EOwnershipStatus>(InItemOwnership.OwnershipStatus);
	}
};

USTRUCT(BlueprintType)
struct FEIK_Ecom_SandboxId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString Value;

	EOS_Ecom_SandboxId Ref;

	FEIK_Ecom_SandboxId(): Value(), Ref(nullptr)
	{
	}
	FEIK_Ecom_SandboxId(EOS_Ecom_SandboxId InSandboxId)
	{
		Ref = InSandboxId;
		Value = UTF8_TO_TCHAR(InSandboxId);
	}
	EOS_Ecom_SandboxId GetValueAsEosType()
	{
		return Ref;
	}
};


USTRUCT(BlueprintType)
struct FEIK_Ecom_SandboxIdItemOwnership
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_SandboxId SandboxId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TArray<FEIK_Ecom_CatalogItemId> OwnedCatalogItemIds;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 OwnedCatalogItemIdsCount;

	FEIK_Ecom_SandboxIdItemOwnership(): OwnedCatalogItemIdsCount(0)
	{
		SandboxId = FEIK_Ecom_SandboxId();
	}

	FEIK_Ecom_SandboxIdItemOwnership(EOS_Ecom_SandboxIdItemOwnership InSandboxIdItemOwnership)
	{
		SandboxId = InSandboxIdItemOwnership.SandboxId;
		OwnedCatalogItemIdsCount = InSandboxIdItemOwnership.OwnedCatalogItemIdsCount;
		for (int32 i = 0; i < OwnedCatalogItemIdsCount; i++)
		{
			OwnedCatalogItemIds.Add(InSandboxIdItemOwnership.OwnedCatalogItemIds[i]);
		}
	}
};


UENUM(BlueprintType)
enum EEIK_EFriendsStatus
{
	//The two accounts have no friendship status.
	EIK_FS_NotFriends = 0 UMETA(DisplayName = "Not Friends"),
	//The local account has sent a friend invite to the other account. NOTE: EOS_FS_InviteSent is not returned by EOS_Friends_GetStatus or in EOS_Friends_AddNotifyFriendsUpdate callbacks unless the local account was logged in with the EOS_AS_FriendsManagement authentication scope. Friend invites are managed automatically by the Social Overlay.
	EIK_FS_InviteSent = 1 UMETA(DisplayName = "Invite Sent"),
	//The other account has sent a friend invite to the local account. NOTE: EOS_FS_InviteReceived is not returned by EOS_Friends_GetStatus or in EOS_Friends_AddNotifyFriendsUpdate callbacks unless the local account was logged in with the EOS_AS_FriendsManagement authentication scope. Friend invites are managed automatically by the Social Overlay.
	EIK_FS_InviteReceived = 2 UMETA(DisplayName = "Invite Received"),
	//The accounts have accepted friendship.
	EIK_FS_Friends = 3 UMETA(DisplayName = "Friends"),
};

UENUM(BlueprintType)
enum EEIK_ELeaderboardAggregation
{
	//Minimum
	EIK_LA_Min = 0 UMETA(DisplayName = "Min"),
	//Maximum
	EIK_LA_Max = 1 UMETA(DisplayName = "Max"),
	//Sum
	EIK_LA_Sum = 2 UMETA(DisplayName = "Sum"),
	//Latest
	EIK_LA_Latest = 3 UMETA(DisplayName = "Latest"),
};

USTRUCT(BlueprintType)
struct FEIK_Leaderboards_Definition
{
	GENERATED_BODY()

	//Unique ID to identify leaderboard.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FString LeaderboardId;

	//Name of stat used to rank leaderboard.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FString StatName;
	
	//Aggregation used to sort leaderboard.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	TEnumAsByte<EEIK_ELeaderboardAggregation> Aggregation;

	//The POSIX timestamp for the start time, or EOS_LEADERBOARDS_TIME_UNDEFINED.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	int64 StartTime;

	//The POSIX timestamp for the end time, or EOS_LEADERBOARDS_TIME_UNDEFINED.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	int64 EndTime;

	EOS_Leaderboards_Definition Ref;
	FEIK_Leaderboards_Definition(): LeaderboardId(), StatName(), Aggregation(EEIK_ELeaderboardAggregation::EIK_LA_Min),
	                                StartTime(0), EndTime(0), Ref()
	{
	}

	FEIK_Leaderboards_Definition(EOS_Leaderboards_Definition InLeaderboardDefinition)
	{
		Ref = InLeaderboardDefinition;
		LeaderboardId = FString(UTF8_TO_TCHAR(InLeaderboardDefinition.LeaderboardId));
		StatName = FString(UTF8_TO_TCHAR(InLeaderboardDefinition.StatName));
		Aggregation = static_cast<EEIK_ELeaderboardAggregation>(InLeaderboardDefinition.Aggregation);
		StartTime = InLeaderboardDefinition.StartTime;
		EndTime = InLeaderboardDefinition.EndTime;
	}
};


USTRUCT(BlueprintType)
struct FEIK_Leaderboards_LeaderboardRecord
{
	GENERATED_BODY()

	//The Product User ID associated with this record
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FEIK_ProductUserId UserId;

	//Sorted position on leaderboard
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	int32 Rank;

	//Leaderboard score
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	int32 Score;

	//The latest display name seen for the user since they last time logged in. This is empty if the user does not have a display name set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FString UserDisplayName;

	EOS_Leaderboards_LeaderboardRecord Ref;
	FEIK_Leaderboards_LeaderboardRecord(): UserId(), Rank(0), Score(0), UserDisplayName(), Ref()
	{
	}

	FEIK_Leaderboards_LeaderboardRecord(EOS_Leaderboards_LeaderboardRecord InLeaderboardRecord)
	{
		Ref = InLeaderboardRecord;
		UserId = InLeaderboardRecord.UserId;
		Rank = InLeaderboardRecord.Rank;
		Score = InLeaderboardRecord.Score;
		UserDisplayName = FString(UTF8_TO_TCHAR(InLeaderboardRecord.UserDisplayName));
	}
};


USTRUCT(BlueprintType)
struct FEIK_Leaderboards_LeaderboardUserScore
{
	GENERATED_BODY()

	//The Product User ID of the user who got this score
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FEIK_ProductUserId UserId;

	//Leaderboard score
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	int32 Score;

	EOS_Leaderboards_LeaderboardUserScore Ref;

	FEIK_Leaderboards_LeaderboardUserScore(): UserId(), Score(0), Ref()
	{
	}

	FEIK_Leaderboards_LeaderboardUserScore(EOS_Leaderboards_LeaderboardUserScore InLeaderboardUserScore)
	{
		Ref = InLeaderboardUserScore;
		UserId = InLeaderboardUserScore.UserId;
		Score = InLeaderboardUserScore.Score;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Leaderboards_UserScoresQueryStatInfo
{
	GENERATED_BODY()

	//The name of the stat to query.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FString StatName;

	//Aggregation used to sort the cached user scores.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	TEnumAsByte<EEIK_ELeaderboardAggregation> Aggregation;

	FEIK_Leaderboards_UserScoresQueryStatInfo(): StatName(), Aggregation(EEIK_ELeaderboardAggregation::EIK_LA_Min)
	{
	}
	EOS_Leaderboards_UserScoresQueryStatInfo GetValueAsEosType()
	{
		EOS_Leaderboards_UserScoresQueryStatInfo UserScoresQueryStatInfo;
		UserScoresQueryStatInfo.ApiVersion = EOS_LEADERBOARDS_USERSCORESQUERYSTATINFO_API_LATEST;
		UserScoresQueryStatInfo.StatName = TCHAR_TO_ANSI(*StatName);
		UserScoresQueryStatInfo.Aggregation = static_cast<EOS_ELeaderboardAggregation>(Aggregation.GetValue());
		return UserScoresQueryStatInfo;
	}
};


USTRUCT(BlueprintType)
struct FEIK_UI_EventId 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | UI Interface")
	int64 Value;
	
	EOS_UI_EventId Ref;
	
	FEIK_UI_EventId(): Ref(-1)
	{
		Value = -1;
	}
	FEIK_UI_EventId(EOS_UI_EventId InEventId)
	{
		Ref = InEventId;
		Value = InEventId;
	}
};

USTRUCT(BlueprintType)
struct FEIK_LobbyId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FString Value;

	EOS_LobbyId Ref;

	FEIK_LobbyId(): Value(), Ref(nullptr)
	{
	}
	FEIK_LobbyId(EOS_LobbyId InLobbyId)
	{
		Ref = InLobbyId;
		Value = UTF8_TO_TCHAR(InLobbyId);
	}
	EOS_LobbyId GetValueAsEosType()
	{
		return Ref;
	}
};

UENUM(BlueprintType)
enum EEIK_ELobbyMemberStatus
{
	//The user has joined the lobby
	EIK_LMS_Joined = 0 UMETA(DisplayName = "Joined"),
	//The user has explicitly left the lobby
	EIK_LMS_Left = 1 UMETA(DisplayName = "Left"),
	//The user has unexpectedly left the lobby
	EIK_LMS_Disconnected = 2 UMETA(DisplayName = "Disconnected"),
	//The user has been kicked from the lobby
	EIK_LMS_Kicked = 3 UMETA(DisplayName = "Kicked"),
	//The user has been promoted to lobby owner
	EIK_LMS_Promoted = 4 UMETA(DisplayName = "Promoted"),
	//The lobby has been closed and user has been removed
	EIK_LMS_Closed = 5 UMETA(DisplayName = "Closed"),
};

/** Advertisement properties for a single attribute associated with a lobby */
UENUM(BlueprintType)
enum EEIK_ELobbyAttributeVisibility
{
	/** Data is visible to lobby members, searchable and visible in search results. */
	EIK_LAV_Public = 0 UMETA(DisplayName = "Public"),
	/** Data is only visible to the user setting the data. Data is not visible to lobby members, not searchable, and not visible in search results. */
	EIK_LAV_Private = 1 UMETA(DisplayName = "Private"),
};

/**
 * Supported types of data that can be stored with inside an attribute (used by sessions/lobbies/etc)
 *
 * @see EOS_LobbySearch_SetParameter
 * @see EOS_SessionSearch_SetParameter
 */
UENUM(BlueprintType)
enum EEIK_EAttributeType
{
	/** Boolean value (true/false) */
	EIK_AT_BOOL = 0 UMETA(DisplayName = "Bool"),
	/** 64 bit integers */
	EIK_AT_INT64 = 1 UMETA(DisplayName = "Int64"),
	/** Double precision floating point */
	EIK_AT_DOUBLE = 2 UMETA(DisplayName = "Double"),
	/** UTF-8 string */
	EIK_AT_STRING = 3 UMETA(DisplayName = "String"),
};

/**
 * Contains information about lobby and lobby member data
 */
USTRUCT(BlueprintType)
struct FEIK_Lobby_AttributeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FString Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	int64 ValueAsInt64;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FString ValueAsString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bValueAsBool;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	float ValueAsDouble;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	TEnumAsByte<EEIK_EAttributeType> ValueType;

	EOS_Lobby_AttributeData Ref;

	FEIK_Lobby_AttributeData(): ValueAsInt64(0), bValueAsBool(false), ValueAsDouble(0), ValueType(), Ref()
	{
	}

	FEIK_Lobby_AttributeData(EOS_Lobby_AttributeData InLobbyAttributeData)
	{
		Ref = InLobbyAttributeData;
		Key = UTF8_TO_TCHAR(InLobbyAttributeData.Key);
		ValueAsInt64 = InLobbyAttributeData.Value.AsInt64;
		ValueAsString = UTF8_TO_TCHAR(InLobbyAttributeData.Value.AsUtf8);
		bValueAsBool = InLobbyAttributeData.Value.AsBool == EOS_TRUE;
		ValueAsDouble = InLobbyAttributeData.Value.AsDouble;
		ValueType = static_cast<EEIK_EAttributeType>(InLobbyAttributeData.ValueType);
	}
};


/**
 *  An attribute and its visibility setting stored with a lobby.
 *  Used to store both lobby and lobby member data
 */
USTRUCT(BlueprintType)
struct FEIK_Lobby_Attribute
{
	GENERATED_BODY()
	
	/** Key/Value pair describing the attribute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_AttributeData Data;

	/** Is this attribute public or private to the lobby and its members */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	TEnumAsByte<EEIK_ELobbyAttributeVisibility> Visibility;
	
	EOS_Lobby_Attribute Ref;
	FEIK_Lobby_Attribute(): Visibility(), Ref()
	{
	}

	FEIK_Lobby_Attribute(EOS_Lobby_Attribute InLobbyAttribute)
	{
		Ref = InLobbyAttribute;
		Data = *InLobbyAttribute.Data;
		Visibility = static_cast<EEIK_ELobbyAttributeVisibility>(InLobbyAttribute.Visibility);
	}
};

USTRUCT(BlueprintType)
struct FEIK_HLobbySearch
{
	GENERATED_BODY()

	EOS_HLobbySearch Ref;

	FEIK_HLobbySearch(): Ref(nullptr)
	{
	}
	FEIK_HLobbySearch(EOS_HLobbySearch InHLobbySearch)
	{
		Ref = InHLobbySearch;
	}
};

USTRUCT(BlueprintType)
struct FEIK_HLobbyModification
{
	GENERATED_BODY()

	EOS_HLobbyModification* Ref;
	
	FEIK_HLobbyModification(): Ref(nullptr)
	{
	}
	FEIK_HLobbyModification(EOS_HLobbyModification* InHLobbyModification)
	{
		if(InHLobbyModification)
		{
			Ref = InHLobbyModification;
		}
	}
};

USTRUCT(BlueprintType)
struct FEIK_HLobbyDetails
{
	GENERATED_BODY()

	EOS_HLobbyDetails Ref;

	FEIK_HLobbyDetails(): Ref(nullptr)
	{
	}
	FEIK_HLobbyDetails(EOS_HLobbyDetails InHLobbyDetails)
	{
		Ref = InHLobbyDetails;
	}
};


/** Permission level gets more restrictive further down */
UENUM(BlueprintType)
enum EEIK_ELobbyPermissionLevel
{
	/** Anyone can find this lobby as long as it isn't full */
	EIK_LPL_PublicAdvertised = 0 UMETA(DisplayName = "Public Advertised"),
	/** Players who have access to presence can see this lobby */
	EIK_LPL_JoinViaPresence = 1 UMETA(DisplayName = "Join Via Presence"),
	/** Only players with invites registered can see this lobby */
	EIK_LPL_InviteOnly = 2 UMETA(DisplayName = "Invite Only"),
};

/**
 * Input parameters to use with Lobby RTC Rooms.
 */
USTRUCT(BlueprintType)
struct FEIK_Lobby_LocalRTCOptions
{
	GENERATED_BODY()

	/** Flags for the local user in this room. The default is 0 if this struct is not specified. @see EOS_RTC_JoinRoomOptions::Flags */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	int32 Flags;

	/**
	* Set to EOS_TRUE to enable Manual Audio Input. If manual audio input is enabled, audio recording is not started and the audio buffers
	* must be passed manually using EOS_RTCAudio_SendAudio. The default is EOS_FALSE if this struct is not specified.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bUseManualAudioInput;
	
	/**
	* Set to EOS_TRUE to enable Manual Audio Output. If manual audio output is enabled, audio rendering is not started and the audio buffers
	* must be received with EOS_RTCAudio_AddNotifyAudioBeforeRender and rendered manually. The default is EOS_FALSE if this struct is not
	* specified.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bUseManualAudioOutput;
	
	/**
	 * Set to EOS_TRUE to start the audio input device's stream as muted when first connecting to the RTC room.
	 *
	 * It must be manually unmuted with a call to EOS_RTCAudio_UpdateSending. If manual audio output is enabled, this value is ignored.
	 * The default value is EOS_FALSE if this struct is not specified.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bLocalAudioDeviceInputStartsMuted;

	FEIK_Lobby_LocalRTCOptions(): Flags(0), bUseManualAudioInput(false), bUseManualAudioOutput(false), bLocalAudioDeviceInputStartsMuted(false)
	{
	}
	FEIK_Lobby_LocalRTCOptions(EOS_Lobby_LocalRTCOptions InLocalRTCOptions)
	{
		Flags = InLocalRTCOptions.Flags;
		bUseManualAudioInput = InLocalRTCOptions.bUseManualAudioInput == EOS_TRUE;
		bUseManualAudioOutput = InLocalRTCOptions.bUseManualAudioOutput == EOS_TRUE;
		bLocalAudioDeviceInputStartsMuted = InLocalRTCOptions.bLocalAudioDeviceInputStartsMuted == EOS_TRUE;
	}
	EOS_Lobby_LocalRTCOptions GetValueAsEosType()
	{
		EOS_Lobby_LocalRTCOptions LocalRTCOptions;
		LocalRTCOptions.ApiVersion = EOS_LOBBY_LOCALRTCOPTIONS_API_LATEST;
		LocalRTCOptions.Flags = Flags;
		LocalRTCOptions.bUseManualAudioInput = bUseManualAudioInput ? EOS_TRUE : EOS_FALSE;
		LocalRTCOptions.bUseManualAudioOutput = bUseManualAudioOutput ? EOS_TRUE : EOS_FALSE;
		LocalRTCOptions.bLocalAudioDeviceInputStartsMuted = bLocalAudioDeviceInputStartsMuted ? EOS_TRUE : EOS_FALSE;
		return LocalRTCOptions;
	}
};

USTRUCT(BlueprintType)
struct FEIK_LobbyDetailsInfo
{
	GENERATED_BODY()

	//Lobby ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_LobbyId LobbyId;

	//The Product User ID of the current owner of the lobby
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_ProductUserId LobbyOwnerUserId;

	//Permission level of the lobby
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	TEnumAsByte<EEIK_ELobbyPermissionLevel> PermissionLevel;

	//Current available space
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	int32 AvailableSlots;

	//Max allowed members in the lobby
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	int32 MaxMembers;

	//If true, users can invite others to this lobby
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bAllowInvites;

	//The main indexed parameter for this lobby, can be any string (i.e. "Region:GameMode")
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FString BucketId;

	//Is host migration allowed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bAllowHostMigration;

	//Was a Real-Time Communication (RTC) room enabled at lobby creation?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bRTCRoomEnabled;

	//Is EOS_Lobby_JoinLobbyById allowed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bAllowJoinById;

	//Does rejoining after being kicked require an invite
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bRejoinAfterKickRequiresInvite;

	//If true, this lobby will be associated with the local user's presence information.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bPresenceEnabled;

	//Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific header file. If null, the lobby will be unrestricted.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	TArray<int32> AllowedPlatformIds;

	EOS_LobbyDetails_Info Ref;

	FEIK_LobbyDetailsInfo(): LobbyId(), LobbyOwnerUserId(), PermissionLevel(), AvailableSlots(0), MaxMembers(0),
	                         bAllowInvites(false), BucketId(),
	                         bAllowHostMigration(false), bRTCRoomEnabled(false), bAllowJoinById(false),
	                         bRejoinAfterKickRequiresInvite(false), bPresenceEnabled(false), AllowedPlatformIds(), Ref()
	{
	}

	FEIK_LobbyDetailsInfo(EOS_LobbyDetails_Info InLobbyDetailsInfo)
	{
		Ref = InLobbyDetailsInfo;
		LobbyId = InLobbyDetailsInfo.LobbyId;
		LobbyOwnerUserId = InLobbyDetailsInfo.LobbyOwnerUserId;
		PermissionLevel = static_cast<EEIK_ELobbyPermissionLevel>(InLobbyDetailsInfo.PermissionLevel);
		AvailableSlots = InLobbyDetailsInfo.AvailableSlots;
		MaxMembers = InLobbyDetailsInfo.MaxMembers;
		bAllowInvites = InLobbyDetailsInfo.bAllowInvites == EOS_TRUE;
		BucketId = FString(UTF8_TO_TCHAR(InLobbyDetailsInfo.BucketId));
		bAllowHostMigration = InLobbyDetailsInfo.bAllowHostMigration == EOS_TRUE;
		bRTCRoomEnabled = InLobbyDetailsInfo.bRTCRoomEnabled == EOS_TRUE;
		bAllowJoinById = InLobbyDetailsInfo.bAllowJoinById == EOS_TRUE;
		bRejoinAfterKickRequiresInvite = InLobbyDetailsInfo.bRejoinAfterKickRequiresInvite == EOS_TRUE;
		bPresenceEnabled = InLobbyDetailsInfo.bPresenceEnabled == EOS_TRUE;
		for (int32 i = 0; i < (int32)InLobbyDetailsInfo.AllowedPlatformIdsCount; i++)
		{
			AllowedPlatformIds.Add(InLobbyDetailsInfo.AllowedPlatformIds[i]);
		}
	}
};

 

USTRUCT(BlueprintType)
struct FEIK_LobbyDetails_MemberInfo
{
	GENERATED_BODY()

	//The Product User ID of the lobby member.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_ProductUserId UserId;

	//The platform of the lobby member.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	int32 Platform;
	
	//Does this member allow crossplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bAllowsCrossplay;

	EOS_LobbyDetails_MemberInfo Ref;

	FEIK_LobbyDetails_MemberInfo(): UserId(), Platform(0), bAllowsCrossplay(false), Ref()
	{
	}

	FEIK_LobbyDetails_MemberInfo(EOS_LobbyDetails_MemberInfo InLobbyDetailsMemberInfo)
	{
		Ref = InLobbyDetailsMemberInfo;
		UserId = InLobbyDetailsMemberInfo.UserId;
		Platform = InLobbyDetailsMemberInfo.Platform;
		bAllowsCrossplay = InLobbyDetailsMemberInfo.bAllowsCrossplay == EOS_TRUE;
	}
};

/**
 * All comparison operators associated with parameters in a search query
 *
 * @see EOS_LobbySearch_SetParameter
 * @see EOS_SessionSearch_SetParameter
 */ 
UENUM(BlueprintType)
enum EEIK_EComparisonOp
{
	/** Value must equal the one stored on the lobby/session */
	EIK_CO_EQUAL = 0,
	/** Value must not equal the one stored on the lobby/session */
	EIK_CO_NOTEQUAL = 1,
	/** Value must be strictly greater than the one stored on the lobby/session */
	EIK_CO_GREATERTHAN = 2,
	/** Value must be greater than or equal to the one stored on the lobby/session */
	EIK_CO_GREATERTHANOREQUAL = 3,
	/** Value must be strictly less than the one stored on the lobby/session */
	EIK_CO_LESSTHAN = 4,
	/** Value must be less than or equal to the one stored on the lobby/session */
	EIK_CO_LESSTHANOREQUAL = 5,
	/** Prefer values nearest the one specified ie. abs(SearchValue-SessionValue) closest to 0 */
	EIK_CO_DISTANCE = 6,
	/** Value stored on the lobby/session may be any from a specified list */
	EIK_CO_ANYOF = 7,
	/** Value stored on the lobby/session may NOT be any from a specified list */
	EIK_CO_NOTANYOF = 8,
	/** This one value is a part of a collection */
	EIK_CO_ONEOF = 9,
	/** This one value is NOT part of a collection */
	EIK_CO_NOTONEOF = 10,
	/** This value is a CASE SENSITIVE substring of an attribute stored on the lobby/session */
	EIK_CO_CONTAINS = 11
};


USTRUCT(BlueprintType)
struct FEIK_PlayerDataStorage_FileMetadata
{
	GENERATED_BODY()

	//The total size of the file in bytes (Includes file header in addition to file contents)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface")
	int32 FileSizeBytes;

	//The MD5 Hash of the entire file (including additional file header), in hex digits
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface")
	FString MD5Hash;

	//The file's name
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface")
	FString Filename;

	//The POSIX timestamp when the file was saved last time or EOS_PLAYERDATASTORAGE_TIME_UNDEFINED if the time is undefined. It will be undefined after a file is written and uploaded at first before a query operation is completed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface")
	int64 LastModifiedTime;

	//The size of data (payload) in file in unencrypted (original) form.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface")
	int32 UnencryptedDataSizeBytes;

	EOS_PlayerDataStorage_FileMetadata Ref;

	FEIK_PlayerDataStorage_FileMetadata(): FileSizeBytes(0), MD5Hash(), Filename(), LastModifiedTime(0),
	                                       UnencryptedDataSizeBytes(0), Ref()
	{
	}

	FEIK_PlayerDataStorage_FileMetadata(EOS_PlayerDataStorage_FileMetadata InFileMetadata)
	{
		Ref = InFileMetadata;
		FileSizeBytes = InFileMetadata.FileSizeBytes;
		MD5Hash = FString(UTF8_TO_TCHAR(InFileMetadata.MD5Hash));
		Filename = FString(UTF8_TO_TCHAR(InFileMetadata.Filename));
		LastModifiedTime = InFileMetadata.LastModifiedTime;
		UnencryptedDataSizeBytes = InFileMetadata.UnencryptedDataSizeBytes;
	}
};


USTRUCT(BlueprintType)
struct FEIK_HPlayerDataStorageFileTransferRequest 
{
	GENERATED_BODY()

	EOS_HPlayerDataStorageFileTransferRequest Ref;

	FEIK_HPlayerDataStorageFileTransferRequest(): Ref(nullptr)
	{
	}
	FEIK_HPlayerDataStorageFileTransferRequest(EOS_HPlayerDataStorageFileTransferRequest InHPlayerDataStorageFileTransferRequest)
	{
		Ref = InHPlayerDataStorageFileTransferRequest;
	}
	
};

/**
 * Presence Status states of a user
 *
 * @see EOS_Presence_CopyPresence
 * @see EOS_PresenceModification_SetStatus
 */
UENUM(BlueprintType)
enum EEIK_Presence_EStatus
{
	/** The status of the account is offline or not known */
	EIK_PS_Offline = 0 UMETA(DisplayName = "Offline"),
	/** The status of the account is online */
	EIK_PS_Online = 1 UMETA(DisplayName = "Online"),
	/** The status of the account is away */
	EIK_PS_Away = 2 UMETA(DisplayName = "Away"),
	/** The status of the account is away, and has been away for a while */
	EIK_PS_ExtendedAway = 3 UMETA(DisplayName = "Extended Away"),
	/** The status of the account is do-not-disturb */
	EIK_PS_DoNotDisturb = 4 UMETA(DisplayName = "Do Not Disturb"),
};


/**
 * An individual presence data record that belongs to a EOS_Presence_Info object. This object is released when its parent EOS_Presence_Info object is released.
 *
 * @see EOS_Presence_Info
 */
USTRUCT(BlueprintType)
struct FEIK_Presence_DataRecord
{
	GENERATED_BODY()
	
	/** The name of this data */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString Key;

	/** The value of this data */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString Value;
	
	EOS_Presence_DataRecord Ref;
	FEIK_Presence_DataRecord(): Key(), Value(), Ref()
	{
	}

	FEIK_Presence_DataRecord(EOS_Presence_DataRecord InPresenceDataRecord)
	{
		Ref = InPresenceDataRecord;
		Key = UTF8_TO_TCHAR(InPresenceDataRecord.Key);
		Value = UTF8_TO_TCHAR(InPresenceDataRecord.Value);
	}
};

/**
 * All the known presence information for a specific user. This object must be released by calling EOS_Presence_Info_Release.
 *
 * @see EOS_Presence_CopyPresence
 * @see EOS_Presence_Info_Release
 */
USTRUCT(BlueprintType)
struct FEIK_Presence_Info
{
	GENERATED_BODY()

	/** The status of the user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	TEnumAsByte<EEIK_Presence_EStatus> Status;

	/** The Epic Account ID of the user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FEIK_EpicAccountId UserId;

	/** The product ID that the user is logged in from */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString ProductId;

	/** The version of the product the user is logged in from */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString ProductVersion;

	/** The platform of that the user is logged in from */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString Platform;

	/** The rich-text of the user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString RichText;

	/** The count of records available */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	int32 RecordsCount;

	/** The first data record, or NULL if RecordsCount is not at least 1 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FEIK_Presence_DataRecord Record;
	
	/** The user-facing name for the product the user is logged in from */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString ProductName;

	/** The integrated platform that the user is logged in with */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString IntegratedPlatform;

	EOS_Presence_Info Ref;

	FEIK_Presence_Info(): Status(), UserId(), ProductId(), ProductVersion(), Platform(), RichText(), RecordsCount(0),
	                      Record(), ProductName(), IntegratedPlatform(), Ref()
	{
	}

	FEIK_Presence_Info(EOS_Presence_Info InPresenceInfo)
	{
		Ref = InPresenceInfo;
		Status = static_cast<EEIK_Presence_EStatus>(InPresenceInfo.Status);
		UserId = InPresenceInfo.UserId;
		ProductId = FString(UTF8_TO_TCHAR(InPresenceInfo.ProductId));
		ProductVersion = FString(UTF8_TO_TCHAR(InPresenceInfo.ProductVersion));
		Platform = FString(UTF8_TO_TCHAR(InPresenceInfo.Platform));
		RichText = FString(UTF8_TO_TCHAR(InPresenceInfo.RichText));
		RecordsCount = InPresenceInfo.RecordsCount;
		if (RecordsCount > 0)
		{
			Record = InPresenceInfo.Records[0];
		}
		ProductName = FString(UTF8_TO_TCHAR(InPresenceInfo.ProductName));
		IntegratedPlatform = FString(UTF8_TO_TCHAR(InPresenceInfo.IntegratedPlatform));
	}
};

USTRUCT(BlueprintType)
struct FEIK_HPresenceModification
{
	GENERATED_BODY()

	EOS_HPresenceModification Ref;

	FEIK_HPresenceModification(): Ref(nullptr)
	{
	}
	FEIK_HPresenceModification(EOS_HPresenceModification InHPresenceModification)
	{
		Ref = InHPresenceModification;
	}
	
};

USTRUCT(BlueprintType)
struct FEIK_PresenceModification_DataRecordId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Presence Interface")
	FString Key;

	EOS_PresenceModification_DataRecordId Ref;

	FEIK_PresenceModification_DataRecordId(): Key(), Ref()
	{
	}

	FEIK_PresenceModification_DataRecordId(EOS_PresenceModification_DataRecordId InPresenceModificationDataRecordId)
	{
		Ref = InPresenceModificationDataRecordId;
		Key = FString(UTF8_TO_TCHAR(InPresenceModificationDataRecordId.Key));
	}
};

/**
 * This struct is used to get information about a specific participant metadata item.
 */
USTRUCT(BlueprintType)
struct FEIK_RTC_ParticipantMetadata
{
	GENERATED_BODY()

	/** The unique key of this metadata item. The max size of the string is EOS_RTC_PARTICIPANTMETADATA_KEY_MAXCHARCOUNT. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	FString Key;

	/** The value of this metadata item. The max size of the string is EOS_RTC_PARTICIPANTMETADATA_VALUE_MAXCHARCOUNT. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	FString Value;

	EOS_RTC_ParticipantMetadata Ref;

	FEIK_RTC_ParticipantMetadata(): Key(), Value(), Ref()
	{
	}
	FEIK_RTC_ParticipantMetadata(EOS_RTC_ParticipantMetadata InParticipantMetadata)
	{
		Ref = InParticipantMetadata;
		Key = FString(UTF8_TO_TCHAR(InParticipantMetadata.Key));
		Value = FString(UTF8_TO_TCHAR(InParticipantMetadata.Value));
	}
};

UENUM(BlueprintType)
enum EEIK_ERTCParticipantStatus
{
	//Participant joined the room
	EIK_RTCPS_Joined = 0 UMETA(DisplayName = "Joined"),
	//Participant left the room
	EIK_RTCPS_Left = 1 UMETA(DisplayName = "Left"),
};

USTRUCT(BlueprintType)
struct FEIK_Sanctions_PlayerSanction
{
	GENERATED_BODY()

	//The POSIX timestamp when the sanction was placed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface")
	int64 TimePlaced;

	//The POSIX timestamp when the sanction will expire. If the sanction is permanent, this will be 0.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface")
	int64 TimeExpires;

	//The action associated with this sanction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface")
	FString Action;

	//A unique identifier for this specific sanction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface")
	FString ReferenceId;

	FEIK_Sanctions_PlayerSanction(): TimePlaced(0), TimeExpires(0), Action(), ReferenceId()
	{
	}
	FEIK_Sanctions_PlayerSanction(EOS_Sanctions_PlayerSanction InPlayerSanction)
	{
		TimePlaced = InPlayerSanction.TimePlaced;
		TimeExpires = InPlayerSanction.TimeExpires;
		Action = FString(UTF8_TO_TCHAR(InPlayerSanction.Action));
		ReferenceId = FString(UTF8_TO_TCHAR(InPlayerSanction.ReferenceId));
	}
};


UENUM(BlueprintType)
enum EEIK_ESanctionAppealReason
{
	//Not used
	EIK_SAR_Invalid = 0 UMETA(DisplayName = "Invalid"),
	//Incorrectly placed sanction
	EIK_SAR_IncorrectSanction = 1 UMETA(DisplayName = "Incorrect Sanction"),
	//The account was compromised, typically this means stolen
	EIK_SAR_CompromisedAccount = 2 UMETA(DisplayName = "Compromised Account"),
	//The punishment is considered too severe by the user
	EIK_SAR_UnfairPunishment = 3 UMETA(DisplayName = "Unfair Punishment"),
	//The user admits to rulebreaking, but still appeals for forgiveness
	EIK_SAR_AppealForForgiveness = 4 UMETA(DisplayName = "Appeal For Forgiveness"),
};

/** All possible states of an existing named session */
UENUM(BlueprintType)
enum EEIK_EOnlineSessionState
{
	/** An online session has not been created yet */
	EIK_OSS_NoSession = 0,
	/** An online session is in the process of being created */
	EIK_OSS_Creating = 1,
	/** Session has been created but the session hasn't started (pre match lobby) */
	EIK_OSS_Pending = 2,
	/** Session has been asked to start (may take time due to communication with backend) */
	EIK_OSS_Starting = 3,
	/** The current session has started. Sessions with join in progress disabled are no longer joinable */
	EIK_OSS_InProgress = 4,
	/** The session is still valid, but the session is no longer being played (post match lobby) */
	EIK_OSS_Ending = 5,
	/** The session is closed and any stats committed */
	EIK_OSS_Ended = 6,
	/** The session is being destroyed */
	EIK_OSS_Destroying = 7
};

UENUM(BlueprintType)
enum EEIK_EOnlineSessionPermissionLevel
{
	/** Anyone can find this session as long as it isn't full */
	EIK_OSPF_PublicAdvertised = 0 UMETA(DisplayName = "Public Advertised"),
	/** Players who have access to presence can see this session */
	EIK_OSPF_JoinViaPresence = 1 UMETA(DisplayName = "Join Via Presence"),
	/** Only players with invites registered can see this session */
	EIK_OSPF_InviteOnly = 2 UMETA(DisplayName = "Invite Only"),
};

USTRUCT(BlueprintType)
struct FEIK_SessionDetails_Settings
{
	GENERATED_BODY()

	/** The main indexed parameter for this session, can be any string (i.e. "Region:GameMode") */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FString BucketId;

	/** Number of total players allowed in the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	int32 NumPublicConnections;

	/** Are players allowed to join the session while it is in the "in progress" state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	bool bAllowJoinInProgress;

	/** Permission level describing allowed access to the session when joining or searching for the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	TEnumAsByte<EEIK_EOnlineSessionPermissionLevel> PermissionLevel;

	/** Are players allowed to send invites for the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	bool bAllowInvites;

	/** Are sanctioned players allowed to join - sanctioned players will be rejected if set to true */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	bool bSanctionsEnabled;

	/** 
	 * Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are
	 * found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific
	 * header file. If null, the session will be unrestricted.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	TArray<int32> AllowedPlatformIds;

	FEIK_SessionDetails_Settings()
	{
		BucketId = "";
		NumPublicConnections = 0;
		bAllowJoinInProgress = false;
		PermissionLevel = EEIK_EOnlineSessionPermissionLevel::EIK_OSPF_PublicAdvertised;
		bAllowInvites = false;
		bSanctionsEnabled = false;
		AllowedPlatformIds.Empty();
	}
	FEIK_SessionDetails_Settings(EOS_SessionDetails_Settings InSessionDetailsSettings)
	{
		BucketId = FString(UTF8_TO_TCHAR(InSessionDetailsSettings.BucketId));
		NumPublicConnections = InSessionDetailsSettings.NumPublicConnections;
		bAllowJoinInProgress = InSessionDetailsSettings.bAllowJoinInProgress == EOS_TRUE;
		PermissionLevel = static_cast<EEIK_EOnlineSessionPermissionLevel>(InSessionDetailsSettings.PermissionLevel);
		bAllowInvites = InSessionDetailsSettings.bInvitesAllowed == EOS_TRUE;
		bSanctionsEnabled = InSessionDetailsSettings.bSanctionsEnabled == EOS_TRUE;
		for (int32 i = 0; i < static_cast<int32>(InSessionDetailsSettings.AllowedPlatformIdsCount); i++)
		{
			AllowedPlatformIds.Add(InSessionDetailsSettings.AllowedPlatformIds[i]);
		}
	}
	
};

USTRUCT(BlueprintType)
struct FEIK_SessionDetails_Info
{
	GENERATED_BODY()

	/** Session ID assigned by the backend service */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FString SessionId;

	/** IP address of this session as visible by the backend service */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FString HostAddress;

	/** Number of remaining open spaces on the session (NumPublicConnections - RegisteredPlayers */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	int32 NumOpenPublicConnections;

	/** Reference to the additional settings associated with this session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_SessionDetails_Settings Settings;

	/** The Product User ID of the session owner. Null if the session is not owned by a user. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_ProductUserId OwnerUserId;

	/** The client id of the session owner. Null if the session is not owned by a server. The session is owned by a server if EOS_Platform_Options::bIsServer is EOS_TRUE. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FString OwnerServerClientId;

	FEIK_SessionDetails_Info()
	{
		SessionId = "";
		HostAddress = "";
		NumOpenPublicConnections = 0;
		Settings = FEIK_SessionDetails_Settings();
		OwnerUserId = FEIK_ProductUserId();
		OwnerServerClientId = "";
	}
	FEIK_SessionDetails_Info(EOS_SessionDetails_Info InSessionDetailsInfo)
	{
		SessionId = FString(UTF8_TO_TCHAR(InSessionDetailsInfo.SessionId));
		HostAddress = FString(UTF8_TO_TCHAR(InSessionDetailsInfo.HostAddress));
		NumOpenPublicConnections = InSessionDetailsInfo.NumOpenPublicConnections;
		Settings = *InSessionDetailsInfo.Settings;
		OwnerUserId = InSessionDetailsInfo.OwnerUserId;
		OwnerServerClientId = UTF8_TO_TCHAR(InSessionDetailsInfo.OwnerServerClientId);
	}
	
};


USTRUCT(BlueprintType)
struct FEIK_ActiveSession_Info
{
	GENERATED_BODY()
	
	/** Name of the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FString SessionName;

	/** The Product User ID of the local user who created or joined the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_ProductUserId LocalUserId;

	/** Current state of the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	TEnumAsByte<EEIK_EOnlineSessionState> State;

	/** Session details */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_SessionDetails_Info SessionDetails;

	FEIK_ActiveSession_Info()
	{
		SessionName = "";
		LocalUserId = FEIK_ProductUserId();
		State = EEIK_EOnlineSessionState::EIK_OSS_NoSession;
		SessionDetails = FEIK_SessionDetails_Info();
	}
	FEIK_ActiveSession_Info(EOS_ActiveSession_Info InActiveSessionInfo)
	{
		SessionName = FString(UTF8_TO_TCHAR(InActiveSessionInfo.SessionName));
		LocalUserId = InActiveSessionInfo.LocalUserId;
		State = static_cast<EEIK_EOnlineSessionState>(InActiveSessionInfo.State);
		SessionDetails = *InActiveSessionInfo.SessionDetails;
	}
};

USTRUCT(BlueprintType)
struct FEIK_HActiveSession
{
	GENERATED_BODY()

	EOS_HActiveSession Ref;

	FEIK_HActiveSession(): Ref(nullptr)
	{
	}
	FEIK_HActiveSession(EOS_HActiveSession InHActiveSession)
	{
		Ref = InHActiveSession;
	}
};

USTRUCT(BlueprintType)
struct FEIK_HSessionDetails
{
	GENERATED_BODY()

	EOS_HSessionDetails Ref;

	FEIK_HSessionDetails(): Ref()
	{
	}
	FEIK_HSessionDetails(EOS_HSessionDetails InHSessionDetails)
	{
		Ref = InHSessionDetails;
	}
};

UENUM(BlueprintType)
enum EIK_ESessionAttributeAdvertisementType
{
	/** Don't advertise via the online service */
	EIK_SAAT_DontAdvertise = 0,
	/** Advertise via the online service only */
	EIK_SAAT_Advertise = 1
};

USTRUCT(BlueprintType)
struct FEIK_Sessions_AttributeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FString Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	int64 ValueAsInt64;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FString ValueAsString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	bool bValueAsBool;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	float ValueAsDouble;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	TEnumAsByte<EEIK_EAttributeType> ValueType;

	FEIK_Sessions_AttributeData(): Key(), ValueAsInt64(0), ValueAsString(), bValueAsBool(false), ValueAsDouble(0.0), ValueType()
	{
	}
	FEIK_Sessions_AttributeData(EOS_Sessions_AttributeData InAttributeData)
	{
		Key = FString(UTF8_TO_TCHAR(InAttributeData.Key));
		ValueAsInt64 = InAttributeData.Value.AsInt64;
		ValueAsString = FString(UTF8_TO_TCHAR(InAttributeData.Value.AsUtf8));
		bValueAsBool = InAttributeData.Value.AsBool == EOS_TRUE;
		ValueAsDouble = InAttributeData.Value.AsDouble;
		ValueType = static_cast<EEIK_EAttributeType>(InAttributeData.ValueType);
	}
	EOS_Sessions_AttributeData GetValueAsEosType()
	{
		EOS_Sessions_AttributeData AttributeData;
		AttributeData.ApiVersion = EOS_SESSIONS_ATTRIBUTEDATA_API_LATEST;
		AttributeData.Key = TCHAR_TO_UTF8(*Key);
		AttributeData.Value.AsInt64 = ValueAsInt64;
		AttributeData.Value.AsUtf8 = TCHAR_TO_UTF8(*ValueAsString);
		AttributeData.Value.AsBool = bValueAsBool ? EOS_TRUE : EOS_FALSE;
		AttributeData.Value.AsDouble = ValueAsDouble;
		AttributeData.ValueType = static_cast<EOS_EAttributeType>(ValueType.GetValue());
		return AttributeData;
	}
};

USTRUCT(BlueprintType)
struct FEIK_SessionDetails_Attribute
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_Sessions_AttributeData Data;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	TEnumAsByte<EIK_ESessionAttributeAdvertisementType> ValueType;

	FEIK_SessionDetails_Attribute(): Data(), ValueType()
	{
	}
	FEIK_SessionDetails_Attribute(EOS_SessionDetails_Attribute InSessionDetailsAttribute)
	{
		Data = *InSessionDetailsAttribute.Data;
		ValueType = static_cast<EIK_ESessionAttributeAdvertisementType>(InSessionDetailsAttribute.AdvertisementType);
	}
};

USTRUCT(BlueprintType)
struct FEIK_HSessionModification
{
	GENERATED_BODY()

	EOS_SessionModificationHandle* Ref;

	FEIK_HSessionModification(): Ref(nullptr)
	{
	}
	FEIK_HSessionModification(EOS_SessionModificationHandle* InHSessionModification)
	{
		Ref = InHSessionModification;
	}
};

USTRUCT(BlueprintType)
struct FEIK_HSessionSearch
{
	GENERATED_BODY()

	EOS_HSessionSearch* Ref;

	FEIK_HSessionSearch(): Ref(nullptr)
	{
	}
	FEIK_HSessionSearch(EOS_HSessionSearch* InHSessionSearch)
	{
		Ref = InHSessionSearch;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Stats_Stat
{
	GENERATED_BODY()

	//Name of the stat.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	FString Name;

	//If not EOS_STATS_TIME_UNDEFINED then this is the POSIX timestamp for start time.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	int64 StartTime;

	//If not EOS_STATS_TIME_UNDEFINED then this is the POSIX timestamp for end time.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	int64 EndTime;

	//Current value for the stat.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	int32 Value;

	FEIK_Stats_Stat(): Name(), StartTime(0), EndTime(0), Value(0)
	{
	}

	FEIK_Stats_Stat(EOS_Stats_Stat InStat)
	{
		Name = FString(UTF8_TO_TCHAR(InStat.Name));
		StartTime = InStat.StartTime;
		EndTime = InStat.EndTime;
		Value = InStat.Value;
	}
};

UENUM(BlueprintType)
enum EEIK_UI_ENotificationLocation
{
	EIK_UNL_TopLeft,
	EIK_UNL_TopRight,
	EIK_UNL_BottomLeft,
	EIK_UNL_BottomRight
};


USTRUCT(BlueprintType)
struct FEIK_UserInfo_BestDisplayName
{
	GENERATED_BODY()

	//The Epic Account ID of the user
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FEIK_EpicAccountId UserId;

	//The display name (un-sanitized). This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString DisplayName;

	//The raw display name (sanitized). This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString DisplayNameSanitized;

	//A nickname/alias for the target user assigned by the local user. This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString Nickname;

	//The platform type for the user which corresponds to the display name
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	int32 PlatformType;

	FEIK_UserInfo_BestDisplayName(): UserId(), DisplayName(), DisplayNameSanitized(), Nickname(), PlatformType(0)
	{
	}
	FEIK_UserInfo_BestDisplayName(EOS_UserInfo_BestDisplayName InBestDisplayName)
	{
		UserId = InBestDisplayName.UserId;
		if(InBestDisplayName.DisplayName != nullptr)
		{
			DisplayName = FString(UTF8_TO_TCHAR(InBestDisplayName.DisplayName));
		}
		if(InBestDisplayName.DisplayNameSanitized != nullptr)
		{
			DisplayNameSanitized = FString(UTF8_TO_TCHAR(InBestDisplayName.DisplayNameSanitized));
		}
		if(InBestDisplayName.Nickname != nullptr)
		{
			Nickname = FString(UTF8_TO_TCHAR(InBestDisplayName.Nickname));
		}
		PlatformType = InBestDisplayName.PlatformType;
	}
};

USTRUCT(BlueprintType)
struct FEIK_UserInfo_ExternalUserInfo
{
	GENERATED_BODY()
	
	//The type of the external account
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	TEnumAsByte<EEIK_EExternalAccountType> AccountType;

	//The ID of the external account. Can be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString AccountId;

	//The display name of the external account (un-sanitized). Can be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString DisplayName;

	//The display name of the external account (sanitized). Can be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString DisplayNameSanitized;

	FEIK_UserInfo_ExternalUserInfo(): AccountType(), AccountId(), DisplayName(), DisplayNameSanitized()
	{
	}
	FEIK_UserInfo_ExternalUserInfo(EOS_UserInfo_ExternalUserInfo InExternalUserInfo)
	{
		AccountType = static_cast<EEIK_EExternalAccountType>(InExternalUserInfo.AccountType);
		if(InExternalUserInfo.AccountId != nullptr)
		{
			AccountId = FString(UTF8_TO_TCHAR(InExternalUserInfo.AccountId));
		}
		if(InExternalUserInfo.DisplayName != nullptr)
		{
			DisplayName = FString(UTF8_TO_TCHAR(InExternalUserInfo.DisplayName));
		}
		if(InExternalUserInfo.DisplayNameSanitized != nullptr)
		{
			DisplayNameSanitized = FString(UTF8_TO_TCHAR(InExternalUserInfo.DisplayNameSanitized));
		}
	}
};

USTRUCT(BlueprintType)
struct FEIK_UserInfo
{
	GENERATED_BODY()

	//The Epic Account ID of the user
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FEIK_EpicAccountId UserId;

	//The name of the owner's country. This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString Country;

	//The display name (un-sanitized). This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString DisplayName;

	//The ISO 639 language code for the user's preferred language. This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString PreferredLanguage;

	//A nickname/alias for the target user assigned by the local user. This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString Nickname;

	//The raw display name (sanitized). This may be null
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | User Info Interface")
	FString DisplayNameSanitized;

	FEIK_UserInfo(): UserId(), Country(), DisplayName(), PreferredLanguage(), Nickname(), DisplayNameSanitized()
	{
	}
	FEIK_UserInfo(EOS_UserInfo InUserInfo)
	{
		UserId = InUserInfo.UserId;
		if(InUserInfo.Country != nullptr)
		{
			Country = FString(UTF8_TO_TCHAR(InUserInfo.Country));
		}
		if(InUserInfo.DisplayName != nullptr)
		{
			DisplayName = FString(UTF8_TO_TCHAR(InUserInfo.DisplayName));
		}
		if(InUserInfo.PreferredLanguage != nullptr)
		{
			PreferredLanguage = FString(UTF8_TO_TCHAR(InUserInfo.PreferredLanguage));
		}
		if(InUserInfo.Nickname != nullptr)
		{
			Nickname = FString(UTF8_TO_TCHAR(InUserInfo.Nickname));
		}
		if(InUserInfo.DisplayNameSanitized != nullptr)
		{
			DisplayNameSanitized = FString(UTF8_TO_TCHAR(InUserInfo.DisplayNameSanitized));
		}
	}
};

USTRUCT(BlueprintType)
struct FEIK_P2P_SocketId
{
	GENERATED_BODY()

	/** A name for the connection. Must be a NULL-terminated string of between 1-32 alpha-numeric characters (A-Z, a-z, 0-9, '-', '_', ' ', '+', '=', '.') */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	FString SocketName;

	FEIK_P2P_SocketId(): SocketName()
	{
	}
	FEIK_P2P_SocketId(EOS_P2P_SocketId InSocketId)
	{
		SocketName = FString(UTF8_TO_TCHAR(InSocketId.SocketName));
	}
	EOS_P2P_SocketId GetAsEosData()
	{
		EOS_P2P_SocketId SocketId;
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*SocketName), SocketName.Len() + 1);
		return SocketId;
	}
};


UENUM(BlueprintType)
enum EEIK_EConnectionClosedReason
{
	/** The connection was closed for unknown reasons. This most notably happens during application shutdown. */
	EIK_CCR_Unknown = 0,
	/** The connection was at least locally accepted, but was closed by the local user via a call to EOS_P2P_CloseConnection / EOS_P2P_CloseConnections. */
	EIK_CCR_ClosedByLocalUser = 1,
	/** The connection was at least locally accepted, but was gracefully closed by the remote user via a call to EOS_P2P_CloseConnection / EOS_P2P_CloseConnections. */
	EIK_CCR_ClosedByPeer = 2,
	/** The connection was at least locally accepted, but was not remotely accepted in time. */
	EIK_CCR_TimedOut = 3,
	/** The connection was accepted, but the connection could not be created due to too many other existing connections */
	EIK_CCR_TooManyConnections = 4,
	/** The connection was accepted, The remote user sent an invalid message */
	EIK_CCR_InvalidMessage = 5,
	/** The connection was accepted, but the remote user sent us invalid data */
	EIK_CCR_InvalidData = 6,
	/** The connection was accepted, but we failed to ever establish a connection with the remote user due to connectivity issues. */
	EIK_CCR_ConnectionFailed = 7,
	/** The connection was accepted and established, but the peer silently went away. */
	EIK_CCR_ConnectionClosed = 8,
	/** The connection was locally accepted, but we failed to negotiate a connection with the remote user. This most commonly occurs if the local user goes offline or is logged-out during the connection process. */
	EIK_CCR_NegotiationFailed = 9,
	/** The connection was accepted, but there was an internal error occurred and the connection cannot be created or continue. */
	EIK_CCR_UnexpectedError = 10
};

UENUM(BlueprintType)
enum EEIK_EConnectionEstablishedType
{
	/** The connection is brand new */
	EIK_CET_NewConnection = 0,
	/** The connection is reestablished (reconnection) */
	EIK_CET_Reconnection = 1
};

UENUM(BlueprintType)
enum EEIK_ENetworkConnectionType
{
	/** There is no established connection */
	EIK_NCT_NoConnection = 0,
	/** A direct connection to the peer over the Internet or Local Network */
	EIK_NCT_DirectConnection = 1,
	/** A relayed connection using Epic-provided servers to the peer over the Internet */
	EIK_NCT_RelayedConnection = 2
};

UENUM(BlueprintType)
enum EEIK_ENATType
{
	/** NAT type either unknown (remote) or we are unable to determine it (local) */
	EIK_NAT_Unknown = 0,
	/** All peers can directly-connect to you */
	EIK_NAT_Open = 1,
	/** You can directly-connect to other Moderate and Open peers */
	EIK_NAT_Moderate = 2,
	/** You can only directly-connect to Open peers */
	EIK_NAT_Strict = 3
};

USTRUCT(BlueprintType)
struct FEIK_P2P_PacketQueueInfo
{
	GENERATED_BODY()

	//The maximum size in bytes of the incoming packet queue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 IncomingPacketQueueMaxSizeBytes;

	//The current size in bytes of the incoming packet queue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 IncomingPacketQueueCurrentSizeBytes;

	//The current number of queued packets in the incoming packet queue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 IncomingPacketQueueCurrentPacketCount;

	//The maximum size in bytes of the outgoing packet queue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 OutgoingPacketQueueMaxSizeBytes;

	//The current size in bytes of the outgoing packet queue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 OutgoingPacketQueueCurrentSizeBytes;

	//The current amount of queued packets in the outgoing packet queue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | P2P Interface")
	int64 OutgoingPacketQueueCurrentPacketCount;

	FEIK_P2P_PacketQueueInfo(): IncomingPacketQueueMaxSizeBytes(0), IncomingPacketQueueCurrentSizeBytes(0), IncomingPacketQueueCurrentPacketCount(0),
	                           OutgoingPacketQueueMaxSizeBytes(0), OutgoingPacketQueueCurrentSizeBytes(0), OutgoingPacketQueueCurrentPacketCount(0)
	{
	}
	FEIK_P2P_PacketQueueInfo(EOS_P2P_PacketQueueInfo InPacketQueueInfo)
	{
		IncomingPacketQueueMaxSizeBytes = InPacketQueueInfo.IncomingPacketQueueMaxSizeBytes;
		IncomingPacketQueueCurrentSizeBytes = InPacketQueueInfo.IncomingPacketQueueCurrentSizeBytes;
		IncomingPacketQueueCurrentPacketCount = InPacketQueueInfo.IncomingPacketQueueCurrentPacketCount;
		OutgoingPacketQueueMaxSizeBytes = InPacketQueueInfo.OutgoingPacketQueueMaxSizeBytes;
		OutgoingPacketQueueCurrentSizeBytes = InPacketQueueInfo.OutgoingPacketQueueCurrentSizeBytes;
		OutgoingPacketQueueCurrentPacketCount = InPacketQueueInfo.OutgoingPacketQueueCurrentPacketCount;
	}
};

UENUM(BlueprintType)
enum EEIK_ERelayControl
{
	/** Peer connections will never attempt to use relay servers. Clients with restrictive NATs may not be able to connect to peers. */
	EIK_RC_NoRelays = 0,
	/** Peer connections will attempt to use relay servers, but only after direct connection attempts fail. This is the default value if not changed. */
	EIK_RC_AllowRelays = 1,
	/** Peer connections will only ever use relay servers. This will add latency to all connections, but will hide IP Addresses from peers. */
	EIK_RC_ForceRelays = 2
};

UENUM(BlueprintType)
enum EEIK_EPacketReliability
{
	/** Packets will only be sent once and may be received out of order */
	EIK_PR_UnreliableUnordered = 0,
	/** Packets may be sent multiple times and may be received out of order */
	EIK_PR_ReliableUnordered = 1,
	/** Packets may be sent multiple times and will be received in order */
	EIK_PR_ReliableOrdered = 2
};

USTRUCT(BlueprintType)
struct FEIK_RTC_Option
{
	GENERATED_BODY()

	//The unique key of the option. The max size of the string is EOS_RTC_OPTION_KEY_MAXCHARCOUNT
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	FString Key;

	//The value of the option. The max size of the string is EOS_RTC_OPTION_VALUE_MAXCHARCOUNT.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	FString Value;

	FEIK_RTC_Option(): Key(), Value()
	{
	}
	FEIK_RTC_Option(EOS_RTC_Option InOption)
	{
		Key = FString(UTF8_TO_TCHAR(InOption.Key));
		Value = FString(UTF8_TO_TCHAR(InOption.Value));
	}
};

USTRUCT(BlueprintType)
struct FEIK_RTCAdmin_UserToken
{
	GENERATED_BODY()

	//The Product User ID for the user who owns this user token
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Admin Interface")
	FEIK_ProductUserId ProductUserId;

	//Access token to enable a user to join a room
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Admin Interface")
	FString Token;

	FEIK_RTCAdmin_UserToken(): ProductUserId(), Token()
	{
	}
	FEIK_RTCAdmin_UserToken(EOS_RTCAdmin_UserToken InUserToken)
	{
		ProductUserId = InUserToken.ProductUserId;
		Token = FString(UTF8_TO_TCHAR(InUserToken.Token));
	}
};

USTRUCT(BlueprintType)
struct FEIK_RTCAudio_AudioBuffer
{
	GENERATED_BODY()

	/** Pointer to the data with the interleaved audio frames in signed 16 bits format. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	TArray<int32> Data;

	/**
 * Number of frames available in the Frames buffer.
 * @note This is the number of frames in a channel, not the total number of frames in the buffer.
 */
 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	int32 FramesCount;

	/** Sample rate for the samples in the Frames buffer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	int32 SampleRate;

	/** Number of channels for the samples in the Frames buffer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	int32 NumChannels;

	EOS_RTCAudio_AudioBuffer GetValueAsEosType() const
	{
		EOS_RTCAudio_AudioBuffer AudioBuffer;
		AudioBuffer.ApiVersion = EOS_RTCAUDIO_AUDIOBUFFER_API_LATEST;
		AudioBuffer.Frames = new int16[Data.Num()];
		for (int32 i = 0; i < Data.Num(); i++)
		{
			AudioBuffer.Frames[i] = Data[i];
		}
		AudioBuffer.FramesCount = FramesCount;
		AudioBuffer.SampleRate = SampleRate;
		AudioBuffer.Channels = NumChannels;
		return AudioBuffer;
	}
	FEIK_RTCAudio_AudioBuffer(): Data(), FramesCount(0), SampleRate(0), NumChannels(0)
	{
	}
	FEIK_RTCAudio_AudioBuffer(EOS_RTCAudio_AudioBuffer InAudioBuffer)
	{
		Data.Empty();
		for (int32 i = 0; i < static_cast<int32>(InAudioBuffer.FramesCount * InAudioBuffer.Channels); i++)
		{
			Data.Add(InAudioBuffer.Frames[i]);
		}
		FramesCount = InAudioBuffer.FramesCount;
		SampleRate = InAudioBuffer.SampleRate;
		NumChannels = InAudioBuffer.Channels;
	}
	FEIK_RTCAudio_AudioBuffer(EOS_RTCAudio_AudioBuffer* InAudioBuffer)
	{
		Data.Empty();
		for (int32 i = 0; i < static_cast<int32>(InAudioBuffer->FramesCount * InAudioBuffer->Channels); i++)
		{
			Data.Add(InAudioBuffer->Frames[i]);
		}
		FramesCount = InAudioBuffer->FramesCount;
		SampleRate = InAudioBuffer->SampleRate;
		NumChannels = InAudioBuffer->Channels;
	}
};


UENUM(BlueprintType)
enum EEIK_ERTCAudioInputStatus
{
	EIK_RTCAIS_Idle = 0 UMETA(DisplayName = "Idle"),
	EIK_RTCAIS_Recording = 1 UMETA(DisplayName = "Recording"),
	EIK_RTCAIS_RecordingSilent = 2 UMETA(DisplayName = "Recording Silent"),
	EIK_RTCAIS_RecordingDisconnected = 3 UMETA(DisplayName = "Recording Disconnected"),
	EIK_RTCAIS_Failed = 4 UMETA(DisplayName = "Failed"),
};

UENUM(BlueprintType)
enum EEIK_ERTCAudioOutputStatus
{
	EIK_RTCAOS_Idle = 0 UMETA(DisplayName = "Idle"),
	EIK_RTCAOS_Playing = 1 UMETA(DisplayName = "Playing"),
	EIK_RTCAOS_Failed = 2 UMETA(DisplayName = "Failed"),
};

UENUM(BlueprintType)
enum EEIK_ERTCAudioStatus
{
	EIK_RTCAS_Unsupported = 0 UMETA(DisplayName = "Unsupported"),
	EIK_RTCAS_Enabled = 1 UMETA(DisplayName = "Enabled"),
	//Audio disabled
	EIK_RTCAS_Disabled = 2 UMETA(DisplayName = "Disabled"),
	//Audio disabled by the administrator
	EIK_RTCAS_DisabledByAdmin = 3 UMETA(DisplayName = "Disabled By Admin"),
	//Audio channel is disabled temporarily for both sending and receiving
	EIK_RTCAS_NotListeningDisabled = 4 UMETA(DisplayName = "Not Listening Disabled"),
};

USTRUCT(BlueprintType)
struct FEIK_RTCAudio_InputDeviceInformation
{
	GENERATED_BODY()

	//True if this is the default audio input device in the system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	bool bDefaultDevice;

	//The persistent unique id of the audio input device. The value can be cached - invalidated only when the audio device pool is changed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	FString DeviceId;

	//Human-readable name of the audio input device
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	FString DeviceName;

	EOS_RTCAudio_InputDeviceInformation Ref;
	FEIK_RTCAudio_InputDeviceInformation(): bDefaultDevice(false), DeviceId(), DeviceName(), Ref()
	{
	}

	FEIK_RTCAudio_InputDeviceInformation(EOS_RTCAudio_InputDeviceInformation InInputDeviceInformation)
	{
		Ref = InInputDeviceInformation;
		bDefaultDevice = InInputDeviceInformation.bDefaultDevice == EOS_TRUE;
		DeviceId = FString(UTF8_TO_TCHAR(InInputDeviceInformation.DeviceId));
		DeviceName = FString(UTF8_TO_TCHAR(InInputDeviceInformation.DeviceName));
	}
};

USTRUCT(BlueprintType)
struct FEIK_RTCAudio_OutputDeviceInformation
{
	GENERATED_BODY()

	//True if this is the default audio output device in the system.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	bool bDefaultDevice;

	//The persistent unique id of the audio output device. The value can be cached - invalidated only when the audio device pool is changed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	FString DeviceId;

	//Human-readable name of the audio output device
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | RTC Audio Interface")
	FString DeviceName;

	EOS_RTCAudio_OutputDeviceInformation Ref;
	FEIK_RTCAudio_OutputDeviceInformation(): bDefaultDevice(false), DeviceId(), DeviceName(), Ref()
	{
	}

	FEIK_RTCAudio_OutputDeviceInformation(EOS_RTCAudio_OutputDeviceInformation InOutputDeviceInformation)
	{
		Ref = InOutputDeviceInformation;
		bDefaultDevice = InOutputDeviceInformation.bDefaultDevice == EOS_TRUE;
		DeviceId = FString(UTF8_TO_TCHAR(InOutputDeviceInformation.DeviceId));
		DeviceName = FString(UTF8_TO_TCHAR(InOutputDeviceInformation.DeviceName));
	}
};


UENUM(BlueprintType)
enum EEIK_ERTCDataStatus
{
	EIK_RTCDS_Unsupported = 0 UMETA(DisplayName = "Unsupported"),
	EIK_RTCDS_Enabled = 1 UMETA(DisplayName = "Enabled"),
	//Data disabled
	EIK_RTCDS_Disabled = 2 UMETA(DisplayName = "Disabled"),
};

DECLARE_LOG_CATEGORY_EXTERN(LogEIK, Verbose, All);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SharedFunctionFile : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "EOS Integration Kit | SDK Functions | Shared Functions")
	static FString ConvertOculusUserIdAndNonceToEosFormat(FString OculusUserId, FString OculusNonce);
};
