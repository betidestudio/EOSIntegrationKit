// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Friends_RejectInvite.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Friends_RejectInvite* UEIK_Friends_RejectInvite::EIK_Friends_RejectInvite(FEIK_EpicAccountId LocalUserId,
	FEIK_EpicAccountId TargetUserId)
{
	UEIK_Friends_RejectInvite* Node = NewObject<UEIK_Friends_RejectInvite>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_Friends_RejectInvite::OnRejectInviteCallback(const EOS_Friends_RejectInviteCallbackInfo* Data)
{
	if (UEIK_Friends_RejectInvite* Node = static_cast<UEIK_Friends_RejectInvite*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->TargetUserId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Friends_RejectInvite::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_RejectInviteOptions RejectInviteOptions = {};
			RejectInviteOptions.ApiVersion = EOS_FRIENDS_REJECTINVITE_API_LATEST;
			RejectInviteOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			RejectInviteOptions.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			EOS_Friends_RejectInvite(EOSRef->FriendsHandle, &RejectInviteOptions, this, &UEIK_Friends_RejectInvite::OnRejectInviteCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to reject friend invite either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_EpicAccountId(), FEIK_EpicAccountId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
