// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

typedef struct EOS_IntegratedPlatformOptionsContainerHandle* EOS_HIntegratedPlatformOptionsContainer;
typedef struct EOS_IntegratedPlatformHandle* EOS_HIntegratedPlatform;

/** These flags are used to determine how a specific Integrated Platform will be managed. */
EOS_ENUM(EOS_EIntegratedPlatformManagementFlags,
	/** The integrated platform library should be disabled. This is equivalent to providing no flags. */
	EOS_IPMF_Disabled = 0x0001,
	/** The integrated platform library is managed by the calling application. EOS SDK should only hook into an existing instance of the integrated platform library. */
	EOS_IPMF_LibraryManagedByApplication = 0x0002,
	/** EOS SDK should fully manage the integrated platform library. It will do this by performing the load, initialize, tick and unload operations as necessary. */
	EOS_IPMF_LibraryManagedBySDK = 0x0004,
	/**
	 * The EOS SDK should not mirror the EOS rich presence with the Integrated Platform.
	 * The default behavior is for EOS SDK to share local presence with the Integrated Platform.
	 */
	EOS_IPMF_DisablePresenceMirroring = 0x0008,
	/**
	 * EOS SDK should not perform any sessions management through the Integrated Platform.
	 * The default behavior is for EOS SDK to perform sessions management through the Integrated Platform.
	 * Sessions management includes:
	 *    - sharing the lobby and session presence enabled games with the Integrated Platform.
	 *    - handling Social Overlay join button events which cannot be handled by normal processing of Epic Services.
	 *    - handling Social Overlay invite button events which cannot be handled by normal processing of Epic Services.
	 *    - handling startup requests from the Integrated Platform to immediately join a game due to in invite while offline.
	 *
	 * @see EOS_Lobby_AddNotifySendLobbyNativeInviteRequested
	 */
	EOS_IPMF_DisableSDKManagedSessions = 0x0010,
	/**
	 * Some features within the EOS SDK may wish to know a preference of Integrated Platform versus EOS.
	 * When determining an absolute platform preference those with this flag will be skipped.
	 * The IntegratedPlatforms list is provided via the EOS_Platform_Options during EOS_Platform_Create.
	 *
	 * The primary usage of the EOS_IPMF_PreferEOSIdentity and EOS_IPMF_PreferIntegratedIdentity flags is with game invites
	 * from the Social Overlay.
	 *
	 * For game invites from the Social Overlay the EOS SDK will follow these rules:
	 *     - If the only account ID we can determine for the target player is an EAS ID then the EOS system will be used.
	 *     - If the only account ID we can determine for the target player is an integrated platform ID then the integrated platform system will be used.
	 *     - If both are available then the EOS SDK will operate in 1 of 3 modes:
	 *         - no preference identified: use both the EOS and integrated platform systems.
	 *         - PreferEOS: Use EOS if the target is an EAS friend and is either online in EAS or not online for the integrated platform.
	 *         - PreferIntegrated: Use integrated platform if the target is an integrated platform friend and is either online in the integrated platform or not online for EAS.
	 *     - If the integrated platform fails to send then try EAS if was not already used.
	 */
	EOS_IPMF_PreferEOSIdentity = 0x0020,
	/**
	 * Some features within the EOS SDK may wish to know a preference of Integrated Platform versus EOS.
	 * For further explanation see EOS_IPMF_PreferEOSIdentity.
	 *
	 * @see EOS_IPMF_PreferEOSIdentity
	 */
	EOS_IPMF_PreferIntegratedIdentity = 0x0040,
	/**
	 * By default the EOS SDK will attempt to detect the login/logout events of local users and update local states accordingly. Setting this flag will disable this functionality,
	 * relying on the application to process login/logout events and notify EOS SDK. It is not possible for the EOS SDK to do this on all platforms, making this flag not always
	 * optional.
	 *
	 * This flag must be set to use the manual platform user login/logout functions, even on platforms where it is not possible for the EOS SDK to detect login/logout events,
	 * making this a required flag for correct Integrated Platform behavior on those platforms.
	 */
	EOS_IPMF_ApplicationManagedIdentityLogin = 0x0080
);
EOS_ENUM_BOOLEAN_OPERATORS(EOS_EIntegratedPlatformManagementFlags);

