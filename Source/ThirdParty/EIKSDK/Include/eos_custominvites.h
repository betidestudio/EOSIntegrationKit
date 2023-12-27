// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_custominvites_types.h"

/**
 * The Custom Invites Interface is designed to allow developers to have custom game Invite and Join operations driven by the Notification Service and supported by the Overlay (if desired).
 * All Custom Invites Interface calls take a handle of type EOS_HCustomInvites as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetCustomInvitesInterface function.
 *
 * @see EOS_Platform_GetCustomInvitesInterface
 */

/**
 * Initializes a Custom Invite with a specified payload in preparation for it to be sent to another user or users.
 *
 * @param Options Structure containing information about the request.
 *
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidParameters if any of the options values are incorrect
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_CustomInvites_SetCustomInvite(EOS_HCustomInvites Handle, const EOS_CustomInvites_SetCustomInviteOptions* Options);

/**
 * Sends a Custom Invite that has previously been initialized via SetCustomInvite to a group of users.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error
 *
 * @return EOS_Success if the query completes successfully
 *         EOS_InvalidParameters if any of the options values are incorrect
 *         EOS_TooManyRequests if the number of allowed queries is exceeded
 *         EOS_NotFound if SetCustomInvite has not been previously successfully called for this user
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_SendCustomInvite(EOS_HCustomInvites Handle, const EOS_CustomInvites_SendCustomInviteOptions* Options, void* ClientData, const EOS_CustomInvites_OnSendCustomInviteCallback CompletionDelegate);

/**
 * Register to receive notifications when a Custom Invite for any logged in local user is received
 * @note must call EOS_CustomInvites_RemoveNotifyCustomInviteReceived to remove the notification
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a Custom Invite is received.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyCustomInviteReceived(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions* Options, void* ClientData, const EOS_CustomInvites_OnCustomInviteReceivedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a Custom Invite for any logged in local user is received
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyCustomInviteReceived(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Custom Invite for any logged in local user is accepted via the Social Overlay
 * Invites accepted in this way still need to have FinalizeInvite called on them after you have finished processing the invite accept (e.g. after joining the game)
 * @note must call EOS_CustomInvites_RemoveNotifyCustomInviteAccepted to remove the notification
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a Custom Invite is accepted via the Social Overlay.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyCustomInviteAccepted(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions* Options, void* ClientData, const EOS_CustomInvites_OnCustomInviteAcceptedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a Custom Invite for any logged in local user is accepted via the Social Overlay
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyCustomInviteAccepted(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Custom Invite for any logged in local user is rejected via the Social Overlay
 * Invites rejected in this way do not need to have FinalizeInvite called on them, it is called automatically internally by the SDK.
 * @note must call EOS_CustomInvites_RemoveNotifyCustomInviteRejected to remove the notification
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a Custom Invite is rejected via the Social Overlay.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyCustomInviteRejected(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions* Options, void* ClientData, const EOS_CustomInvites_OnCustomInviteRejectedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a Custom Invite for any logged in local user is rejected via the Social Overlay
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyCustomInviteRejected(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Signal that the title has completed processing a received Custom Invite, and that it should be cleaned up internally and in the Overlay
 *
 * @param Options Structure containing information about the request.
 *
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidParameters if any of the option values are incorrect
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_CustomInvites_FinalizeInvite(EOS_HCustomInvites Handle, const EOS_CustomInvites_FinalizeInviteOptions* Options);

/**
 * Request that another user send an invitation.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error
 *
 * @return EOS_Success if the query completes successfully
 *         EOS_InvalidParameters if any of the options values are incorrect
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_SendRequestToJoin(EOS_HCustomInvites Handle, const EOS_CustomInvites_SendRequestToJoinOptions* Options, void* ClientData, const EOS_CustomInvites_OnSendRequestToJoinCallback CompletionDelegate);

/**
 * Register to receive notifications when a request to join is responded to by a target user. Note that there is no guarantee a response will be received for every request to join.
 * A player is free to ignore a Request to Join until it expires at which point it will be deleted without sending a response.
 * @note must call EOS_CustomInvites_RemoveNotifyRequestToJoinResponseReceived to remove the notification
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a response is received for an invite request.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifyRequestToJoinResponseReceivedOptions* Options, void* ClientData, const EOS_CustomInvites_OnRequestToJoinResponseReceivedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a request to join for any logged in local user is received
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinResponseReceived(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a request to join is received for a local user
 * @note must call EOS_CustomInvites_RemoveNotifyRequestToJoinReceived to remove the notification
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a response is received for an invite request.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinReceived(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifyRequestToJoinReceivedOptions* Options, void* ClientData, const EOS_CustomInvites_OnRequestToJoinReceivedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a request to join for any logged in local user is received
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinReceived(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications about a custom invite "INVITE" performed by a local user via the overlay.
 * This is only needed when a configured integrated platform has EOS_IPMF_DisableSDKManagedSessions set.  The EOS SDK will
 * then use the state of EOS_IPMF_PreferEOSIdentity and EOS_IPMF_PreferIntegratedIdentity to determine when the NotificationFn is
 * called.
 *
 * @note must call EOS_CustomInvites_RemoveNotifySendCustomNativeInviteRequested to remove the notification.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a notification is received.
 *
 * @return handle representing the registered callback
 *
 * @see EOS_IPMF_DisableSDKManagedSessions
 * @see EOS_IPMF_PreferEOSIdentity
 * @see EOS_IPMF_PreferIntegratedIdentity
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifySendCustomNativeInviteRequestedOptions* Options, void* ClientData, const EOS_CustomInvites_OnSendCustomNativeInviteRequestedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a user requests a send invite via the overlay.
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifySendCustomNativeInviteRequested(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Request to Join for any logged in local user is accepted via the Social Overlay
 * @note must call EOS_CustomInvites_RemoveNotifyRequestToJoinAccepted to remove the notification
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a Request to Join is accepted via the Social Overlay.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinAccepted(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifyRequestToJoinAcceptedOptions* Options, void* ClientData, const EOS_CustomInvites_OnRequestToJoinAcceptedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a Request to Join for any logged in local user is accepted via the Social Overlay
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinAccepted(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Request to Join for any logged in local user is rejected via the Social Overlay
 * @note must call EOS_CustomInvites_RemoveNotifyRequestToJoinRejected to remove the notification
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a Request to Join is accepted via the Social Overlay.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_CustomInvites_AddNotifyRequestToJoinRejected(EOS_HCustomInvites Handle, const EOS_CustomInvites_AddNotifyRequestToJoinRejectedOptions* Options, void* ClientData, const EOS_CustomInvites_OnRequestToJoinRejectedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a Request to Join for any logged in local user is rejected via the Social Overlay
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RemoveNotifyRequestToJoinRejected(EOS_HCustomInvites Handle, EOS_NotificationId InId);

/**
 * Accept a request to join from another user
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error
 *
 * @return EOS_Success if the query completes successfully
 *         EOS_InvalidParameters if any of the options values are incorrect
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_AcceptRequestToJoin(EOS_HCustomInvites Handle, const EOS_CustomInvites_AcceptRequestToJoinOptions* Options, void* ClientData, const EOS_CustomInvites_OnAcceptRequestToJoinCallback CompletionDelegate);

/**
 * Reject a request to join from another user
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error
 *
 * @return EOS_Success if the query completes successfully
 *         EOS_InvalidParameters if any of the options values are incorrect
 */
EOS_DECLARE_FUNC(void) EOS_CustomInvites_RejectRequestToJoin(EOS_HCustomInvites Handle, const EOS_CustomInvites_RejectRequestToJoinOptions* Options, void* ClientData, const EOS_CustomInvites_OnRejectRequestToJoinCallback CompletionDelegate);
