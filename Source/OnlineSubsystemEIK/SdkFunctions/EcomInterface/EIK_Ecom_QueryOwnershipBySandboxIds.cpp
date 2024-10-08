// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Ecom_QueryOwnershipBySandboxIds.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Ecom_QueryOwnershipBySandboxIds* UEIK_Ecom_QueryOwnershipBySandboxIds::EIK_Ecom_QueryOwnershipBySandboxIds(
	FEIK_Ecom_QueryOwnershipBySandboxIdsOptions QueryOwnershipBySandboxIdsOptions)
{
	UEIK_Ecom_QueryOwnershipBySandboxIds* Node = NewObject<UEIK_Ecom_QueryOwnershipBySandboxIds>();
	Node->Var_QueryOwnershipBySandboxIdsOptions = QueryOwnershipBySandboxIdsOptions;
	return Node;
}

void UEIK_Ecom_QueryOwnershipBySandboxIds::OnQueryOwnershipBySandboxIdsCallback(
	const EOS_Ecom_QueryOwnershipBySandboxIdsCallbackInfo* Data)
{
	if (UEIK_Ecom_QueryOwnershipBySandboxIds* Node = static_cast<UEIK_Ecom_QueryOwnershipBySandboxIds*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(*Data);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Ecom_QueryOwnershipBySandboxIds::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Ecom_QueryOwnershipBySandboxIdsOptions QueryOwnershipBySandboxIdsOptions = Var_QueryOwnershipBySandboxIdsOptions.ToEOS_Ecom_QueryOwnershipBySandboxIdsOptions();
			EOS_Ecom_QueryOwnershipBySandboxIds(EOSRef->EcomHandle, &QueryOwnershipBySandboxIdsOptions, this, &UEIK_Ecom_QueryOwnershipBySandboxIds::OnQueryOwnershipBySandboxIdsCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query ownership by sandbox ids either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_Ecom_QueryOwnershipBySandboxIdsCallbackInfo());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
