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
	else
	{
		return false;
	}
}

IVoiceChat* UEIK_Voice_Subsystem::EVIK_Local_GetVoiceChat()
{
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

void UEIK_Voice_Subsystem::UpdatePositionalVoiceChat(bool UsePositionalVoiceChat, float FMaxHearingDistance, bool V_bUseDebugPoint, FVector V_DebugPointLocation)
{
	bIsPositionalVoiceChatUsed = UsePositionalVoiceChat;
	MaxHearingDistance = FMaxHearingDistance;
	bUseDebugPoint = V_bUseDebugPoint;
	DebugPointLocation = V_DebugPointLocation;
}

void UEIK_Voice_Subsystem::ForceUpdateOutputVolume(bool bUseThisVolume, bool bUseWithPositionalChat, float Volume)
{
	bUseOutputVolume = bUseThisVolume;
	bUseOutputVolumeWithPositionalChat = bUseWithPositionalChat;
	OutputVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
	return;
}

void UEIK_Voice_Subsystem::PlayerListUpdated()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerListUpdated"));
	if(UpdatePositionalVoiceChatTimerHandle.IsValid())
	{
		UpdatePositionalVoiceChatTimerHandle.Invalidate();
	}
	//GetWorld()->GetTimerManager().SetTimer(UpdatePositionalVoiceChatTimerHandle, this, &UEIK_Voice_Subsystem::UpdatePositionalVoiceChat, 0.2f, true);
}

void UEIK_Voice_Subsystem::SetupPlayerList_Implementation(const TArray<FEVIKPlayerList>& PlayerList)
{
	PlayerListVar = PlayerList;
}

void UEIK_Voice_Subsystem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(UEIK_Voice_Subsystem, PlayerListVar);
}