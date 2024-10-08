// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Friends_SendInvite.h"

UEIK_Friends_SendInvite* UEIK_Friends_SendInvite::EIK_Friends_SendInvite(FEIK_EpicAccountId LocalUserId,
	FEIK_EpicAccountId TargetUserId)
{
	UEIK_Friends_SendInvite* Node = NewObject<UEIK_Friends_SendInvite>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_Friends_SendInvite::OnSendInviteCallback(const EOS_Friends_SendInviteCallbackInfo* Data)
{
	if (UEIK_Friends_SendInvite* Node = static_cast<UEIK_Friends_SendInvite*>(Data->ClientData))
	{
		Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->TargetUserId);
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Friends_SendInvite::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_SendInviteOptions SendInviteOptions = {};
			SendInviteOptions.ApiVersion = EOS_FRIENDS_SENDINVITE_API_LATEST;
			SendInviteOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			SendInviteOptions.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			EOS_Friends_SendInvite(EOSRef->FriendsHandle, &SendInviteOptions, this, &UEIK_Friends_SendInvite::OnSendInviteCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to send friend invite either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_EpicAccountId(), FEIK_EpicAccountId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
