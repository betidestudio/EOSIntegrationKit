// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Ecom_QueryOffers.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Ecom_QueryOffers* UEIK_Ecom_QueryOffers::EIK_Ecom_QueryOffers(FEIK_EpicAccountId LocalUserId,
	FString OverrideCatalogNamespace)
{
	UEIK_Ecom_QueryOffers* Node = NewObject<UEIK_Ecom_QueryOffers>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_OverrideCatalogNamespace = OverrideCatalogNamespace;
	return Node;
}

void UEIK_Ecom_QueryOffers::OnQueryOffersCallback(const EOS_Ecom_QueryOffersCallbackInfo* Data)
{
	if (UEIK_Ecom_QueryOffers* Node = static_cast<UEIK_Ecom_QueryOffers*>(Data->ClientData))
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
void UEIK_Ecom_QueryOffers::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Ecom_QueryOffersOptions QueryOffersOptions;
			QueryOffersOptions.ApiVersion = EOS_ECOM_QUERYOFFERS_API_LATEST;
			QueryOffersOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			if(Var_OverrideCatalogNamespace.IsEmpty())
			{
				QueryOffersOptions.OverrideCatalogNamespace = nullptr;
			}
			else
			{
				QueryOffersOptions.OverrideCatalogNamespace = TCHAR_TO_ANSI(*Var_OverrideCatalogNamespace);
			}			
			EOS_Ecom_QueryOffers(EOSRef->EcomHandle, &QueryOffersOptions, this, &UEIK_Ecom_QueryOffers::OnQueryOffersCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query offers either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_EpicAccountId(), EEIK_Result::EOS_ServiceFailure);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
