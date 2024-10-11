// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Ecom_RedeemEntitlements.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Ecom_RedeemEntitlements* UEIK_Ecom_RedeemEntitlements::EIK_Ecom_RedeemEntitlements(
	const FEIK_EpicAccountId& LocalUserId, const TArray<FEIK_Ecom_EntitlementId>& EntitlementIds)
{
	UEIK_Ecom_RedeemEntitlements* Node = NewObject<UEIK_Ecom_RedeemEntitlements>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_EntitlementIds = EntitlementIds;
	return Node;
}

void UEIK_Ecom_RedeemEntitlements::OnRedeemEntitlementsCallback(const EOS_Ecom_RedeemEntitlementsCallbackInfo* Data)
{
	if (UEIK_Ecom_RedeemEntitlements* Node = static_cast<UEIK_Ecom_RedeemEntitlements*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(Data->LocalUserId, static_cast<EEIK_Result>(Data->ResultCode), Data->RedeemedEntitlementIdsCount);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Ecom_RedeemEntitlements::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Ecom_RedeemEntitlementsOptions RedeemEntitlementsOptions;
			RedeemEntitlementsOptions.ApiVersion = EOS_ECOM_REDEEMENTITLEMENTS_API_LATEST;
			RedeemEntitlementsOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			RedeemEntitlementsOptions.EntitlementIdCount = Var_EntitlementIds.Num();
			RedeemEntitlementsOptions.EntitlementIds = new EOS_Ecom_EntitlementId[Var_EntitlementIds.Num()];
			for (int32 i = 0; i < Var_EntitlementIds.Num(); i++)
			{
				RedeemEntitlementsOptions.EntitlementIds[i] = Var_EntitlementIds[i].Ref;
			}
			EOS_Ecom_RedeemEntitlements(EOSRef->EcomHandle, &RedeemEntitlementsOptions, this, &UEIK_Ecom_RedeemEntitlements::OnRedeemEntitlementsCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to redeem entitlements either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_EpicAccountId(), EEIK_Result::EOS_Disabled, 0);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
