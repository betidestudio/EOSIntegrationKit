// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_common.h"
#include "UObject/Object.h"
#include "EIK_SharedFunctionFile.generated.h"

USTRUCT(BlueprintType)
struct FEIK_ProductUserId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS|Connect")
	FString ProductUserId;

	FEIK_ProductUserId()
	{
		ProductUserId = "";
	}

	FEIK_ProductUserId(EOS_ProductUserId InProductUserId)
	{
		char ProductIdAnsi[EOS_PRODUCTUSERID_MAX_LENGTH+1];
		int32 ProductIdLen;
		EOS_ProductUserId_ToString(InProductUserId, ProductIdAnsi, &ProductIdLen);
		ProductUserId = FString(UTF8_TO_TCHAR(ProductIdAnsi));
	}

	EOS_ProductUserId ProductUserId_FromString()
	{
		const char* ProductIdAnsi = TCHAR_TO_ANSI(*ProductUserId);
		EOS_ProductUserId ProductUserIdSec = EOS_ProductUserId_FromString(ProductIdAnsi);
		return ProductUserIdSec;
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

	FEIK_ContinuanceToken()
	{
		ContinuanceToken = "";
	}

	FEIK_ContinuanceToken(EOS_ContinuanceToken InContinuanceToken)
	{
		ContinuanceToken = FString(UTF8_TO_TCHAR(InContinuanceToken));
	}
	
	EOS_ContinuanceToken EOS_ContinuanceToken_FromString()
	{
		const char* StringToken = TCHAR_TO_ANSI(*ContinuanceToken);
		// Allocate memory for the EOS_ContinuanceToken
		size_t tokenSize = std::strlen(StringToken) + 1;
		EOS_ContinuanceToken ContinuanceTokenSec = (EOS_ContinuanceToken)malloc(tokenSize);
		if (ContinuanceTokenSec)
		{
			std::strcpy((char*)ContinuanceTokenSec, StringToken);
		}
		return ContinuanceTokenSec;
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

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SharedFunctionFile : public UObject
{
	GENERATED_BODY()
};
