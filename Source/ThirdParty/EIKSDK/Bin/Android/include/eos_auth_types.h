// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_auth_types_deprecated.inl"
#include "eos_common.h"

#pragma pack(push, 8)

EXTERN_C typedef struct EOS_AuthHandle* EOS_HAuth;

/**
 * All possible types of login methods, availability depends on permissions granted to the client.
 *
 * @see EOS_Auth_Login
 * @see EOS_Auth_Credentials
 */
EOS_ENUM(EOS_ELoginCredentialType,
	/**
	 * Login using account email address and password.
	 *
	 * @note Use of this login method is restricted and cannot be used in general.
	 */
	EOS_LCT_Password = 0,
	/**
	 * A short-lived one-time use exchange code to login the local user.
	 *
	 * @details Typically retrieved via command-line parameters provided by a launcher that generated the exchange code for this application.
	 * When started, the application is expected to consume the exchange code by using the EOS_Auth_Login API as soon as possible.
	 * This is needed in order to authenticate the local user before the exchange code would expire.
	 * Attempting to consume an already expired exchange code will return EOS_EResult::EOS_Auth_ExchangeCodeNotFound error by the EOS_Auth_Login API.
	 */
	EOS_LCT_ExchangeCode = 1,
	/**
	 * Used by standalone applications distributed outside the supported game platforms such as Epic Games Store or Steam, and on Nintendo Switch.
	 *
	 * Persistent Auth is used in conjuction with the EOS_LCT_AccountPortal login method for automatic login of the local user across multiple runs of the application.
	 *
	 * Standalone applications implement the login sequence as follows:
	 * 1. Application calls EOS_Auth_Login with EOS_LCT_PersistentAuth, using a previously stored Epic refresh token for an automatic user login.
	 * 2. If automatic login fails, the application discards the Epic refresh token used as defunct, and proceeds to call EOS_Auth_Login with EOS_LCT_AccountPortal to prompt the user for manual login.
	 *
	 * @note On Desktop and Mobile platforms, the persistent refresh token is automatically managed by the SDK that stores it in the keychain of the currently logged in user of the local device.
	 * On Nintendo Switch, after a successful login the refresh token must be retrieved using the EOS_Auth_CopyUserAuthToken API and stored by the application specifically for the active Nintendo Switch user.
	 *
	 * @see EOS_LCT_AccountPortal
	 */
	EOS_LCT_PersistentAuth = 2,
	/**
	 * Not supported. Superseded by EOS_LCT_ExternalAuth login method.
	 *
	 * @see EOS_LCT_ExternalAuth
	 */
	EOS_LCT_DeviceCode = 3,
	/**
	 * Login with named credentials hosted by the EOS SDK Developer Authentication Tool.
	 *
	 * @note Used for development purposes only.
	 */
	EOS_LCT_Developer = 4,
	/**
	 * Refresh token that was retrieved from a previous call to EOS_Auth_Login API in another local process context.
	 * Mainly used in conjunction with custom desktop launcher applications.
	 *
	 * @details Can be used for example when launching the game from Epic Games Launcher and having an intermediate process
	 * in-between that requires authenticating the user before eventually starting the actual game client application.
	 * In such scenario, an intermediate launcher will log in the user by consuming the exchange code it received from the
	 * Epic Games Launcher. To allow the game client to also authenticate the user, it can copy the refresh token using the
	 * EOS_Auth_CopyUserAuthToken API and pass it via launch parameters to the started game client. The game client can then
	 * use the refresh token to log in the user.
	 */
	EOS_LCT_RefreshToken = 5,
	/**
	 * Used by standalone applications distributed outside the supported game platforms such as Epic Games Store or Steam, and on Nintendo Switch.
	 *
	 * Login using the built-in user onboarding experience provided by the SDK, which will automatically store a persistent
	 * refresh token to enable automatic user login for consecutive application runs on the local device. Applications are
	 * expected to attempt automatic login using the EOS_LCT_PersistentAuth login method, and fall back to EOS_LCT_AccountPortal
	 * to prompt users for manual login.
	 *
	 * @note On Windows, using this login method requires applications to be started through the EOS Bootstrapper application
	 * and to have the local Epic Online Services redistributable installed on the local system. See EOS_Platform_GetDesktopCrossplayStatus
	 * for adding a readiness check prior to calling EOS_Auth_Login.
	 *
	 * @see EOS_LCT_PersistentAuth
	 */
	EOS_LCT_AccountPortal = 6,
	/**
	 * Login using external account provider credentials, such as PlayStation(TM)Network, Steam, and Xbox Live.
	 *
	 * This is the intended login method on PlayStation® and Xbox console devices.
	 * On Desktop and Mobile, used when launched through any of the commonly supported platform clients.
	 *
	 * @details The user is seamlessly logged in to their Epic account using an external account access token.
	 * If the local platform account is already linked with the user's Epic account, the login will succeed and EOS_EResult::EOS_Success is returned.
	 * When the local platform account has not been linked with an Epic account yet,
	 * EOS_EResult::EOS_InvalidUser is returned and the EOS_ContinuanceToken will be set in the EOS_Auth_LoginCallbackInfo data.
	 * If EOS_EResult::EOS_InvalidUser is returned,
	 * the application should proceed to call the EOS_Auth_LinkAccount API with the EOS_ContinuanceToken to continue with the external account login
	 * and to link the external account at the end of the login flow.
	 *
	 * @details Login flow when the platform user account has not been linked with an Epic account yet:
	 * 1. Game calls EOS_Auth_Login with the EOS_LCT_ExternalAuth credential type.
	 * 2. EOS_Auth_Login returns EOS_EResult::EOS_InvalidUser with a non-null EOS_ContinuanceToken in the EOS_Auth_LoginCallbackInfo data.
	 * 3. Game calls EOS_Auth_LinkAccount with the EOS_ContinuanceToken to initiate the login flow for linking the platform account with the user's Epic account.
	 * 4. The user is taken automatically to the Epic accounts user onboarding flow managed by the SDK.
	 * 5. Once the user completes the login, cancels it or if the login flow times out, EOS_Auth_LinkAccount invokes the completion callback to the caller.
	 *    - If the user was logged in successfully, EOS_EResult::EOS_Success is returned in the EOS_Auth_LoginCallbackInfo. Otherwise, an error result code is returned accordingly.
	 *
	 * @note On Windows, using this login method requires applications to be started through the EOS Bootstrapper application
	 * and to have the local Epic Online Services redistributable installed on the local system. See EOS_Platform_GetDesktopCrossplayStatus
	 * for adding a readiness check prior to calling EOS_Auth_Login.
	 */
	EOS_LCT_ExternalAuth = 7
);

