// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Ecom_Checkout.h"

#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"


UEIK_Ecom_Checkout* UEIK_Ecom_Checkout::EIK_Ecom_Checkout(FEIK_Ecom_CheckoutOptions CheckoutOptions)
{
	UEIK_Ecom_Checkout* Node = NewObject<UEIK_Ecom_Checkout>();
	Node->Local_CheckoutOptions = CheckoutOptions;
	return Node;
}

void UEIK_Ecom_Checkout::OnCheckoutCallback(const EOS_Ecom_CheckoutCallbackInfo* Data)
{
	if(UEIK_Ecom_Checkout* Node = static_cast<UEIK_Ecom_Checkout*>(Data->ClientData))
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

void UEIK_Ecom_Checkout::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Ecom_CheckoutOptions CheckoutOptions = Local_CheckoutOptions.ToEOS_Ecom_CheckoutOptions();
			EOS_Ecom_Checkout(EOSRef->EcomHandle, &CheckoutOptions, this, &UEIK_Ecom_Checkout::OnCheckoutCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to checkout item either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_Ecom_CheckoutCallbackInfo());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
