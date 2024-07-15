// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_SharedFunctionFile.h"

FString UEIK_SharedFunctionFile::ConvertOculusUserIdAndNonceToEosFormat(FString OculusUserId, FString OculusNonce)
{
	return OculusUserId + TEXT("|") + OculusNonce;
}
DEFINE_LOG_CATEGORY(LogEIK);
