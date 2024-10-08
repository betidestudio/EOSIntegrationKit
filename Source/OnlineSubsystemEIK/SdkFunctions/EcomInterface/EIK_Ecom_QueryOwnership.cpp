// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Ecom_QueryOwnership.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Ecom_QueryOwnership* UEIK_Ecom_QueryOwnership::EIK_Ecom_QueryOwnership(
	FEIK_Ecom_QueryOwnershipOptions QueryOwnershipOptions)
{
	UEIK_Ecom_QueryOwnership* Node = NewObject<UEIK_Ecom_QueryOwnership>();
	Node->Var_QueryOwnershipOptions = QueryOwnershipOptions;
	return Node;
}

void UEIK_Ecom_QueryOwnership::OnQueryOwnershipCallback(const EOS_Ecom_QueryOwnershipCallbackInfo* Data)
{
	if (UEIK_Ecom_QueryOwnership* Node = static_cast<UEIK_Ecom_QueryOwnership*>(Data->ClientData))
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

void UEIK_Ecom_QueryOwnership::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Ecom_QueryOwnershipOptions QueryOwnershipOptions = Var_QueryOwnershipOptions.ToEOS_Ecom_QueryOwnershipOptions();
			EOS_Ecom_QueryOwnership(EOSRef->EcomHandle, &QueryOwnershipOptions, this, &UEIK_Ecom_QueryOwnership::OnQueryOwnershipCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query ownership either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_Ecom_QueryOwnershipCallbackInfo());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