/** A macro to identify the Steam integrated platform. */
#define EOS_IPT_Steam "STEAM"

#define EOS_INTEGRATEDPLATFORM_OPTIONS_API_LATEST 1

/**
 */
EOS_STRUCT(EOS_IntegratedPlatform_Options, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_OPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** The type to be initialized. */
	EOS_IntegratedPlatformType Type;
	/** Identifies how to initialize the IntegratedPlatform. */
	EOS_EIntegratedPlatformManagementFlags Flags;
	/**
	 * Options specific to this integrated platform type.
	 * This parameter is either required or set to NULL based on the platform type.
	 *
	 * @see EOS_IntegratedPlatform_Steam_Options
	 */
	const void* InitOptions;
));

#define EOS_INTEGRATEDPLATFORM_STEAM_OPTIONS_API_LATEST 2

/**
 * Required initialization options to use with EOS_IntegratedPlatform_Options for Steam.
 * Steamworks API needs to be at least v1.13
 * Steam Sanitization requires at least v1.45
 *
 * @see EOS_IntegratedPlatform_Options
 */
EOS_STRUCT(EOS_IntegratedPlatform_Steam_Options, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_STEAM_OPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * Usage of this parameter is dependent on the specified EOS_EIntegratedPlatformManagementFlags.
	 *
	 * Optional with EOS_IPMF_LibraryManagedByApplication.
	 * Set to override the loaded library basename, or use NULL to assume the default basename by platform:
	 *
	 * - Linux: libsteam_api.so,
	 * - macOS: libsteam_api.dylib,
	 * - Windows 32-bit: steam_api.dll,
	 * - Windows 64-bit: steam_api64.dll.
	 *
	 * Required with EOS_IPMF_LibraryManagedBySDK.
	 * Set to a fully qualified file path to the Steamworks SDK runtime library on disk.
	 */
	const char* OverrideLibraryPath;
	/**
	 * Used to specify the major version of the Steam SDK your game is compiled against, e.g.:
	 *
	 * Options.SteamMajorVersion = 1;
	 */
	uint32_t SteamMajorVersion;
	/**
	 * Used to specify the minor version of the Steam SDK your game is compiled against, e.g.:
	 *
	 * Options.SteamMinorVersion = 57;
	 */
	uint32_t SteamMinorVersion;
));

#define EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST 1

/**
 * Data for the EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer function.
 */
EOS_STRUCT(EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Creates an integrated platform options container handle. This handle can used to add multiple options to your container which will then be applied with EOS_Platform_Create.
 * The resulting handle must be released by calling EOS_IntegratedPlatformOptionsContainer_Release once it has been passed to EOS_Platform_Create.
 *
 * @param Options structure containing operation input parameters.
 * @param OutIntegratedPlatformOptionsContainerHandle Pointer to an integrated platform options container handle to be set if successful.
 * @return Success if we successfully created the integrated platform options container handle pointed at in OutIntegratedPlatformOptionsContainerHandle, or an error result if the input data was invalid.
 *
 * @see EOS_IntegratedPlatformOptionsContainer_Release
 * @see EOS_Platform_Create
 * @see EOS_IntegratedPlatformOptionsContainer_Add
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer(const EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions* Options, EOS_HIntegratedPlatformOptionsContainer* OutIntegratedPlatformOptionsContainerHandle);

/**
 * Release the memory associated with an EOS_HIntegratedPlatformOptionsContainer handle. This must be called on Handles retrieved from EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer.
 * This can be safely called on a NULL integrated platform options container handle.
 *
 * @param IntegratedPlatformOptionsContainerHandle The integrated platform options container handle to release.
 *
 * @see EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer
 */
EOS_DECLARE_FUNC(void) EOS_IntegratedPlatformOptionsContainer_Release(EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle);


#define EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST 1

/**
 * Data for the EOS_IntegratedPlatformOptionsContainer_Add function.
 */
EOS_STRUCT(EOS_IntegratedPlatformOptionsContainer_AddOptions, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST. */
	int32_t ApiVersion;
	/** The integrated platform options to add. */
	const EOS_IntegratedPlatform_Options* Options;
));

