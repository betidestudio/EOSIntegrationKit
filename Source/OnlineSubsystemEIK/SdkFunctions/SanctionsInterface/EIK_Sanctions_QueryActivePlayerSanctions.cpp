// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sanctions_QueryActivePlayerSanctions.h"

#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Sanctions_QueryActivePlayerSanctions* UEIK_Sanctions_QueryActivePlayerSanctions::
EIK_Sanctions_QueryActivePlayerSanctions(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId TargetUserId)
{
	UEIK_Sanctions_QueryActivePlayerSanctions* Node = NewObject<UEIK_Sanctions_QueryActivePlayerSanctions>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_Sanctions_QueryActivePlayerSanctions::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sanctions_QueryActivePlayerSanctionsOptions Options = {};
			Options.ApiVersion = EOS_SANCTIONS_QUERYACTIVEPLAYERSANCTIONS_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			EOS_Sanctions_QueryActivePlayerSanctions(EOSRef->SanctionsHandle, &Options, this, &UEIK_Sanctions_QueryActivePlayerSanctions::Internal_OnQueryActivePlayerSanctionsComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_Sanctions_QueryActivePlayerSanctions::Activate: Unable to get EOS SDK"));
	OnCallback.Broadcast(Var_LocalUserId, Var_TargetUserId, EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sanctions_QueryActivePlayerSanctions::Internal_OnQueryActivePlayerSanctionsComplete(
	const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo* Data)
{
	if (UEIK_Sanctions_QueryActivePlayerSanctions* Node = static_cast<UEIK_Sanctions_QueryActivePlayerSanctions*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(Node->Var_LocalUserId, Node->Var_TargetUserId, static_cast<EEIK_Result>(Data->ResultCode));
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}