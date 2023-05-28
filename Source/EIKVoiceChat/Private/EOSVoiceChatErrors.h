// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "EOSShared.h"

#if WITH_EOS_RTC

#include "VoiceChatResult.h"

#define EOSVOICECHAT_ERROR(...) FVoiceChatResult::CreateError(TEXT("errors.com.epicgames.voicechat.eos"), __VA_ARGS__)

enum class EOS_EResult : int32_t;

FVoiceChatResult ResultFromEOSResult(const EOS_EResult EosResult);

#endif // WITH_EOS_RTC
