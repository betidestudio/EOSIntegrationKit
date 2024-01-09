// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_integratedplatform_types.h"

/**
 * To add integrated platforms, you must call EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer to create an integrated platform options container. To modify that handle, call
 * EOS_IntegratedPlatformOptionsContainer_* methods. Once you are finished, call EOS_Platform_Create with your handle. You must then release your integrated platform options container
 * handle by calling EOS_IntegratedPlatformOptionsContainer_Release.
 */

/**
 * Adds an integrated platform options to the container.
 *
 * @param Options Object containing properties related to setting a user's Status
 * @return Success if modification was added successfully, otherwise an error code related to the problem
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatformOptionsContainer_Add(EOS_HIntegratedPlatformOptionsContainer Handle, const EOS_IntegratedPlatformOptionsContainer_AddOptions* InOptions);

/**
 * Sets the current login status of a specific local platform user to a new value.
 *
 * This function may only be used with an Integrated Platform initialized with the EOS_IPMF_ApplicationManagedIdentityLogin flag, otherwise
 * calls will return EOS_InvalidState and a platform user's login status will be controlled by OS events.
 *
 * If the login status of a user changes, a Integrated Platform User Login Status Changed notification will fire, and depending on the state
 * of the user's login and the platform, the EOS SDK might start fetching data for the user, it may clear cached data, or it may do nothing.
 *
 * If the login status of a user is not different from a previous call to this function, the function will do nothing and return EOS_Success.
 * This will not trigger a call to the Integrated Platform User Login Status Changed.
 *
 * @param Options
 * @return EOS_Success if the call was successful
 *         EOS_NotConfigured if the Integrated Platform was not initialized on platform creation
 *         EOS_InvalidState if the Integrated Platform was not initialized with the EOS_IPMF_ApplicationManagedIdentityLogin flag
 *         EOS_InvalidUser if the LocalPlatformUserId is not a valid user id for the provided Integrated Platform
 *         EOS_InvalidParameters if any other input was invalid
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_SetUserLoginStatus(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_SetUserLoginStatusOptions* Options);

/**
 * Register to receive notifications when the login state of Integrated Platform users change.
 *
 * This notification will trigger any time the EOS SDK's internal login state changes for a user, including for manual login state
 * changes (when the EOS_IPMF_ApplicationManagedIdentityLogin flag is set), or automatically detected ones (when not disabled by the
 * EOS_IPMF_ApplicationManagedIdentityLogin flag).
 *
 * @param Options Data associated with what version of the notification to receive.
 * @param ClientData A context pointer that is returned in the callback function.
 * @param CallbackFunction The function that is called when Integrated Platform user logins happen
 * @return A valid notification that can be used to unregister for notifications, or EOS_INVALID_NOTIFICATIONID if input was invalid.
 *
 * @see EOS_IntegratedPlatform_RemoveNotifyUserLoginStatusChanged
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_IntegratedPlatform_AddNotifyUserLoginStatusChanged(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_AddNotifyUserLoginStatusChangedOptions* Options, void* ClientData, const EOS_IntegratedPlatform_OnUserLoginStatusChangedCallback CallbackFunction);

/**
 * Unregister from Integrated Platform user login and logout notifications.
 *
 * @param NotificationId The NotificationId that was returned from registering for Integrated Platform user login and logout notifications.
 *
 * @see EOS_IntegratedPlatform_AddNotifyUserLoginStatusChanged
 */
EOS_DECLARE_FUNC(void) EOS_IntegratedPlatform_RemoveNotifyUserLoginStatusChanged(EOS_HIntegratedPlatform Handle, EOS_NotificationId NotificationId);

/**
 * Sets the integrated platform user logout handler for all integrated platforms.
 *
 * There can only be one handler set at once, attempting to set a handler when one is already set will result in a EOS_AlreadyConfigured error.
 *
 * This callback handler allows applications to decide if a user is logged-out immediately when the SDK receives a system user logout event,
 * or if the application would like to give the user a chance to correct themselves and log back in if they are in a state that might be
 * disruptive if an accidental logout happens (unsaved user data, in a multiplayer match, etc). This is not supported on all integrated
 * platforms, such as those where applications automatically close when a user logs out, or those where a user is always logged-in.
 *
 * If a logout is deferred, applications are expected to eventually call EOS_IntegratedPlatform_FinalizeDeferredUserLogout when they
 * have decided a user meant to logout, or if they have logged in again.
 *
 * @param Options Data that specifies the API version.
 * @param ClientData An optional context pointer that is returned in the callback data.
 * @param CallbackFunction The function that will handle the callback.
 * @return EOS_Success if the platform user logout handler was bound successfully.
 *		   EOS_AlreadyConfigured if there is already a platform user logout handler bound.
 *
 * @see EOS_IntegratedPlatform_ClearUserPreLogoutCallback
 * @see EOS_IntegratedPlatform_FinalizeDeferredUserLogout
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_SetUserPreLogoutCallback(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_SetUserPreLogoutCallbackOptions* Options, void* ClientData, EOS_IntegratedPlatform_OnUserPreLogoutCallback CallbackFunction);

/**
 * Clears a previously set integrated platform user logout handler for the specified integrated platform. If none is set for the specified platform, this does nothing.
 *
 * If there are any pending deferred user-logouts when a handler is cleared, those users will internally be logged-out and cached data about those users cleared before this function returns.
 * Any applicable callbacks about those users being logged-out will occur in a future call to EOS_Platform_Tick().
 *
 * @param Options Data for which integrated platform to no longer call a previously-registered callback for.
 *
 * @see EOS_IntegratedPlatform_SetUserPreLogoutCallback
 */
EOS_DECLARE_FUNC(void) EOS_IntegratedPlatform_ClearUserPreLogoutCallback(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_ClearUserPreLogoutCallbackOptions* Options);

/**
 * Complete a Logout/Login for a previously deferred Integrated Platform User Logout.
 *
 * This function allows applications to control whether an integrated-platform user actually logs out when an integrated platform's system tells the SDK a user has been logged-out.
 * This allows applications to prevent accidental logouts from destroying application user state. If a user did not mean to logout, the application should prompt and confirm whether
 * the user meant to logout, and either wait for them to confirm they meant to, or wait for them to login again, before calling this function.
 *
 * If the sign-out is intended and your application believes the user is still logged-out, the UserExpectedLoginState in Options should be EOS_LS_NotLoggedIn.
 * If the sign-out was NOT intended and your application believes the user has logged-in again, the UserExpectedLoginState in Options should be EOS_LS_LoggedIn.
 *
 * @param Options Data for which integrated platform and user is now in the expected logged-in/logged-out state.
 * @return EOS_Success if the platform user state matches the UserExpectedLoginState internally.
 *         EOS_NotConfigured if the Integrated Platform was not initialized on platform creation
 *         EOS_InvalidUser if the LocalPlatformUserId is not a valid user id for the provided Integrated Platform, or if there is no deferred logout waiting to be completed for this specified user
 *         EOS_InvalidParameters if any other input was invalid
 *
 * @see EOS_IntegratedPlatform_SetUserPreLogoutCallback
 * @see EOS_IntegratedPlatform_ClearUserPreLogoutCallback
 * @see EOS_IntegratedPlatform_AddNotifyUserLoginStatusChanged
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_IntegratedPlatform_FinalizeDeferredUserLogout(EOS_HIntegratedPlatform Handle, const EOS_IntegratedPlatform_FinalizeDeferredUserLogoutOptions* Options);
