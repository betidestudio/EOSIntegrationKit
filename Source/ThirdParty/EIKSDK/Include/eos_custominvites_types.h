// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"
#include "eos_ui_types.h"

#pragma pack(push, 8)

/** Handle to the custom invites interface */
EXTERN_C typedef struct EOS_CustomInvitesHandle* EOS_HCustomInvites;

/** Maximum size of the custom invite payload string */
#define EOS_CUSTOMINVITES_MAX_PAYLOAD_LENGTH 500

/** The most recent version of the EOS_CustomInvites_SetCustomInvite API. */
#define EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_SetCustomInviteOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST. */
	int32_t ApiVersion;
	/** Local user creating / sending a Custom Invite */
	EOS_ProductUserId LocalUserId;
	/** String payload for the Custom Invite (must be less than EOS_CUSTOMINVITES_MAX_PAYLOAD_LENGTH) */
	const char* Payload;
));

/** The most recent version of the EOS_CustomInvites_SendCustomInvite API. */
#define EOS_CUSTOMINVITES_SENDCUSTOMINVITE_API_LATEST 1

/**
 * Input parameters for the EOS_CustomInvites_SendCustomInvite function.
 */
EOS_STRUCT(EOS_CustomInvites_SendCustomInviteOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_SENDCUSTOMINVITE_API_LATEST. */
	int32_t ApiVersion;
	/** Local user sending a CustomInvite */
	EOS_ProductUserId LocalUserId;
	/** Users to whom the invites should be sent */
	EOS_ProductUserId* TargetUserIds;
	/** The number of users we are sending to */
	uint32_t TargetUserIdsCount;
));

/**
 * Output parameters for the EOS_CustomInvites_SendCustomInvite Function. These parameters are received through the callback provided to EOS_CustomInvites_SendCustomInvite
 */
EOS_STRUCT(EOS_CustomInvites_SendCustomInviteCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_SendCustomInvite */
	void* ClientData;
	/** Local user sending a CustomInvite */
	EOS_ProductUserId LocalUserId;
	/** Users to whom the invites were successfully sent (can be different than original call if an invite for same Payload was previously sent) */
	EOS_ProductUserId* TargetUserIds;
	/** The number of users we are sending to */
	uint32_t TargetUserIdsCount;
));

/**
 * Function prototype definition for callbacks passed to EOS_CustomInvites_SendCustomInvite
 * @param Data A EOS_CustomInvites_SendCustomInviteCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnSendCustomInviteCallback, const EOS_CustomInvites_SendCustomInviteCallbackInfo* Data);


/** The most recent version of the EOS_CustomInvites_AddNotifyCustomInviteReceived API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITERECEIVED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_CustomInvites_OnCustomInviteReceivedCallback Function.
 */
EOS_STRUCT(EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteReceived */
	void* ClientData;
	/** User that sent this custom invite */
	EOS_ProductUserId TargetUserId;
	/** Recipient Local user id */
	EOS_ProductUserId LocalUserId;
	/** Id of the received Custom Invite*/
	const char* CustomInviteId;
	/** Payload of the received Custom Invite */
	const char* Payload;
));

/**
 * Function prototype definition for notifications that comes from EOS_CustomInvites_AddNotifyCustomInviteReceived
 *
 * @param Data A EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnCustomInviteReceivedCallback, const EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo* Data);


/** The most recent version of the EOS_CustomInvites_AddNotifyCustomInviteAccepted API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEACCEPTED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_CustomInvites_OnCustomInviteAcceptedCallback Function.
 */
EOS_STRUCT(EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteAccepted */
	void* ClientData;
	/** User that sent the custom invite */
	EOS_ProductUserId TargetUserId;
	/** Recipient Local user id */
	EOS_ProductUserId LocalUserId;
	/** Id of the accepted Custom Invite */
	const char* CustomInviteId;
	/** Payload of the accepted Custom Invite */
	const char* Payload;
));

/**
 * Function prototype definition for notifications that comes from EOS_CustomInvites_AddNotifyCustomInviteAccepted
 *
 * @param Data A EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnCustomInviteAcceptedCallback, const EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_AddNotifyCustomInviteRejected API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEREJECTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEREJECTED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_CustomInvites_OnCustomInviteRejectedCallback Function.
 */
EOS_STRUCT(EOS_CustomInvites_CustomInviteRejectedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteRejected */
	void* ClientData;
	/** User that sent the custom invite */
	EOS_ProductUserId TargetUserId;
	/** Recipient Local user id */
	EOS_ProductUserId LocalUserId;
	/** Id of the rejected Custom Invite */
	const char* CustomInviteId;
	/** Payload of the rejected Custom Invite */
	const char* Payload;
));

