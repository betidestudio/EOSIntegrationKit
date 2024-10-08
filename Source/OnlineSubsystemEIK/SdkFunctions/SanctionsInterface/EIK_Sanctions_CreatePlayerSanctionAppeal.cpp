// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sanctions_CreatePlayerSanctionAppeal.h"

#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Sanctions_CreatePlayerSanctionAppeal* UEIK_Sanctions_CreatePlayerSanctionAppeal::EIK_Sanctions_CreatePlayerSanctionAppeal(FEIK_ProductUserId LocalUserId, const TEnumAsByte<EEIK_ESanctionAppealReason>& AppealReason, const FString& ReferenceId)
{
	UEIK_Sanctions_CreatePlayerSanctionAppeal* Node = NewObject<UEIK_Sanctions_CreatePlayerSanctionAppeal>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_AppealReason = AppealReason;
	Node->Var_ReferenceId = ReferenceId;
	return Node;
}

void UEIK_Sanctions_CreatePlayerSanctionAppeal::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sanctions_CreatePlayerSanctionAppealOptions Options = {};
			Options.ApiVersion = EOS_SANCTIONS_CREATEPLAYERSANCTIONAPPEAL_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.Reason = static_cast<EOS_ESanctionAppealReason>(Var_AppealReason.GetValue());
			Options.ReferenceId = TCHAR_TO_ANSI(*Var_ReferenceId);
			EOS_Sanctions_CreatePlayerSanctionAppeal(EOSRef->SanctionsHandle, &Options, this, &UEIK_Sanctions_CreatePlayerSanctionAppeal::Internal_OnCreatePlayerSanctionAppealComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_Sanctions_CreatePlayerSanctionAppeal::Activate: Unable to get EOS SDK"));
	OnCallback.Broadcast(Var_LocalUserId, EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sanctions_CreatePlayerSanctionAppeal::Internal_OnCreatePlayerSanctionAppealComplete(
	const EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo* Data)
{
	if (UEIK_Sanctions_CreatePlayerSanctionAppeal* Node = static_cast<UEIK_Sanctions_CreatePlayerSanctionAppeal*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(Node->Var_LocalUserId, static_cast<EEIK_Result>(Data->ResultCode));
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}
