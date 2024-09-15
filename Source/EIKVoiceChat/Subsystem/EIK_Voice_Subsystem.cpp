// Copyright 2023 Betide Studio. All rights reserved.


#include "EIK_Voice_Subsystem.h"
#include "Net/UnrealNetwork.h"


bool UEIK_Voice_Subsystem::EVIK_Local_Initialize()
{
	EVIK_VoiceChat = IVoiceChat::Get();
	if(EVIK_VoiceChat)
	{
		return EVIK_VoiceChat->Initialize();
	}
	return false;
}

IVoiceChat* UEIK_Voice_Subsystem::EVIK_Local_GetVoiceChat()
{
	if (!EVIK_VoiceChat)
	{
		EVIK_VoiceChat = IVoiceChat::Get();
		if(EVIK_VoiceChat)
		{
			EVIK_VoiceChat->Initialize();
		}
	}
	return EVIK_VoiceChat;
}

void UEIK_Voice_Subsystem::EVIK_Local_Connect(const FEIKResultDelegate& ResultDelegate)
{
	if(EVIK_VoiceChat)
	{
		EVIK_VoiceChat->Connect(FOnVoiceChatConnectCompleteDelegate::CreateLambda([this, ResultDelegate](const FVoiceChatResult& Result)
		{
			if(Result.IsSuccess())
			{
				ResultDelegate.ExecuteIfBound(true, EEVIKResultCodes::Success);
			}
			else
			{
				ResultDelegate.ExecuteIfBound(false, EEVIKResultCodes::Failed);
			}
		}
		));
	}	
}


void UEIK_Voice_Subsystem::PlayerListUpdated()
{
	if(UpdatePositionalVoiceChatTimerHandle.IsValid())
	{
		UpdatePositionalVoiceChatTimerHandle.Invalidate();
	}
	//GetWorld()->GetTimerManager().SetTimer(UpdatePositionalVoiceChatTimerHandle, this, &UEIK_Voice_Subsystem::UpdatePositionalVoiceChat, 0.2f, true);
}