/**
 * Function prototype definition for notifications that comes from EOS_CustomInvites_AddNotifyCustomInviteRejected
 *
 * @param Data A EOS_CustomInvites_CustomInviteRejectedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnCustomInviteRejectedCallback, const EOS_CustomInvites_CustomInviteRejectedCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_FinalizeInvite API. */
#define EOS_CUSTOMINVITES_FINALIZEINVITE_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_FinalizeInviteOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_FINALIZEINVITE_API_LATEST. */
	int32_t ApiVersion;
	/** User that sent the custom invite */
	EOS_ProductUserId TargetUserId;
	/** Recipient Local user id */
	EOS_ProductUserId LocalUserId;
	/** Id of the Custom Invite accepted */
	const char* CustomInviteId;
	/** Result of the Processing operation, transmitted to Social Overlay if applicable */
	EOS_EResult ProcessingResult;
));

/** The most recent version of the EOS_CustomInvites_SendRequestToJoinOptions API. */
#define EOS_CUSTOMINVITES_SENDREQUESTTOJOIN_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_SendRequestToJoinOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_SENDREQUESTTOJOIN_API_LATEST. */
	int32_t ApiVersion;
	/** Local user Requesting an Invite */
	EOS_ProductUserId LocalUserId;
	/** Recipient of Request Invite*/
	EOS_ProductUserId TargetUserId;
));

/**
 * Output parameters for the EOS_CustomInvites_SendRequestToJoin Function. These parameters are received through the callback provided to EOS_CustomInvites_SendRequestToJoin
 */
EOS_STRUCT(EOS_CustomInvites_SendRequestToJoinCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_SendRequestToJoin */
	void* ClientData;
	/** Local user requesting an invite */
	EOS_ProductUserId LocalUserId;
	/** Recipient of Request Invite*/
	EOS_ProductUserId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_CustomInvites_SendRequestToJoin
 * @param Data A EOS_CustomInvites_SendCustomInviteCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnSendRequestToJoinCallback, const EOS_CustomInvites_SendRequestToJoinCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRESPONSERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinResponseReceivedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRESPONSERECEIVED_API_LATEST. */
	int32_t ApiVersion;
));

/** Response to an invite request. */
EOS_ENUM(EOS_ERequestToJoinResponse,
	/** The target of the invite request has accepted. */
	EOS_RTJR_ACCEPTED = 0,
	/** The target of the invite request has rejected. */
	EOS_RTJR_REJECTED = 1
);

/**
 * Output parameters for the EOS_CustomInvites_OnRequestToJoinResponseReceivedCallback function.
 */
EOS_STRUCT(EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived */
	void* ClientData;
	/** User that sent this response */
	EOS_ProductUserId FromUserId;
	/** Recipient Local user id */
	EOS_ProductUserId ToUserId;
	/** The Intent associated with this response */
	EOS_ERequestToJoinResponse Response;
));

/**
 * Function prototype definition for notifications that come from EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived
 *
 * @param Data A EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinResponseReceivedCallback, const EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo* Data);

/** The most recent version of the AddNotifyRequestToJoinReceived API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRECEIVED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinReceivedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRECEIVED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_CustomInvites_AddNotifyRequestToJoinReceived function.
 */
EOS_STRUCT(EOS_CustomInvites_RequestToJoinReceivedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifyRequestToJoinReceived */
	void* ClientData;
	/** User that sent this response */
	EOS_ProductUserId FromUserId;
	/** Recipient Local user id */
	EOS_ProductUserId ToUserId;
));

/**
 * Function prototype definition for notifications that comes from EOS_CustomInvites_AddNotifyRequestToJoinReceived
 *
 * @param Data A EOS_CustomInvites_RequestToJoinReceivedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinReceivedCallback, const EOS_CustomInvites_RequestToJoinReceivedCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_AcceptRequestToJoin API. */
#define EOS_CUSTOMINVITES_ACCEPTREQUESTTOJOIN_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AcceptRequestToJoinOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ACCEPTREQUESTTOJOIN_API_LATEST. */
	int32_t ApiVersion;
	/** Local user accepting a request to join */
	EOS_ProductUserId LocalUserId;
	/** Target user that sent original request to join */
	EOS_ProductUserId TargetUserId;
));

/**
 * Output parameters for the EOS_CustomInvites_AcceptRequestToJoin Function. These parameters are received through the callback provided to EOS_CustomInvites_AcceptRequestToJoin
 */