/** The most recent version of the EOS_Auth_Token struct. */
#define EOS_AUTH_TOKEN_API_LATEST 2

/**
 * Types of auth tokens
 *
 * @see EOS_Auth_CopyUserAuthToken
 * @see EOS_Auth_Token
 */
EOS_ENUM(EOS_EAuthTokenType,
	/** Auth token is for a validated client */
	EOS_ATT_Client = 0,
	/** Auth token is for a validated user */
	EOS_ATT_User = 1
);

/**
 * A structure that contains an auth token.
 * These structures are created by EOS_Auth_CopyUserAuthToken and must be passed to EOS_Auth_Token_Release.
 */
EOS_STRUCT(EOS_Auth_Token, (
	/** API Version: Set this to EOS_AUTH_TOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** Name of the app related to the client ID involved with this token */
	const char* App;
	/** Client ID that requested this token */
	const char* ClientId;
	/** The Epic Account ID associated with this auth token */
	EOS_EpicAccountId AccountId;
	/** Access token for the current user login session */
	const char* AccessToken;
	/** Time before the access token expires, in seconds, relative to the call to EOS_Auth_CopyUserAuthToken */
	double ExpiresIn;
	/** Absolute time in UTC before the access token expires, in ISO 8601 format */
	const char* ExpiresAt;
	/** Type of auth token */
	EOS_EAuthTokenType AuthType;
	/**
	 * Refresh token.
	 *
	 * @see EOS_ELoginCredentialType::EOS_LCT_RefreshToken
	 */
	const char* RefreshToken;
	/** Time before the access token expires, in seconds, relative to the call to EOS_Auth_CopyUserAuthToken */
	double RefreshExpiresIn;
	/** Absolute time in UTC before the refresh token expires, in ISO 8601 format */
	const char* RefreshExpiresAt;
));

