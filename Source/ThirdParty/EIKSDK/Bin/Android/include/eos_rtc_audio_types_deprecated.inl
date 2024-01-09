// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

/**
 * This file contains the deprecated types for EOS RTC Audio. In a future version, these types will be removed.
 */

#pragma pack(push, 8)

/** The most recent version of the EOS_RTCAudio_RegisterPlatformAudioUser API. */
#define EOS_RTCAUDIO_REGISTERPLATFORMAUDIOUSER_API_LATEST 1

/**
 * This struct is used to inform the audio system of a user.
 */
EOS_STRUCT(EOS_RTCAudio_RegisterPlatformAudioUserOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_REGISTERPLATFORMAUDIOUSER_API_LATEST. */
	int32_t ApiVersion;
	/** Platform dependent user id. */
	const char* UserId;
));

/** The most recent version of the EOS_RTCAudio_UnregisterPlatformAudioUser API. */
#define EOS_RTCAUDIO_UNREGISTERPLATFORMAUDIOUSER_API_LATEST 1

/**
 * This struct is used to remove a user from the audio system.
 */
EOS_STRUCT(EOS_RTCAudio_UnregisterPlatformAudioUserOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_UNREGISTERPLATFORMAUDIOUSER_API_LATEST. */
	int32_t ApiVersion;
	/** The account of a user associated with this event. */
	const char* UserId;
));

/** The most recent version of the EOS_RTCAudio_GetAudioInputDevicesCount API. */
#define EOS_RTCAUDIO_GETAUDIOINPUTDEVICESCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_RTCAudio_GetAudioInputDevicesCount function.
 */
EOS_STRUCT(EOS_RTCAudio_GetAudioInputDevicesCountOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_GETAUDIOINPUTDEVICESCOUNT_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_RTCAudio_GetAudioInputDeviceByIndex API. */
#define EOS_RTCAUDIO_GETAUDIOINPUTDEVICEBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_RTCAudio_GetAudioInputDeviceByIndex function.
 */
EOS_STRUCT(EOS_RTCAudio_GetAudioInputDeviceByIndexOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_GETAUDIOINPUTDEVICEBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the device info to retrieve. */
	uint32_t DeviceInfoIndex;
));

/** The most recent version of the EOS_RTCAudio_AudioInputDeviceInfo struct. */
#define EOS_RTCAUDIO_AUDIOINPUTDEVICEINFO_API_LATEST 1

/**
 * This struct is used to get information about a specific input device.
 */
EOS_STRUCT(EOS_RTCAudio_AudioInputDeviceInfo, (
	/** API Version: Set this to EOS_RTCAUDIO_AUDIOINPUTDEVICEINFO_API_LATEST. */
	int32_t ApiVersion;
	/** True if this is the default audio input device in the system. */
	EOS_Bool bDefaultDevice;
	/**
	 * The persistent unique id of the device.
	 * The value can be cached - invalidated only when the audio device pool is changed.
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	const char* DeviceId;
	/** Human-readable name of the device */
	const char* DeviceName;
));

/** The most recent version of the EOS_RTCAudio_GetAudioOutputDevicesCount API. */
#define EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICESCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_RTCAudio_GetAudioOutputDevicesCount function.
 */
EOS_STRUCT(EOS_RTCAudio_GetAudioOutputDevicesCountOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICESCOUNT_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_RTCAudio_GetAudioOutputDeviceByIndex API. */
#define EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICEBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_RTCAudio_GetAudioOutputDeviceByIndex function.
 */
EOS_STRUCT(EOS_RTCAudio_GetAudioOutputDeviceByIndexOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_GETAUDIOOUTPUTDEVICEBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the device info to retrieve. */
	uint32_t DeviceInfoIndex;
));

/** The most recent version of the EOS_RTCAudio_AudioOutputDeviceInfo struct. */
#define EOS_RTCAUDIO_AUDIOOUTPUTDEVICEINFO_API_LATEST 1

/**
 * This struct is used to get information about a specific output device.
 */
EOS_STRUCT(EOS_RTCAudio_AudioOutputDeviceInfo, (
	/** API Version: Set this to EOS_RTCAUDIO_AUDIOOUTPUTDEVICEINFO_API_LATEST. */
	int32_t ApiVersion;
	/** True if this is the default audio output device in the system. */
	EOS_Bool bDefaultDevice;
	/**
	 * The persistent unique id of the device.
	 * The value can be cached - invalidated only when the audio device pool is changed.
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	const char* DeviceId;
	/** The human readable name of the device */
	const char* DeviceName;
));

/** The most recent version of the EOS_RTCAudio_SetAudioInputSettings API. */
#define EOS_RTCAUDIO_SETAUDIOINPUTSETTINGS_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_SetAudioInputSettings.
 */
EOS_STRUCT(EOS_RTCAudio_SetAudioInputSettingsOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_SETAUDIOINPUTSETTINGS_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/**
	 * The device Id to be used for this user. Pass NULL or empty string to use default input device.
	 *
	 * If the device ID is invalid, the default device will be used instead.
	 * Despite this fact, that device ID will be stored and the library will try to move on it when an audio device pool is being changed.
	 *
	 * The actual hardware audio device usage depends on the current payload and optimized not to use it
	 * when generated audio frames cannot be processed by someone else based on a scope of rules (For instance, when a client is alone in a room).
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	const char* DeviceId;
	/**
	 * The volume to be used for all rooms of this user (range 0.0 to 100.0).
	 *
	 * At the moment, the only value that produce any effect is 0.0 (silence). Any other value is ignored and causes no change to the volume.
	 */
	float Volume;
	/** Enable or disable Platform AEC (Acoustic Echo Cancellation) if available. */
	EOS_Bool bPlatformAEC;
));

/** The most recent version of the EOS_RTCAudio_SetAudioOutputSettings API. */
#define EOS_RTCAUDIO_SETAUDIOOUTPUTSETTINGS_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudio_SetAudioOutputSettings.
 */
EOS_STRUCT(EOS_RTCAudio_SetAudioOutputSettingsOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_SETAUDIOOUTPUTSETTINGS_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/**
	 * The device Id to be used for this user. Pass NULL or empty string to use default output device.
	 *
	 * If the device ID is invalid, the default device will be used instead.
	 * Despite of this fact, that device ID will be stored and the library will try to move on it when a device pool is being changed.
	 *
	 * The actual hardware audio device usage depends on the current payload and optimized not to use it
	 * when generated audio frames cannot be processed by someone else based on a scope of rules (For instance, when a client is alone in a room).
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 *
	 */
	const char* DeviceId;
	/**
	 * The volume to be used for all rooms of this user (range 0.0 to 100.0).
	 *
	 * Volume 50.0 means that the audio volume is not modified and stays in its source value.
	 */
	float Volume;
));

#pragma pack(pop)
