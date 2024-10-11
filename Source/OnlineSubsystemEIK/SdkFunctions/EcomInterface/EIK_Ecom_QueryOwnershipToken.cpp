// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Ecom_QueryOwnershipToken.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Ecom_QueryOwnershipToken* UEIK_Ecom_QueryOwnershipToken::EIK_Ecom_QueryOwnershipToken(
	FEIK_Ecom_QueryOwnershipTokenOptions QueryOwnershipTokenOptions)
{
	UEIK_Ecom_QueryOwnershipToken* Node = NewObject<UEIK_Ecom_QueryOwnershipToken>();
	Node->Var_QueryOwnershipTokenOptions = QueryOwnershipTokenOptions;
	return Node;
}

void UEIK_Ecom_QueryOwnershipToken::OnQueryOwnershipTokenCallback(const EOS_Ecom_QueryOwnershipTokenCallbackInfo* Data)
{
	if (UEIK_Ecom_QueryOwnershipToken* Node = static_cast<UEIK_Ecom_QueryOwnershipToken*>(Data->ClientData))
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

void UEIK_Ecom_QueryOwnershipToken::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Ecom_QueryOwnershipTokenOptions QueryOwnershipTokenOptions = Var_QueryOwnershipTokenOptions.ToEOS_Ecom_QueryOwnershipOptions();
			EOS_Ecom_QueryOwnershipToken(EOSRef->EcomHandle, &QueryOwnershipTokenOptions, this, &UEIK_Ecom_QueryOwnershipToken::OnQueryOwnershipTokenCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query ownership token either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_Ecom_QueryOwnershipTokenCallbackInfo());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
