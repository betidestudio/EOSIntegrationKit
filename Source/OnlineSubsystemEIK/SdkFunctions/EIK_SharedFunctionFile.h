// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
THIRD_PARTY_INCLUDES_START
#include "eos_common.h"
#include "eos_ecom_types.h"
#include "eos_ecom.h"
#include "eos_connect_types.h"
#include "eos_achievements_types.h"
#include "eos_auth_types.h"
THIRD_PARTY_INCLUDES_END
#include "UObject/Object.h"
#include "EIK_SharedFunctionFile.generated.h"

USTRUCT(BlueprintType)
struct FEIK_ProductUserId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS|Connect")
	FString ProductUserId;

	EOS_ProductUserId ProductUserIdBasic;

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

	EOS_ProductUserId ProductUserId_FromString()
	{
		if (EOS_ProductUserId_IsValid(ProductUserIdBasic))
		{
			return ProductUserIdBasic;
		}
		const char* ProductIdAnsi = TCHAR_TO_ANSI(*ProductUserId);
		EOS_ProductUserId ProductUserIdSec = EOS_ProductUserId_FromString(ProductIdAnsi);
		return ProductUserIdSec;
	}
};

USTRUCT(BlueprintType)
struct FEIK_EpicAccountId 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Auth Interface")
	FString EpicAccountId;

	EOS_EpicAccountId EpicAccountIdBasic;

	FEIK_EpicAccountId(): EpicAccountIdBasic(nullptr)
	{
		EpicAccountId = "";
	}

	FEIK_EpicAccountId(EOS_EpicAccountId InEpicAccountId)
	{
		EpicAccountIdBasic = InEpicAccountId;
		char EpicAccountIdAnsi[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
		int32 EpicAccountIdLen;
		EOS_EpicAccountId_ToString(InEpicAccountId, EpicAccountIdAnsi, &EpicAccountIdLen);
		EpicAccountId = FString(UTF8_TO_TCHAR(EpicAccountIdAnsi));
	}
	EOS_EpicAccountId EpicAccountId_FromString()
	{
		if (EOS_EpicAccountId_IsValid(EpicAccountIdBasic))
		{
			return EpicAccountIdBasic;
		}
		const char* EpicAccountIdAnsi = TCHAR_TO_ANSI(*EpicAccountId);
		EOS_EpicAccountId EpicAccountIdSec = EOS_EpicAccountId_FromString(EpicAccountIdAnsi);
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

	EOS_ContinuanceToken ContinuanceTokenBasic;

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
	
	EOS_ContinuanceToken EOS_ContinuanceToken_FromStruct()
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

	EOS_NotificationId EOS_NotificationId_FromInt64()
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

	EOS_Connect_IdToken EOS_Connect_IdToken_FromStruct()
	{
		EOS_Connect_IdToken IdToken;
		IdToken.ApiVersion = EOS_CONNECT_IDTOKEN_API_LATEST;
		IdToken.ProductUserId = UserId.ProductUserId_FromString();
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
	EIK_ECT_STEAM_APP_TICKET = 1,
	/**
	 * PlayStation(TM)Network ID Token
	 *
	 * Retrieved from the PlayStation(R) SDK. Please see first-party documentation for additional information.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EIK_ECT_PSN_ID_TOKEN = 2,
	/**
	 * Xbox Live XSTS Token
	 *
	 * Retrieved from the GDK and XDK. Please see first-party documentation for additional information.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EIK_ECT_XBL_XSTS_TOKEN = 3,
	/**
	 * Discord Access Token
	 *
	 * Retrieved using the ApplicationManager::GetOAuth2Token API of Discord SDK.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_DISCORD_ACCESS_TOKEN = 4,
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
	EIK_ECT_GOG_SESSION_TICKET = 5,
	/**
	 * Nintendo Account ID Token
	 *
	 * Identifies a Nintendo user account and is acquired through web flow authentication where the local user logs in using their email address/sign-in ID and password.
	 * This is the common Nintendo account that users login with outside the Nintendo Switch device.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EIK_ECT_NINTENDO_ID_TOKEN = 6,
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
	EIK_ECT_NINTENDO_NSA_ID_TOKEN = 7,
	/**
	 * Uplay Access Token
	 */
	EIK_ECT_UPLAY_ACCESS_TOKEN = 8,
	/**
	 * OpenID Provider Access Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_OPENID_ACCESS_TOKEN = 9,
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
	EIK_ECT_DEVICEID_ACCESS_TOKEN = 10,
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
	EIK_ECT_GOOGLE_ID_TOKEN = 12,
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
	EIK_ECT_OCULUS_USERID_NONCE = 13,
	/**
	 * itch.io JWT Access Token
	 *
	 * Use the itch.io app manifest to receive a JWT access token for the local user via the ITCHIO_API_KEY process environment variable.
	 * The itch.io access token is valid for 7 days after which the game needs to be restarted by the user as otherwise EOS Connect
	 * authentication session can no longer be refreshed.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_ITCHIO_JWT = 14,
	/**
	 * itch.io Key Access Token
	 *
	 * This access token type is retrieved through the OAuth 2.0 authentication flow for the itch.io application.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_ITCHIO_KEY = 15,
	/**
	 * Epic Games ID Token
	 *
	 * Acquired using EOS_Auth_CopyIdToken that returns EOS_Auth_IdToken::JsonWebToken.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_EPIC_ID_TOKEN = 16,
	/**
	 * Amazon Access Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_AMAZON_ACCESS_TOKEN = 17,
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
	EIK_ECT_STEAM_SESSION_TICKET = 18,
	/**
	 * VIVEPORT User Session Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EIK_ECT_VIVEPORT_USER_TOKEN = 19

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
	EOS_Connect_ExternalAccountInfo EOS_Connect_ExternalAccountInfo_FromStruct()
	{
		EOS_Connect_ExternalAccountInfo ExternalAccountInfo;
		ExternalAccountInfo.ApiVersion = EOS_CONNECT_EXTERNALACCOUNTINFO_API_LATEST;
		ExternalAccountInfo.ProductUserId = UserId.ProductUserId_FromString();
		ExternalAccountInfo.AccountId = TCHAR_TO_ANSI(*AccountId);
		ExternalAccountInfo.DisplayName = TCHAR_TO_ANSI(*DisplayName);
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

	EOS_Achievements_DefinitionV2* DefinitionRef;
	
	FEIK_Achievements_DefinitionV2(): bIsHidden(false)
	{
	}

	FEIK_Achievements_DefinitionV2(EOS_Achievements_DefinitionV2 InDefinition)
	{
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
	EOS_Achievements_DefinitionV2* GetReference()
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
	double Progress;

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

	FEIK_Achievements_PlayerAchievement()
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

	EOS_Achievements_PlayerAchievement* GetReference()
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
	EOS_Auth_IdToken EOS_Auth_IdToken_FromStruct()
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
	EOS_Auth_Token EOS_Auth_Token_FromStruct()
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

	EOS_Auth_PinGrantInfo EOS_Auth_PinGrantInfo_FromStruct()
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
	EIK_LCT_DeviceCode = 3 UMETA(DisplayName = "Developer"),
	//Login with named credentials hosted by the EOS SDK Developer Authentication Tool.
	EIK_LCT_Developer = 4 UMETA(DisplayName = "Device Code"),
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
	EOS_Auth_Credentials EOS_Auth_Credentials_FromStruct()
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






UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SharedFunctionFile : public UObject
{
	GENERATED_BODY()
};