/** The most recent version of the EOS_IntegratedPlatform_SetUserLoginStatus API. */
#define EOS_INTEGRATEDPLATFORM_SETUSERLOGINSTATUS_API_LATEST 1

/**
 * Input parameters for the EOS_IntegratedPlatform_SetUserLoginStatus function.
 */
EOS_STRUCT(EOS_IntegratedPlatform_SetUserLoginStatusOptions, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_SETUSERLOGINSTATUS_API_LATEST. */
	int32_t ApiVersion;

	/** The integrated platform this user belongs to. */
	EOS_IntegratedPlatformType PlatformType;
	
	/** String version of the integrated platform-dependent user id. */
	const char* LocalPlatformUserId;
	
	/** The login status of the provided user */
	EOS_ELoginStatus CurrentLoginStatus;
));


/** The most recent version of the EOS_IntegratedPlatform_AddNotifyUserLoginStatusChanged API. */
#define EOS_INTEGRATEDPLATFORM_ADDNOTIFYUSERLOGINSTATUSCHANGED_API_LATEST 1

/**
 * Input parameters for the EOS_IntegratedPlatform_AddNotifyUserLoginStatusChanged function.
 */
EOS_STRUCT(EOS_IntegratedPlatform_AddNotifyUserLoginStatusChangedOptions, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_ADDNOTIFYUSERLOGINSTATUSCHANGED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Data about which integrated platform and which user that had a login status change and what the login status changed to.
 */
EOS_STRUCT(EOS_IntegratedPlatform_UserLoginStatusChangedCallbackInfo, (
	/** Context that was passed into EOS_IntegratedPlatform_AddNotifyUserLoginStatusChanged */
	void* ClientData;
	/** The integrated platform of the local platform user. */
	EOS_IntegratedPlatformType PlatformType;
	/** String version of platform's user id. */
	const char* LocalPlatformUserId;
	/** The Epic Games Account ID associated with this Integrated Platform's User (if there is one) */
	EOS_EpicAccountId AccountId;
	/** The EOS Product User ID associated with this Integrated Platform's User (if there is one) */
	EOS_ProductUserId ProductUserId;
	/** The login status prior to this change. */
	EOS_ELoginStatus PreviousLoginStatus;
	/** The login status at the time of this notification. */
	EOS_ELoginStatus CurrentLoginStatus;
));
/**
 * The callback function for when a local integrated platform user's login status has changed.
 */
EOS_DECLARE_CALLBACK(EOS_IntegratedPlatform_OnUserLoginStatusChangedCallback, const EOS_IntegratedPlatform_UserLoginStatusChangedCallbackInfo* Data);

/** The most recent version of the EOS_IntegratedPlatform_SetUserPreLogoutCallback API. */
#define EOS_INTEGRATEDPLATFORM_SETUSERPRELOGOUTCALLBACK_API_LATEST 1

/**
 * Input parameters for the EOS_IntegratedPlatform_SetUserPreLogoutCallback function.
 */
EOS_STRUCT(EOS_IntegratedPlatform_SetUserPreLogoutCallbackOptions, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_SETUSERPRELOGOUTCALLBACK_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * The return value for the EOS_IntegratedPlatform_OnUserPreLogoutCallback callback function. This signifies what the application wants to do for
 * the provided user of the integrated platform.
 */
EOS_ENUM(EOS_EIntegratedPlatformPreLogoutAction,
	/**
	 * The application accepts the user being logged-out. all cached data for the user will be cleared immediately and any pending
	 * actions canceled.
	 */
	EOS_IPLA_ProcessLogoutImmediately = 0,
	/**
	 * Instead of the user being logged-out, the SDK will wait for a call to EOS_IntegratedPlatform_FinalizeDeferredUserLogout with the
	 * expected login state of the user. If the expected state matches the current state, the user will continue to be logged-in or they
	 * will be logged-out, depending on the value of the expected state. This lets the application choose to ask the user if they meant
	 * to logout if it wishes, possibly preventing losing any unsaved changes, such as game progress, leaving a multiplayer match, or
	 * similar.
	 *
	 * @see EOS_IntegratedPlatform_FinalizeDeferredUserLogout
	 */
	EOS_IPLA_DeferLogout = 1
);

/**
 * Data passed to the application in the EOS_IntegratedPlatform_OnUserPreLogoutCallback function. This contains which user and associated
 * Integrated Platform that was detected as logged-out.
 */
EOS_STRUCT(EOS_IntegratedPlatform_UserPreLogoutCallbackInfo, (
	/** Context that was passed into EOS_IntegratedPlatform_SetUserPreLogoutCallback  */
	void* ClientData;
	/** The integrated platform the local user logged-out of. */
	EOS_IntegratedPlatformType PlatformType;
	/** String version of platform-dependent user id. */
	const char* LocalPlatformUserId;
	/** The Epic Games Account ID associated with this Integrated Platform's User (if there is one) */
	EOS_EpicAccountId AccountId;
	/** The EOS Product User ID associated with this Integrated Platform's User (if there is one) */
	EOS_ProductUserId ProductUserId;
));
/**
 * The callback function for when an integrated platform user is detected to have logged-out.
 */
EOS_DECLARE_CALLBACK_RETVALUE(EOS_EIntegratedPlatformPreLogoutAction, EOS_IntegratedPlatform_OnUserPreLogoutCallback, const EOS_IntegratedPlatform_UserPreLogoutCallbackInfo* Data);

/** The most recent version of the EOS_IntegratedPlatform_ClearUserPreLogoutCallback API. */
#define EOS_INTEGRATEDPLATFORM_CLEARUSERPRELOGOUTCALLBACK_API_LATEST 1

/**
 * Input parameters for the EOS_IntegratedPlatform_ClearUserPreLogoutCallback function.
 */
EOS_STRUCT(EOS_IntegratedPlatform_ClearUserPreLogoutCallbackOptions, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_CLEARUSERPRELOGOUTCALLBACK_API_LATEST. */
	int32_t ApiVersion;
));


/** The most recent version of the EOS_IntegratedPlatform_FinalizeDeferredUserLogout API. */
#define EOS_INTEGRATEDPLATFORM_FINALIZEDEFERREDUSERLOGOUT_API_LATEST 1

/**
 * Input parameters for the EOS_IntegratedPlatform_FinalizeDeferredUserLogout function.
 */
EOS_STRUCT(EOS_IntegratedPlatform_FinalizeDeferredUserLogoutOptions, (
	/** API Version: Set this to EOS_INTEGRATEDPLATFORM_FINALIZEDEFERREDUSERLOGOUT_API_LATEST. */
	int32_t ApiVersion;

	/** The integrated platform this user belongs to. */
	EOS_IntegratedPlatformType PlatformType;

	/** String version of the integrated platform-dependent user id. */
	const char* LocalPlatformUserId;

	/**
	 * The logged-in state the user is expected to be (EOS_LS_LoggedIn or EOS_LS_NotLoggedIn). If the provided
	 * state does not match internal EOS state, this function will return in failure. If the state is incorrect,
	 * the application should wait and attempt to call the function again next tick, after both updating its own
	 * state from the system and calling EOS_Platform_Tick, allowing the SDK to update its state from the system
	 * as well.
	 */
	EOS_ELoginStatus ExpectedLoginStatus;
));

#pragma pack(pop)