/**
 * Release the memory associated with an EOS_Auth_Token structure. This must be called on data retrieved from EOS_Auth_CopyUserAuthToken.
 *
 * @param AuthToken The auth token structure to be released.
 *
 * @see EOS_Auth_Token
 * @see EOS_Auth_CopyUserAuthToken
 */
EOS_DECLARE_FUNC(void) EOS_Auth_Token_Release(EOS_Auth_Token* AuthToken);

/** The most recent version of the EOS_Auth_Credentials struct. */
#define EOS_AUTH_CREDENTIALS_API_LATEST 4

/**
 * Login credentials filled as part of the EOS_Auth_LoginOptions struct for EOS_Auth_Login API.
 *
 * Required input parameters to be set depend on the login credential type.
 * Any parameters not being used must be set to NULL. Otherwise, EOS_InvalidParameters error is returned.
 *
 * EOS_LCT_Password			| ID is the email address, and Token is the password.
 * EOS_LCT_ExchangeCode		| Set ID to NULL. Token is the exchange code.
 * EOS_LCT_PersistentAuth	| Set ID to NULL. On console platforms, Token is the long-lived refresh token. Otherwise, set to NULL.
 * EOS_LCT_Developer		| Set ID as the host (e.g. localhost:6547). Token is the credential name registered in the EOS Developer Authentication Tool.
 * EOS_LCT_RefreshToken		| Set ID to NULL. Token is the refresh token.
 * EOS_LCT_AccountPortal	| Set ID and Token to NULL. SystemAuthCredentialsOptions may be required on mobile platforms.
 * EOS_LCT_ExternalAuth		| Set ID to NULL or the External Account ID that belongs to the external auth token. Token is the external auth token specified by ExternalType. External Account IDs set to the ID are expected as either base-10 numeric strings for integer-based external Account IDs, or the actual string for everything else. If ID is provided, login will automatically be cancelled if the EOS SDK is able to and does detect the external account signing-out. If ID is provided, it must match the external account ID belonging to the auth-token, or login will fail.
 *
 * @see EOS_ELoginCredentialType
 * @see EOS_Auth_Login
 * @see EOS_Auth_DeletePersistentAuthOptions
 */
EOS_STRUCT(EOS_Auth_Credentials, (
	/** API Version: Set this to EOS_AUTH_CREDENTIALS_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * Authentication ID value based on the used EOS_ELoginCredentialType.
	 * If not used, must be set to NULL.
	 */
	const char* Id;
	/**
	 * Authentication Token value based on the used EOS_ELoginCredentialType.
	 * If not used, must be set to NULL.
	 */
	const char* Token;
	/** Login credentials type based on the authentication method used. */
	EOS_ELoginCredentialType Type;
	/**
	 * This field is for system specific options, if any.
	 *
	 * If provided, the structure will be located in (System)/eos_(system).h.
	 * The structure will be named EOS_(System)_Auth_CredentialsOptions.
	 */
	void* SystemAuthCredentialsOptions;
	/**
	 * Type of external login. Needed to identify the external auth method to use.
	 * Used when login type is set to EOS_LCT_ExternalAuth, ignored for other EOS_ELoginCredentialType methods.
	 */
	EOS_EExternalCredentialType ExternalType;
));

/** The most recent version of the EOS_Auth_PinGrantInfo struct. */
#define EOS_AUTH_PINGRANTINFO_API_LATEST 2

/**
 * Intermediate data for completing Epic account login, when neither the in-game overlay or a platform browser is used.
 * The EOS_Auth_PinGrantInfo struct is returned as part of the EOS_Auth_LoginCallbackInfo and EOS_Auth_LinkAccountCallbackInfo structs.
 * The data inside should be exposed to the user for entry on a secondary device.
 * All data must be copied out before the completion of this callback.
 */
EOS_STRUCT(EOS_Auth_PinGrantInfo, (
	/** API Version: Set this to EOS_AUTH_PINGRANTINFO_API_LATEST. */
	int32_t ApiVersion;
	/** Code the user must input on an external device to activate the login. */
	const char* UserCode;
	/** The end-user verification URI. Users can be asked to manually type this into their browser. */
	const char* VerificationURI;
	/** Time the user has, in seconds, to complete the process or else timeout. */
	int32_t ExpiresIn;
	/** A verification URI that includes the user code. Useful for non-textual transmission. */
	const char* VerificationURIComplete;
));

