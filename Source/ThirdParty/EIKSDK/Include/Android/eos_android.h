// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_types.h"

#pragma pack(push, 8)
/** The most recent version of the EOS_Android_InitializeOptions structure. */
#define EOS_ANDROID_INITIALIZEOPTIONS_API_LATEST 2

/**
 * Options for initializing mount paths required for some platforms.
 */
EOS_STRUCT(EOS_Android_InitializeOptions, (
	/** API Version: Set this to EOS_ANDROID_INITIALIZEOPTIONS_API_LATEST. */
	int32_t ApiVersion;

	/** Reserved, set to null */
	void* Reserved;

	/** Full internal directory path. Can be null */
	const char* OptionalInternalDirectory;
	/** Full external directory path. Can be null */
	const char* OptionalExternalDirectory;
));

#pragma pack(pop)
