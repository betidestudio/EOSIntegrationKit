// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sessions_QueryInvites.h"

UEIK_Sessions_QueryInvites* UEIK_Sessions_QueryInvites::EIK_Sessions_QueryInvites(FEIK_ProductUserId LocalUserId)
{
	UEIK_Sessions_QueryInvites* Node = NewObject<UEIK_Sessions_QueryInvites>();
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}

void UEIK_Sessions_QueryInvites::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_QueryInvitesOptions QueryInvitesOptions = { };
			QueryInvitesOptions.ApiVersion = EOS_SESSIONS_QUERYINVITES_API_LATEST;
			QueryInvitesOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_Sessions_QueryInvites(EOSRef->SessionsHandle, &QueryInvitesOptions, this, &UEIK_Sessions_QueryInvites::OnQueryInvitesCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query invites either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sessions_QueryInvites::OnQueryInvitesCallback(const EOS_Sessions_QueryInvitesCallbackInfo* Data)
{
	if (UEIK_Sessions_QueryInvites* Node = static_cast<UEIK_Sessions_QueryInvites*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