/** Flags that describe user permissions */
EOS_ENUM(EOS_EAuthScopeFlags,
	EOS_AS_NoFlags = 0x0,
	/** Permissions to see your account ID, display name, and language */
	EOS_AS_BasicProfile = 0x1,
	/** Permissions to see a list of your friends who use this application */
	EOS_AS_FriendsList = 0x2,
	/** Permissions to set your online presence and see presence of your friends */
	EOS_AS_Presence = 0x4,
	/** Permissions to manage the Epic friends list. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures. */
	EOS_AS_FriendsManagement = 0x8,
	/** Permissions to see email in the response when fetching information for a user. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures. */
	EOS_AS_Email = 0x10,
	/** Permissions to see your country */
	EOS_AS_Country = 0x20
);

EOS_ENUM_BOOLEAN_OPERATORS(EOS_EAuthScopeFlags)

/** Optional login flags used in EOS_Auth_Login. */

/**
 * Specify login to be performed without SDK provided user interface.
 *
 * By default, and without this flag, the SDK uses the overlay or a system browser to show user interfaces during login
 * when the user needs to perform some action. With this flag, an error such as EOS_Auth_UserInterfaceRequired is returned
 * to the login callback, and no user interface is shown in those cases.
 */
#define EOS_LF_NO_USER_INTERFACE 0x00001

/** The most recent version of the EOS_Auth_Login API. */
#define EOS_AUTH_LOGIN_API_LATEST 3

/**
 * Input parameters for the EOS_Auth_Login function.
 */
EOS_STRUCT(EOS_Auth_LoginOptions, (
	/** API Version: Set this to EOS_AUTH_LOGIN_API_LATEST. */
	int32_t ApiVersion;
	/** Credentials specified for a given login method. */
	const EOS_Auth_Credentials* Credentials;
	/** Auth scope flags are permissions to request from the user while they are logging in. This is a bitwise-or union of EOS_EAuthScopeFlags flags defined above. */
	EOS_EAuthScopeFlags ScopeFlags;
	/** Optional flags for the desired login behavior, e.g. EOS_LF_NO_USER_INTERFACE. This is a bitwise-or union of the defined flags. */
	uint64_t LoginFlags;
));

/**
 * Output parameters for the EOS_Auth_Login Function.
 */
EOS_STRUCT(EOS_Auth_LoginCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Auth_Login. */
	void* ClientData;
	/** The Epic Account ID of the local user who has logged in. */
	EOS_EpicAccountId LocalUserId;
	/**
	 * Optional data that may be returned in the middle of the login flow, when neither the in-game overlay or a platform browser is used.
	 * This data is present when the ResultCode is EOS_Auth_PinGrantCode.
	 */
	const EOS_Auth_PinGrantInfo* PinGrantInfo;
	/** If the user was not found with external auth credentials passed into EOS_Auth_Login, this continuance token can be passed to EOS_Auth_LinkAccount to continue the flow. */
	EOS_ContinuanceToken ContinuanceToken;
	/** Deprecated field that is no longer used. */
	const EOS_Auth_AccountFeatureRestrictedInfo* AccountFeatureRestrictedInfo_DEPRECATED;
	/**
	 * The Epic Account ID that has been previously selected to be used for the current application.
	 * Applications should use this ID to authenticate with online backend services that store game-scoped data for users.
	 *
	 * Note: This ID may be different from LocalUserId if the user has previously merged Epic accounts into the account
	 * represented by LocalUserId, and one of the accounts that got merged had game data associated with it for the application.
	 */
	EOS_EpicAccountId SelectedAccountId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Auth_Login
 * @param Data A EOS_Auth_LoginCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Auth_OnLoginCallback, const EOS_Auth_LoginCallbackInfo* Data);

/** The most recent version of the EOS_Auth_Logout API. */
#define EOS_AUTH_LOGOUT_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_Logout function.
 */
