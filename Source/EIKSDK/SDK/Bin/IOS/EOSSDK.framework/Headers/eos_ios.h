// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_types.h"

#pragma pack(push, 8)

/** The most recent version of the EOS_IOS_Auth_CredentialsOptions structure. */
#define EOS_IOS_AUTH_CREDENTIALSOPTIONS_API_LATEST 1

/**
 * Options for initializing login for IOS.
 */
EOS_STRUCT(EOS_IOS_Auth_CredentialsOptions, (
	/** API version of EOS_IOS_Auth_CredentialsOptions. */
	int32_t ApiVersion;

	/** When calling EOS_Auth_Login
	 *  NSObject that implements the ASWebAuthenticationPresentationContextProviding protocol,
	 *  typically this is added to the applications UIViewController.
	 *  Required for iOS 13+ only, for earlier versions this value must be a nullptr.
	 *  Object must be retained and cast to a void* using: (void*)CFBridgingRetain(presentationContextProviding)
	 *  EOSSDK will release this bridged object when the value is consumed for iOS 13+.
	 */
	void* PresentationContextProviding;

));

#pragma pack(pop)
