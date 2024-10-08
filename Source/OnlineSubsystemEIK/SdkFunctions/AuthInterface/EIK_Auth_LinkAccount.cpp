// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Auth_LinkAccount.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Auth_LinkAccount* UEIK_Auth_LinkAccount::LinkAccount(FEIK_ContinuanceToken ContinuanceToken,
	FEIK_EpicAccountId LocalUserId, TEnumAsByte<EEIK_ELinkAccountFlags> LinkAccountFlags)
{
	UEIK_Auth_LinkAccount* Node = NewObject<UEIK_Auth_LinkAccount>();
	Node->Var_ContinuanceToken = ContinuanceToken;
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_LinkAccountFlags = LinkAccountFlags;
	return Node;
}

void UEIK_Auth_LinkAccount::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_LinkAccountOptions Options = {};
			Options.ApiVersion = EOS_AUTH_LINKACCOUNT_API_LATEST;
			Options.ContinuanceToken = Var_ContinuanceToken.GetValueAsEosType();
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.LinkAccountFlags = static_cast<EOS_ELinkAccountFlags>(Var_LinkAccountFlags.GetValue());
			EOS_Auth_LinkAccount(EOSRef->AuthHandle, &Options, this, &UEIK_Auth_LinkAccount::OnLinkAccountCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to link account either OnlineSubsystem is not valid or EOSRef is not valid."));
	FEIK_Auth_LinkAccountCallbackInfo Info;
	Info.ResultCode = EEIK_Result::EOS_NotFound;
	OnCallback.Broadcast(Info);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Auth_LinkAccount::OnLinkAccountCallback(const EOS_Auth_LinkAccountCallbackInfo* Data)
{
	UEIK_Auth_LinkAccount* Node = static_cast<UEIK_Auth_LinkAccount*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(Data);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