EOS_STRUCT(EOS_Auth_LogoutOptions, (
	/** API Version: Set this to EOS_AUTH_LOGOUT_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the local user who is being logged out */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Output parameters for the EOS_Auth_Logout Function.
 */
EOS_STRUCT(EOS_Auth_LogoutCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Auth_Login */
	void* ClientData;
	/** The Epic Account ID of the local user requesting the information */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Auth_Logout
 * @param Data A EOS_Auth_LogoutCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Auth_OnLogoutCallback, const EOS_Auth_LogoutCallbackInfo* Data);

/**
 * Flags used to describe how the account linking operation is to be performed.
 *
 * @see EOS_Auth_LinkAccount
 */
EOS_ENUM(EOS_ELinkAccountFlags,
	/**
	 * Default flag used for a standard account linking operation.
	 *
	 * This flag is set when using a continuance token received from a previous call to the EOS_Auth_Login API,
	 * when the local user has not yet been successfully logged in to an Epic Account yet.
	 */
	EOS_LA_NoFlags = 0x0,
	/**
	 * Specified when the EOS_ContinuanceToken describes a Nintendo NSA ID account type.
	 *
	 * This flag is used only with, and must be set, when the continuance token was received from a previous call
	 * to the EOS_Auth_Login API using the EOS_EExternalCredentialType::EOS_ECT_NINTENDO_NSA_ID_TOKEN login type.
	 */
	EOS_LA_NintendoNsaId = 0x1
);

/** The most recent version of the EOS_Auth_LinkAccount API. */
#define EOS_AUTH_LINKACCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_LinkAccount function.
 */
EOS_STRUCT(EOS_Auth_LinkAccountOptions, (
	/** API Version: Set this to EOS_AUTH_LINKACCOUNT_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * Combination of the enumeration flags to specify how the account linking operation will be performed.
	 */
	EOS_ELinkAccountFlags LinkAccountFlags;
	/**
	 * Continuance token received from a previous call to the EOS_Auth_Login API.
	 *
	 * A continuance token is received in the case when the external account used for login was not found to be linked
	 * against any existing Epic Account. In such case, the application needs to proceed with an account linking operation in which case
	 * the user is first asked to create a new account or login into their existing Epic Account, and then link their external account to it.
	 * Alternatively, the application may suggest the user to login using another external account that they have already linked to their existing Epic Account.
	 * In this flow, the external account is typically the currently logged in local platform user account.
	 * It can also be another external user account that the user is offered to login with.
	 */
	EOS_ContinuanceToken ContinuanceToken;
	/**
	 * The Epic Account ID of the logged in local user whose Epic Account will be linked with the local Nintendo NSA ID Account. By default set to NULL.
	 *
	 * This parameter is only used and required to be set when EOS_ELinkAccountFlags::EOS_LA_NintendoNsaId is specified.
	 * Otherwise, set to NULL, as the standard account linking and login flow using continuance token will handle logging in the user to their Epic Account.
	 */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Output parameters for the EOS_Auth_LinkAccount Function.
 */
EOS_STRUCT(EOS_Auth_LinkAccountCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Auth_LinkAccount. */
	void* ClientData;
	/** The Epic Account ID of the local user whose account has been linked during login. */
	EOS_EpicAccountId LocalUserId;
	/**
	 * Optional data that may be returned in the middle of the login flow, when neither the in-game overlay or a platform browser is used.
	 * This data is present when the ResultCode is EOS_Auth_PinGrantCode.
	 */
	const EOS_Auth_PinGrantInfo* PinGrantInfo;
	/**
	 * The Epic Account ID that has been previously selected to be used for the current application.
	 * Applications should use this ID to authenticate with online backend services that store game-scoped data for users.
	 *
	 * Note: This ID may be different from LocalUserId if the user has previously merged Epic accounts into the account
	 * represented by LocalUserId, and one of the accounts that got merged had game data associated with it for the application.
	 */
	EOS_EpicAccountId SelectedAccountId;
));

/**
 * Function prototype definition for callbacks passed to EOS_Auth_LinkAccount
 * @param Data A EOS_Auth_LinkAccountCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Auth_OnLinkAccountCallback, const EOS_Auth_LinkAccountCallbackInfo* Data);

/** The most recent version of the EOS_Auth_VerifyUserAuth API. */
#define EOS_AUTH_VERIFYUSERAUTH_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_VerifyUserAuth function.
 * This operation is destructive, the pointer will remain the same but the data pointers inside will update
 */
EOS_STRUCT(EOS_Auth_VerifyUserAuthOptions, (
	/** API Version: Set this to EOS_AUTH_VERIFYUSERAUTH_API_LATEST. */
	int32_t ApiVersion;
	/** Auth token to verify against the backend service */
	const EOS_Auth_Token* AuthToken;
));

/**
 * Output parameters for the EOS_Auth_VerifyUserAuth Function.
 */
EOS_STRUCT(EOS_Auth_VerifyUserAuthCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Auth_Login */
	void* ClientData;
));

/**
 * Function prototype definition for callbacks passed to EOS_Auth_VerifyUserAuth
 * @param Data A EOS_Auth_VerifyUserAuthCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Auth_OnVerifyUserAuthCallback, const EOS_Auth_VerifyUserAuthCallbackInfo* Data);

/** The most recent version of the EOS_Auth_CopyUserAuthToken API. */
#define EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_CopyUserAuthToken function.
 */
EOS_STRUCT(EOS_Auth_CopyUserAuthTokenOptions, (
	/** API Version: Set this to EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_Auth_CopyIdToken API. */
#define EOS_AUTH_COPYIDTOKEN_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_CopyIdToken function.
 */
EOS_STRUCT(EOS_Auth_CopyIdTokenOptions, (
	/** API Version: Set this to EOS_AUTH_COPYIDTOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the user being queried. */
	EOS_EpicAccountId AccountId;
));

/** The most recent version of the EOS_Auth_IdToken struct. */
#define EOS_AUTH_IDTOKEN_API_LATEST 1

/**
 * A structure that contains an ID token.
 * These structures are created by EOS_Auth_CopyIdToken and must be passed to EOS_Auth_IdToken_Release when finished.
 */
EOS_STRUCT(EOS_Auth_IdToken, (
	/** API Version: Set this to EOS_AUTH_IDTOKEN_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * The Epic Account ID described by the ID token.
	 * Use EOS_EpicAccountId_FromString to populate this field when validating a received ID token.
	 */
	EOS_EpicAccountId AccountId;
	/** The ID token as a Json Web Token (JWT) string. */
	const char* JsonWebToken;
));

/**
 * Release the memory associated with an EOS_Auth_IdToken structure. This must be called on data retrieved from EOS_Auth_CopyIdToken.
 *
 * @param IdToken The ID token structure to be released.
 *
 * @see EOS_Auth_IdToken
 * @see EOS_Auth_CopyIdToken
 */
EOS_DECLARE_FUNC(void) EOS_Auth_IdToken_Release(EOS_Auth_IdToken* IdToken);

/** The most recent version of the EOS_Auth_QueryIdToken API. */
#define EOS_AUTH_QUERYIDTOKEN_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_QueryIdToken function.
 */
EOS_STRUCT(EOS_Auth_QueryIdTokenOptions, (
	/** API Version: Set this to EOS_AUTH_QUERYIDTOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the local authenticated user. */
	EOS_EpicAccountId LocalUserId;
	/**
	 * The target Epic Account ID for which to query an ID token.
	 * This account id may be the same as the input LocalUserId or another merged account id associated with the local user's Epic account.
	 *
	 * An ID token for the selected account id of a locally authenticated user will always be readily available.
	 * To retrieve it for the selected account ID, you can use EOS_Auth_CopyIdToken directly after a successful user login.
	 */
	EOS_EpicAccountId TargetAccountId;
));

/**
 * Output parameters for the EOS_Auth_QueryIdToken Function.
 */
EOS_STRUCT(EOS_Auth_QueryIdTokenCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Auth_QueryIdToken */
	void* ClientData;
	/** The Epic Account ID of the local authenticated user. */
	EOS_EpicAccountId LocalUserId;
	/** The target Epic Account ID for which the ID token was retrieved. */
	EOS_EpicAccountId TargetAccountId;
));

EOS_DECLARE_CALLBACK(EOS_Auth_OnQueryIdTokenCallback, const EOS_Auth_QueryIdTokenCallbackInfo* Data);

/** The most recent version of the EOS_Auth_VerifyIdToken API. */
#define EOS_AUTH_VERIFYIDTOKEN_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_VerifyIdToken function.
 */
EOS_STRUCT(EOS_Auth_VerifyIdTokenOptions, (
	/** API Version: Set this to EOS_AUTH_VERIFYIDTOKEN_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * The ID token to verify.
	 * Use EOS_EpicAccountId_FromString to populate the AccountId field of this struct.
	 */
	const EOS_Auth_IdToken* IdToken;
));

/**
 * Output parameters for the EOS_Auth_VerifyIdToken Function.
 */
EOS_STRUCT(EOS_Auth_VerifyIdTokenCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Auth_VerifyIdToken */
	void* ClientData;
	/**
	 * Epic Account Services Application ID.
	 */
	const char* ApplicationId;
	/**
	 * Client ID of the authorized client.
	 */
	const char* ClientId;
	/**
	 * Product ID.
	 */
	const char* ProductId;
	/**
	 * Sandbox ID.
	 */
	const char* SandboxId;
	/**
	 * Deployment ID.
	 */
	const char* DeploymentId;
	/**
	 * Epic Account display name.
	 *
	 * This value may be set to an empty string.
	 */
	const char* DisplayName;
	/**
	 * Flag set to indicate whether external account information is present.
	 * Applications must always first check this value to be set before attempting
	 * to read the ExternalAccountIdType, ExternalAccountId, ExternalAccountDisplayName and Platform fields.
	 *
	 * This flag is set when the user has logged in to their Epic Account using external account credentials, e.g. through local platform authentication.
	 */
	EOS_Bool bIsExternalAccountInfoPresent;
	/**
	 * The identity provider that the user logged in with to their Epic Account.
	 *
	 * If bIsExternalAccountInfoPresent is set, this field describes the external account type.
	 */
	EOS_EExternalAccountType ExternalAccountIdType;
	/**
	 * The external account ID of the logged in user.
	 *
	 * This value may be set to an empty string.
	 */
	const char* ExternalAccountId;
	/**
	 * The external account display name.
	 *
	 * This value may be set to an empty string.
	 */
	const char* ExternalAccountDisplayName;
	/**
	 * Platform that the user is connected from.
	 *
	 * This value may be set to an empty string.
	 */
	const char* Platform;
));

/**
 * Function prototype definition for callbacks passed into EOS_Auth_VerifyIdToken.
 *
 * @param Data A EOS_Auth_VerifyIdTokenCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Auth_OnVerifyIdTokenCallback, const EOS_Auth_VerifyIdTokenCallbackInfo* Data);

/** The most recent version of the EOS_Auth_AddNotifyLoginStatusChanged API. */
#define EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST 1

/**
 * Input parameters for the EOS_Auth_AddNotifyLoginStatusChanged Function.
 */
EOS_STRUCT(EOS_Auth_AddNotifyLoginStatusChangedOptions, (
	/** API Version: Set this to EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_Auth_DeletePersistentAuth API. */
#define EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST 2

/**
 * Input parameters for the EOS_Auth_DeletePersistentAuth function.
 */
EOS_STRUCT(EOS_Auth_DeletePersistentAuthOptions, (
	/** API Version: Set this to EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * A long-lived refresh token that is used with the EOS_LCT_PersistentAuth login type and is to be revoked from the authentication server. Only used on Console platforms.
	 * On Desktop and Mobile platforms, set this parameter to NULL.
	 */
	const char* RefreshToken;
));

/**
 * Output parameters for the EOS_Auth_DeletePersistentAuth Function.
 */
EOS_STRUCT(EOS_Auth_DeletePersistentAuthCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Auth_DeletePersistentAuth */
	void* ClientData;
));

/**
 * Function prototype definition for callbacks passed to EOS_Auth_DeletePersistentAuth
 * @param Data A EOS_Auth_DeletePersistentAuthCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Auth_OnDeletePersistentAuthCallback, const EOS_Auth_DeletePersistentAuthCallbackInfo* Data);

/**
 * Output parameters for the EOS_Auth_OnLoginStatusChangedCallback Function.
 */
EOS_STRUCT(EOS_Auth_LoginStatusChangedCallbackInfo, (
	/** Context that was passed into EOS_Auth_AddNotifyLoginStatusChanged */
	void* ClientData;
	/** The Epic Account ID of the local user whose status has changed */
	EOS_EpicAccountId LocalUserId;
	/** The status prior to the change */
	EOS_ELoginStatus PrevStatus;
	/** The status at the time of the notification */
	EOS_ELoginStatus CurrentStatus;
));

/**
 * Function prototype definition for notifications that come from EOS_Auth_AddNotifyLoginStatusChanged
 *
 * @param Data A EOS_Auth_LoginStatusChangedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Auth_OnLoginStatusChangedCallback, const EOS_Auth_LoginStatusChangedCallbackInfo* Data);

#pragma pack(pop)
