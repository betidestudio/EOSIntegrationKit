// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EXTERN_C typedef struct EOS_RTCAudioHandle* EOS_HRTCAudio;

/**
 * An enumeration of the different audio channel statuses.
 */
EOS_ENUM(EOS_ERTCAudioStatus,
	/** Audio unsupported by the source (no devices) */
	EOS_RTCAS_Unsupported = 0,
	/** Audio enabled */
	EOS_RTCAS_Enabled = 1,
	/** Audio disabled */
	EOS_RTCAS_Disabled = 2,
	/** Audio disabled by the administrator */
	EOS_RTCAS_AdminDisabled = 3,
	/** Audio channel is disabled temporarily for both sending and receiving */
	EOS_RTCAS_NotListeningDisabled = 4
);

/** The most recent version of the EOS_RTCAudio_AddNotifyParticipantUpdated API. */
#define EOS_RTCAUDIO_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_AddNotifyParticipantUpdated.
 */
EOS_STRUCT(EOS_RTCAudio_AddNotifyParticipantUpdatedOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The  room this event is registered on. */
	const char* RoomName;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_AddNotifyParticipantUpdated registered event.
 */
EOS_STRUCT(EOS_RTCAudio_ParticipantUpdatedCallbackInfo, (
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyParticipantUpdated. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room associated with this event. */
	const char* RoomName;
	/** The participant updated. */
	EOS_ProductUserId ParticipantId;
	/** The participant speaking / non-speaking status. */
	EOS_Bool bSpeaking;
	/** The participant audio status (enabled, disabled). */
	EOS_ERTCAudioStatus AudioStatus;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnParticipantUpdatedCallback, const EOS_RTCAudio_ParticipantUpdatedCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_AddNotifyAudioDevicesChanged API. */
#define EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_AddNotifyAudioDevicesChanged.
 */
EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioDevicesChangedOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioDevicesChanged registered event.
 */
EOS_STRUCT(EOS_RTCAudio_AudioDevicesChangedCallbackInfo, (
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioDevicesChanged. */
	void* ClientData;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioDevicesChangedCallback, const EOS_RTCAudio_AudioDevicesChangedCallbackInfo* Data);

/**
 * An enumeration of the different audio input device statuses.
 */
EOS_ENUM(EOS_ERTCAudioInputStatus,
	/** The device is not in used right now (e.g: you are alone in the room). In such cases, the hardware resources are not allocated. */
	EOS_RTCAIS_Idle = 0,
	/** The device is being used and capturing audio */
	EOS_RTCAIS_Recording = 1,
	/**
	 * The SDK is in a recording state, but actually capturing silence because the device is exclusively being used by the platform at the moment.
	 * This only applies to certain platforms.
	 */
	EOS_RTCAIS_RecordingSilent = 2,
	/**
	 * The SDK is in a recording state, but actually capturing silence because the device is disconnected (e.g: the microphone is not plugged in).
	 * This only applies to certain platforms.
	 */
	EOS_RTCAIS_RecordingDisconnected = 3,
	/** Something failed while trying to use the device */
	EOS_RTCAIS_Failed = 4
);

/** The most recent version of the EOS_RTCAudio_AddNotifyAudioInputState API. */
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOINPUTSTATE_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_AddNotifyAudioInputState.
 */
EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioInputStateOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_ADDNOTIFYAUDIOINPUTSTATE_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this event is registered on. */
	const char* RoomName;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioInputState registered event.
 */
EOS_STRUCT(EOS_RTCAudio_AudioInputStateCallbackInfo, (
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioInputState. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room associated with this event. */
	const char* RoomName;
	/** The status of the audio input. */
	EOS_ERTCAudioInputStatus Status;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioInputStateCallback, const EOS_RTCAudio_AudioInputStateCallbackInfo* Data);

/**
 * An enumeration of the different audio output device statuses.
 */
EOS_ENUM(EOS_ERTCAudioOutputStatus,
	/** The device is not in used right now (e.g: you are alone in the room). In such cases, the hardware resources are not allocated. */
	EOS_RTCAOS_Idle = 0,
	/** Device is in use */
	EOS_RTCAOS_Playing = 1,
	/** Something failed while trying to use the device */
	EOS_RTCAOS_Failed = 2
);

/** The most recent version of the EOS_RTCAudio_AddNotifyAudioOutputState API. */
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOOUTPUTSTATE_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_AddNotifyAudioOutputState.
 */
EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioOutputStateOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_ADDNOTIFYAUDIOOUTPUTSTATE_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The  room this event is registered on. */
	const char* RoomName;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioOutputState registered event.
 */
EOS_STRUCT(EOS_RTCAudio_AudioOutputStateCallbackInfo, (
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioOutputState. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room associated with this event. */
	const char* RoomName;
	/** The status of the audio output. */
	EOS_ERTCAudioOutputStatus Status;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioOutputStateCallback, const EOS_RTCAudio_AudioOutputStateCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_AddNotifyAudioBeforeSend API. */
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORESEND_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_AddNotifyAudioBeforeSend.
 */
EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioBeforeSendOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORESEND_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The  room this event is registered on. */
	const char* RoomName;
));


/** The most recent version of the EOS_RTCAudio_AudioBuffer API */
#define EOS_RTCAUDIO_AUDIOBUFFER_API_LATEST 1

/**
 * This struct is used to represent an audio buffer received in callbacks from EOS_RTCAudio_AddNotifyAudioBeforeSend and EOS_RTCAudio_AddNotifyAudioBeforeRender.
 */
EOS_STRUCT(EOS_RTCAudio_AudioBuffer, (
	/** API Version: Set this to EOS_RTCAUDIO_AUDIOBUFFER_API_LATEST. */
	int32_t ApiVersion;
	/** Pointer to the data with the interleaved audio frames in signed 16 bits format. */
	int16_t* Frames;
	/**
	 * Number of frames available in the Frames buffer.
	 * @note This is the number of frames in a channel, not the total number of frames in the buffer.
	 */
	uint32_t FramesCount;
	/** Sample rate for the samples in the Frames buffer. */
	uint32_t SampleRate;
	/** Number of channels for the samples in the Frames buffer. */
	uint32_t Channels;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioBeforeSend registered event.
 */
EOS_STRUCT(EOS_RTCAudio_AudioBeforeSendCallbackInfo, (
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioBeforeSend. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room associated with this event. */
	const char* RoomName;
	/** Audio buffer. */
	EOS_RTCAudio_AudioBuffer* Buffer;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioBeforeSendCallback, const EOS_RTCAudio_AudioBeforeSendCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_AddNotifyAudioBeforeRender API. */
#define EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORERENDER_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_AddNotifyAudioBeforeRender.
 */
EOS_STRUCT(EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORERENDER_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The  room this event is registered on. */
	const char* RoomName;
	/**
	 * Mixed audio or unmixed audio.
	 */
	EOS_Bool bUnmixedAudio;
));


/**
 * This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioBeforeRender registered event.
 */
EOS_STRUCT(EOS_RTCAudio_AudioBeforeRenderCallbackInfo, (
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioBeforeRender. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room associated with this event. */
	const char* RoomName;
	/**
	 * Audio buffer.
	 */
	EOS_RTCAudio_AudioBuffer* Buffer;
	/**
	 * The Product User ID of the participant if bUnmixedAudio was set to true when setting the notifications, or empty if
	 * bUnmixedAudio was set to false and thus the buffer is the mixed audio of all participants
	 */
	EOS_ProductUserId ParticipantId;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnAudioBeforeRenderCallback, const EOS_RTCAudio_AudioBeforeRenderCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudio_SendAudio API. */
#define EOS_RTCAUDIO_SENDAUDIO_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_SendAudio.
 */
EOS_STRUCT(EOS_RTCAudio_SendAudioOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_SENDAUDIO_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The  room this event is registered on. */
	const char* RoomName;
	/**
	 * Audio buffer, which must have a duration of 10 ms.
	 * @note The SDK makes a copy of buffer. There is no need to keep the buffer around after calling EOS_RTCAudio_SendAudio
	 */
	EOS_RTCAudio_AudioBuffer* Buffer;
));

/** The most recent version of the EOS_RTCAudio_UpdateSending API. */
#define EOS_RTCAUDIO_UPDATESENDING_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateSending
 */
EOS_STRUCT(EOS_RTCAudio_UpdateSendingOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_UPDATESENDING_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** Muted or unmuted audio track status */
	EOS_ERTCAudioStatus AudioStatus;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUpdateSendingCallback.
 */
EOS_STRUCT(EOS_RTCAudio_UpdateSendingCallbackInfo, (
	/** This returns:
	 * EOS_Success if sending of channels of the local user was successfully enabled/disabled.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateSending. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** Muted or unmuted audio track status */
	EOS_ERTCAudioStatus AudioStatus;
));

/**
 * Callback for completion of update sending request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateSendingCallback, const EOS_RTCAudio_UpdateSendingCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_UpdateReceiving API. */
#define EOS_RTCAUDIO_UPDATERECEIVING_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateReceiving.
 */
EOS_STRUCT(EOS_RTCAudio_UpdateReceivingOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_UPDATERECEIVING_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The participant to modify or null to update the global configuration */
	EOS_ProductUserId ParticipantId;
	/** Mute or unmute audio track */
	EOS_Bool bAudioEnabled;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUpdateReceivingCallback.
 */
EOS_STRUCT(EOS_RTCAudio_UpdateReceivingCallbackInfo, (
	/** This returns:
	 * EOS_Success if receiving of channels of remote users was successfully enabled/disabled.
     * EOS_NotFound if the participant isn't found by ParticipantId.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateReceiving. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The participant to modify or null to update the global configuration */
	EOS_ProductUserId ParticipantId;
	/** Muted or unmuted audio track */
	EOS_Bool bAudioEnabled;
));

/**
 * Callback for completion of update receiving request
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateReceivingCallback, const EOS_RTCAudio_UpdateReceivingCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_UpdateSendingVolume API. */
#define EOS_RTCAUDIO_UPDATESENDINGVOLUME_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateSendingVolume
 */
EOS_STRUCT(EOS_RTCAudio_UpdateSendingVolumeOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_UPDATESENDINGVOLUME_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The volume to be set for sent audio (range 0.0 to 100.0). Volume 50 means that the audio volume is not modified
	 * and stays in its source value. */
	float Volume;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUpdateSendingVolumeCallback.
 */
EOS_STRUCT(EOS_RTCAudio_UpdateSendingVolumeCallbackInfo, (
	/** This returns:
	 * EOS_Success if sending volume of channels of the local user was successfully changed.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateSendingVolume. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The volume that was set for sent audio (range 0.0 to 100.0). */
	float Volume;
));

/**
 * Callback for completion of update sending volume request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateSendingVolumeCallback, const EOS_RTCAudio_UpdateSendingVolumeCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_UpdateReceivingVolume API. */
#define EOS_RTCAUDIO_UPDATERECEIVINGVOLUME_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateReceivingVolume
 */
EOS_STRUCT(EOS_RTCAudio_UpdateReceivingVolumeOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_UPDATERECEIVINGVOLUME_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The volume to be set for received audio (range 0.0 to 100.0). Volume 50 means that the audio volume is not modified
	 * and stays in its source value. */
	float Volume;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUpdateReceivingVolumeCallback.
 */
EOS_STRUCT(EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo, (
	/** This returns:
	 * EOS_Success if receiving volume of channels of the local user was successfully changed.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateReceivingVolume. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The volume that was set for received audio (range 0.0 to 100.0). */
	float Volume;
));

/**
 * Callback for completion of update receiving volume request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateReceivingVolumeCallback, const EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudio_UpdateParticipantVolume API. */
#define EOS_RTCAUDIO_UPDATEPARTICIPANTVOLUME_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateParticipantVolume
 */
EOS_STRUCT(EOS_RTCAudio_UpdateParticipantVolumeOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_UPDATEPARTICIPANTVOLUME_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The participant to modify or null to update the global configuration */
	EOS_ProductUserId ParticipantId;
	/** The volume to be set for received audio (range 0.0 to 100.0). Volume 50 means that the audio volume is not modified
	 * and stays in its source value. */
	float Volume;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUpdateParticipantVolumeCallback.
 */
EOS_STRUCT(EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo, (
	/** This returns:
	 * EOS_Success if volume of remote participant audio was successfully changed.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateParticipantVolume. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** The participant to modify or null to update the global configuration */
	EOS_ProductUserId ParticipantId;
	/** The volume that was set for received audio (range 0.0 to 100.0). */
	float Volume;
));

/**
 * Callback for completion of update participant volume request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUpdateParticipantVolumeCallback, const EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_RegisterPlatformUser API. */
#define EOS_RTCAUDIO_REGISTERPLATFORMUSER_API_LATEST 1

/**
 * This struct is used to inform the audio system of a user.
 */
EOS_STRUCT(EOS_RTCAudio_RegisterPlatformUserOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_REGISTERPLATFORMUSER_API_LATEST. */
	int32_t ApiVersion;
	/** Platform dependent user id. */
	const char* PlatformUserId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnRegisterPlatformUserCallback.
 */
EOS_STRUCT(EOS_RTCAudio_OnRegisterPlatformUserCallbackInfo, (
	/** This returns:
	 * EOS_Success if the user was successfully registered.
	 * EOS_InvalidParameters if any of the parameters are incorrect.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_RegisterPlatformUser. */
	void* ClientData;
	/** Platform dependent user id. */
	const char* PlatformUserId;
));

/**
 * Callback for completion of register platform user request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnRegisterPlatformUserCallback, const EOS_RTCAudio_OnRegisterPlatformUserCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_UnregisterPlatformUser API. */
#define EOS_RTCAUDIO_UNREGISTERPLATFORMUSER_API_LATEST 1

/**
 * This struct is used to remove a user from the audio system.
 */
EOS_STRUCT(EOS_RTCAudio_UnregisterPlatformUserOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_UNREGISTERPLATFORMUSER_API_LATEST. */
	int32_t ApiVersion;
	/** The account of a user associated with this event. */
	const char* PlatformUserId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUnregisterPlatformUserCallback.
 */
EOS_STRUCT(EOS_RTCAudio_OnUnregisterPlatformUserCallbackInfo, (
	/** This returns:
	 * EOS_Success if the user was successfully unregistered.
	 * EOS_InvalidParameters if any of the parameters are incorrect.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UnregisterPlatformUser. */
	void* ClientData;
	/** Platform dependent user id. */
	const char* PlatformUserId;
));

/**
 * Callback for completion of unregister platform user request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnUnregisterPlatformUserCallback, const EOS_RTCAudio_OnUnregisterPlatformUserCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_QueryInputDevicesInformation API. */
#define EOS_RTCAUDIO_QUERYINPUTDEVICESINFORMATION_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_QueryInputDevicesInformation.
 */
EOS_STRUCT(EOS_RTCAudio_QueryInputDevicesInformationOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_QUERYINPUTDEVICESINFORMATION_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnQueryInputDevicesInformationCallback.
 */
EOS_STRUCT(EOS_RTCAudio_OnQueryInputDevicesInformationCallbackInfo, (
	/** This returns:
	 * EOS_Success if the operation succeeded.
	 * EOS_InvalidParameters if any of the parameters are incorrect.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_QueryInputDevicesInformation. */
	void* ClientData;
));

/**
 * Callback for completion of query input devices information request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnQueryInputDevicesInformationCallback, const EOS_RTCAudio_OnQueryInputDevicesInformationCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_GetInputDevicesCount API. */
#define EOS_RTCAUDIO_GETINPUTDEVICESCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_RTCAudio_GetInputDevicesCount function.
 */
EOS_STRUCT(EOS_RTCAudio_GetInputDevicesCountOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_GETINPUTDEVICESCOUNT_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_RTCAudio_CopyInputDeviceInformationByIndex API. */
#define EOS_RTCAUDIO_COPYINPUTDEVICEINFORMATIONBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_RTCAudio_CopyInputDeviceInformationByIndex function.
 */
EOS_STRUCT(EOS_RTCAudio_CopyInputDeviceInformationByIndexOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_COPYINPUTDEVICEINFORMATIONBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the audio input device's information to retrieve. */
	uint32_t DeviceIndex;
));

/** The most recent version of the EOS_RTCAudio_InputDeviceInformation struct. */
#define EOS_RTCAUDIO_INPUTDEVICEINFORMATION_API_LATEST 1

/**
 * This struct is used to get information about a specific audio input device.
 */
EOS_STRUCT(EOS_RTCAudio_InputDeviceInformation, (
	/** API Version: Set this to EOS_RTCAUDIO_INPUTDEVICEINFORMATION_API_LATEST. */
	int32_t ApiVersion;
	/** True if this is the default audio input device in the system. */
	EOS_Bool bDefaultDevice;
	/**
	 * The persistent unique id of the audio input device.
	 * The value can be cached - invalidated only when the audio device pool is changed.
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	const char* DeviceId;
	/** Human-readable name of the audio input device */
	const char* DeviceName;
));

/**
 * Release the memory associated with EOS_RTCAudio_InputDeviceInformation. This must be called on data retrieved from
 * EOS_RTCAudio_CopyInputDeviceInformationByIndex.
 *
 * @param DeviceInformation - The audio input device's information to release.
 *
 * @see EOS_RTCAudio_InputDeviceInformation
 * @see EOS_RTCAudio_CopyInputDeviceInformationByIndex
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudio_InputDeviceInformation_Release(EOS_RTCAudio_InputDeviceInformation* DeviceInformation);

/** The most recent version of the EOS_RTCAudio_QueryOutputDevicesInformation API. */
#define EOS_RTCAUDIO_QUERYOUTPUTDEVICESINFORMATION_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_QueryOutputDevicesInformation.
 */
EOS_STRUCT(EOS_RTCAudio_QueryOutputDevicesInformationOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_QUERYOUTPUTDEVICESINFORMATION_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnQueryOutputDevicesInformationCallback.
 */
EOS_STRUCT(EOS_RTCAudio_OnQueryOutputDevicesInformationCallbackInfo, (
	/** This returns:
	 * EOS_Success if the operation succeeded.
	 * EOS_InvalidParameters if any of the parameters are incorrect.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_QueryOutputDevicesInformation. */
	void* ClientData;
));

/**
 * Callback for completion of query output devices information request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnQueryOutputDevicesInformationCallback, const EOS_RTCAudio_OnQueryOutputDevicesInformationCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_GetOutputDevicesCount API. */
#define EOS_RTCAUDIO_GETOUTPUTDEVICESCOUNT_API_LATEST 1

/**
 * Output parameters for the EOS_RTCAudio_GetOutputDevicesCount function.
 */
EOS_STRUCT(EOS_RTCAudio_GetOutputDevicesCountOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_GETOUTPUTDEVICESCOUNT_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_RTCAudio_CopyOutputDeviceInformationByIndex API. */
#define EOS_RTCAUDIO_COPYOUTPUTDEVICEINFORMATIONBYINDEX_API_LATEST 1

/**
 * Output parameters for the EOS_RTCAudio_CopyOutputDeviceInformationByIndex function.
 */
EOS_STRUCT(EOS_RTCAudio_CopyOutputDeviceInformationByIndexOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_COPYOUTPUTDEVICEINFORMATIONBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the audio output device's information to retrieve. */
	uint32_t DeviceIndex;
));

/** The most recent version of the EOS_RTCAudio_OutputDeviceInformation struct. */
#define EOS_RTCAUDIO_OUTPUTDEVICEINFORMATION_API_LATEST 1

/**
 * This struct is used to get information about a specific audio output device.
 */
EOS_STRUCT(EOS_RTCAudio_OutputDeviceInformation, (
	/** API Version: Set this to EOS_RTCAUDIO_OUTPUTDEVICEINFORMATION_API_LATEST. */
	int32_t ApiVersion;
	/** True if this is the default audio output device in the system. */
	EOS_Bool bDefaultDevice;
	/**
	 * The persistent unique id of the audio output device.
	 * The value can be cached - invalidated only when the audio device pool is changed.
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	const char* DeviceId;
	/** Human-readable name of the audio output device */
	const char* DeviceName;
));

/**
 * Release the memory associated with EOS_RTCAudio_OutputDeviceInformation. This must be called on data retrieved from
 * EOS_RTCAudio_CopyOutputDeviceInformationByIndex.
 *
 * @param DeviceInformation - The audio output device's information to release.
 *
 * @see EOS_RTCAudio_OutputDeviceInformation
 * @see EOS_RTCAudio_CopyOutputDeviceInformationByIndex
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudio_OutputDeviceInformation_Release(EOS_RTCAudio_OutputDeviceInformation* DeviceInformation);

/** The most recent version of the EOS_RTCAudio_SetInputDeviceSettings API. */
#define EOS_RTCAUDIO_SETINPUTDEVICESETTINGS_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_SetInputDeviceSettings.
 */
EOS_STRUCT(EOS_RTCAudio_SetInputDeviceSettingsOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_SETINPUTDEVICESETTINGS_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/**
	 * The device Id to be used for this user. Pass NULL or empty string to use a default input device.
	 *
	 * If the device ID is invalid, the default device will be used instead.
	 * Despite this fact, that device ID will be stored and the library will try to move on it when an audio device pool is being changed.
	 *
	 * The actual hardware audio input device usage depends on the current payload and optimized not to use it
	 * when generated audio frames cannot be processed by someone else based on a scope of rules (For instance, when a client is alone in a room).
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	const char* RealDeviceId;
	/** Enable or disable Platform AEC (Acoustic Echo Cancellation) if available. */
	EOS_Bool bPlatformAEC;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnSetInputDeviceSettingsCallback.
 */
EOS_STRUCT(EOS_RTCAudio_OnSetInputDeviceSettingsCallbackInfo, (
	/** This returns:
	 * EOS_Success if the operation succeeded.
	 * EOS_InvalidParameters if any of the parameters are incorrect.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_SetInputDeviceSettings. */
	void* ClientData;
	/** Associated audio input device Id. */
	const char* RealDeviceId;
));

/**
 * Callback for completion of set input device settings request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnSetInputDeviceSettingsCallback, const EOS_RTCAudio_OnSetInputDeviceSettingsCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudio_SetOutputDeviceSettings API. */
#define EOS_RTCAUDIO_SETOUTPUTDEVICESETTINGS_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_SetOutputDeviceSettings.
 */
EOS_STRUCT(EOS_RTCAudio_SetOutputDeviceSettingsOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_SETOUTPUTDEVICESETTINGS_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/**
	 * The device Id to be used for this user. Pass NULL or empty string to use a default output device.
	 *
	 * If the device ID is invalid, the default device will be used instead.
	 * Despite this fact, that device ID will be stored and the library will try to move on it when an audio device pool is being changed.
	 *
	 * The actual hardware audio output device usage depends on the current payload and optimized not to use it
	 * when generated audio frames cannot be processed by someone else based on a scope of rules (For instance, when a client is alone in a room).
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	const char* RealDeviceId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnSetOutputDeviceSettingsCallback.
 */
EOS_STRUCT(EOS_RTCAudio_OnSetOutputDeviceSettingsCallbackInfo, (
	/** This returns:
	 * EOS_Success if the operation succeeded.
	 * EOS_InvalidParameters if any of the parameters are incorrect.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_SetOutputDeviceSettings. */
	void* ClientData;
	/** Associated audio output device Id. */
	const char* RealDeviceId;
));

/**
 * Callback for completion of set output device settings request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudio_OnSetOutputDeviceSettingsCallback, const EOS_RTCAudio_OnSetOutputDeviceSettingsCallbackInfo* Data);

#pragma pack(pop)

#include "eos_rtc_audio_types_deprecated.inl"
