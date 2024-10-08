// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Friends_AcceptInvite.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Friends_AcceptInvite* UEIK_Friends_AcceptInvite::EIK_Friends_AcceptInvite(FEIK_EpicAccountId LocalUserId,
                                                                               FEIK_EpicAccountId TargetUserId)
{
	UEIK_Friends_AcceptInvite* Node = NewObject<UEIK_Friends_AcceptInvite>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_Friends_AcceptInvite::OnAcceptInviteCallback(const EOS_Friends_AcceptInviteCallbackInfo* Data)
{
	if (UEIK_Friends_AcceptInvite* Node = static_cast<UEIK_Friends_AcceptInvite*>(Data->ClientData))
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

void UEIK_Friends_AcceptInvite::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_AcceptInviteOptions AcceptInviteOptions = {};
			AcceptInviteOptions.ApiVersion = EOS_FRIENDS_ACCEPTINVITE_API_LATEST;
			AcceptInviteOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			AcceptInviteOptions.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			EOS_Friends_AcceptInvite(EOSRef->FriendsHandle, &AcceptInviteOptions, this, &UEIK_Friends_AcceptInvite::OnAcceptInviteCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to accept friend invite either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_EpicAccountId(), FEIK_EpicAccountId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
