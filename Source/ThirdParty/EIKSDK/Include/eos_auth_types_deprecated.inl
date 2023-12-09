// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

/** The most recent version of the EOS_Auth_AccountFeatureRestrictedInfo struct. */
#define EOS_AUTH_ACCOUNTFEATURERESTRICTEDINFO_API_LATEST 1

/**
 * Intermediate data needed to complete account restriction verification during login flow, returned by EOS_Auth_LoginCallbackInfo when the ResultCode is EOS_Auth_AccountFeatureRestricted.
 * The URI inside should be exposed to the user for entry in a web browser. The URI must be copied out of this struct before completion of the callback.
 */
EOS_STRUCT(EOS_Auth_AccountFeatureRestrictedInfo, (
	/** API Version: Set this to EOS_AUTH_ACCOUNTFEATURERESTRICTEDINFO_API_LATEST. */
	int32_t ApiVersion;
	/** The end-user verification URI. Users must be asked to open the page in a browser to address the restrictions. */
	const char* VerificationURI;
));

#pragma pack(pop)
