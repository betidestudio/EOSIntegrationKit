// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Ecom_QueryEntitlements.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Ecom_QueryEntitlements* UEIK_Ecom_QueryEntitlements::EIK_Ecom_QueryEntitlements(
	FEIK_Ecom_QueryEntitlementsOptions QueryEntitlementsOptions)
{
	UEIK_Ecom_QueryEntitlements* Node = NewObject<UEIK_Ecom_QueryEntitlements>();
	Node->Local_QueryEntitlementsOptions = QueryEntitlementsOptions;
	return Node;
}

void UEIK_Ecom_QueryEntitlements::OnQueryEntitlementsCallback(const EOS_Ecom_QueryEntitlementsCallbackInfo* Data)
{
	if (UEIK_Ecom_QueryEntitlements* Node = static_cast<UEIK_Ecom_QueryEntitlements*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(Data->LocalUserId, static_cast<EEIK_Result>(Data->ResultCode));
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Ecom_QueryEntitlements::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Ecom_QueryEntitlementsOptions QueryEntitlementsOptions = Local_QueryEntitlementsOptions.ToEOS_Ecom_QueryEntitlementsOptions();
			EOS_Ecom_QueryEntitlements(EOSRef->EcomHandle, &QueryEntitlementsOptions, this, &UEIK_Ecom_QueryEntitlements::OnQueryEntitlementsCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query entitlements either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_EpicAccountId(), EEIK_Result::EOS_ServiceFailure);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