EOS_STRUCT(EOS_CustomInvites_AcceptRequestToJoinCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_AcceptRequestToJoin */
	void* ClientData;
	/** Local user accepting an invite request */
	EOS_ProductUserId LocalUserId;
	/** Target user that sent original invite request */
	EOS_ProductUserId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_CustomInvites_AcceptRequestToJoin
 * @param Data A EOS_CustomInvites_AcceptRequestToJoinCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnAcceptRequestToJoinCallback, const EOS_CustomInvites_AcceptRequestToJoinCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_RejectRequestToJoin API. */
#define EOS_CUSTOMINVITES_REJECTREQUESTTOJOIN_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_RejectRequestToJoinOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_REJECTREQUESTTOJOIN_API_LATEST. */
	int32_t ApiVersion;
	/** Local user declining an invite request */
	EOS_ProductUserId LocalUserId;
	/** Target user that sent original invite request */
	EOS_ProductUserId TargetUserId;
));

/**
 * Output parameters for the EOS_CustomInvites_RejectRequestToJoin Function. These parameters are received through the callback provided to EOS_CustomInvites_RejectRequestToJoin
 */
EOS_STRUCT(EOS_CustomInvites_RejectRequestToJoinCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_RejectRequestToJoin */
	void* ClientData;
	/** Local user declining a request to join */
	EOS_ProductUserId LocalUserId;
	/** Target user that sent original request to join */
	EOS_ProductUserId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_CustomInvites_RejectRequestToJoin
 * @param Data A EOS_CustomInvites_OnRejectRequestToJoinCallback containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRejectRequestToJoinCallback, const EOS_CustomInvites_RejectRequestToJoinCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYSENDCUSTOMNATIVEINVITEREQUESTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifySendCustomNativeInviteRequestedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYSENDCUSTOMNATIVEINVITEREQUESTED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_CustomInvites_OnSendCustomNativeInviteRequestedCallback Function.
 */
EOS_STRUCT(EOS_CustomInvites_SendCustomNativeInviteRequestedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested */
	void* ClientData;
	/**
	 * Identifies this event which will need to be acknowledged with EOS_UI_AcknowledgeEventId().
	 * @see EOS_UI_AcknowledgeEventId
	 */
	EOS_UI_EventId UiEventId;
	/** The Product User ID of the local user who is inviting. */
	EOS_ProductUserId LocalUserId;
	/**
	 * The Native Platform Account Type. If only a single integrated platform is configured then
	 * this will always reference that platform.
	 */
	EOS_IntegratedPlatformType TargetNativeAccountType;
	/** The Native Platform Account ID of the target user being invited. */
	const char* TargetUserNativeAccountId;
	/** Invite ID that the user is being invited to */
	const char* InviteId;
));

/**
 * Function prototype definition for notifications that comes from EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested
 *
 * @param Data A EOS_CustomInvites_SendCustomNativeInviteRequestedCallbackInfo containing the output information and result
 *
 * @note After processing the callback EOS_UI_AcknowledgeEventId must be called.
 *
 * @see EOS_UI_AcknowledgeEventId
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnSendCustomNativeInviteRequestedCallback, const EOS_CustomInvites_SendCustomNativeInviteRequestedCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_AddNotifyCustomInviteAccepted API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINACCEPTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinAcceptedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINACCEPTED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_CustomInvites_OnRequestToJoinAcceptedCallback Function.
 */
EOS_STRUCT(EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifyRequestToJoinAccepted */
	void* ClientData;
	/** User that sent the request to join */
	EOS_ProductUserId TargetUserId;
	/** Local user ID of the Request to Join recipient */
	EOS_ProductUserId LocalUserId;
));

/**
 * Function prototype definition for notifications that comes from EOS_CustomInvites_AddNotifyRequestToJoinAccepted
 *
 * @param Data A EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinAcceptedCallback, const EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo* Data);

/** The most recent version of the EOS_CustomInvites_AddNotifyRequestToJoinRejected API. */
#define EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINREJECTED_API_LATEST 1

EOS_STRUCT(EOS_CustomInvites_AddNotifyRequestToJoinRejectedOptions, (
	/** API Version: Set this to EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINREJECTED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_CustomInvites_OnRequestToJoinRejectedCallback Function.
 */
EOS_STRUCT(EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo, (
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteRejected */
	void* ClientData;
	/** User that sent the custom invite */
	EOS_ProductUserId TargetUserId;
	/** Recipient Local user id */
	EOS_ProductUserId LocalUserId;
));

/**
 * Function prototype definition for notifications that comes from EOS_CustomInvites_AddNotifyRequestToJoinRejected
 *
 * @param Data A EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_CustomInvites_OnRequestToJoinRejectedCallback, const EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo* Data);

#pragma pack(pop)
