// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Auth_QueryIdToken.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Auth_QueryIdToken* UEIK_Auth_QueryIdToken::QueryIdToken(FEIK_EpicAccountId LocalUserId,
	FEIK_EpicAccountId TargetAccountId)
{
	UEIK_Auth_QueryIdToken* Node = NewObject<UEIK_Auth_QueryIdToken>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetAccountId = TargetAccountId;
	return Node;
}

void UEIK_Auth_QueryIdToken::OnQueryIdTokenCallback(const EOS_Auth_QueryIdTokenCallbackInfo* Data)
{
	UEIK_Auth_QueryIdToken* Node = static_cast<UEIK_Auth_QueryIdToken*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->TargetAccountId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Auth_QueryIdToken::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_QueryIdTokenOptions Options = {};
			Options.ApiVersion = EOS_AUTH_QUERYIDTOKEN_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.TargetAccountId = Var_TargetAccountId.GetValueAsEosType();
			EOS_Auth_QueryIdToken(EOSRef->AuthHandle, &Options, this, &UEIK_Auth_QueryIdToken::OnQueryIdTokenCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query id token either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, Var_LocalUserId, Var_TargetAccountId);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
