// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_QueryProductUserIdMappings.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_Connect_QueryProductUserIdMappings* UEIK_Connect_QueryProductUserIdMappings::
EIK_Connect_QueryProductUserIdMappings(FEIK_ProductUserId LocalUserId,
	const TArray<FEIK_ProductUserId>& TargetProductUserIds)
{
	UEIK_Connect_QueryProductUserIdMappings* Node = NewObject<UEIK_Connect_QueryProductUserIdMappings>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetProductUserIds = TargetProductUserIds;
	return Node;
}

void UEIK_Connect_QueryProductUserIdMappings::OnQueryProductUserIdMappingsCallback(
	const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* Data)
{
	if (UEIK_Connect_QueryProductUserIdMappings* Proxy = static_cast<UEIK_Connect_QueryProductUserIdMappings*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Proxy, Data]()
		{
			Proxy->OnCallback.Broadcast(FEIK_ProductUserId(Data->LocalUserId), static_cast<EEIK_Result>(Data->ResultCode));
		});
		Proxy->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Proxy->MarkAsGarbage();
#else
		Proxy->MarkPendingKill();
#endif
	}
}

void UEIK_Connect_QueryProductUserIdMappings::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_QueryProductUserIdMappingsOptions QueryProductUserIdMappingsOptions = {};
			QueryProductUserIdMappingsOptions.ApiVersion = EOS_CONNECT_QUERYPRODUCTUSERIDMAPPINGS_API_LATEST;
			QueryProductUserIdMappingsOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			QueryProductUserIdMappingsOptions.ProductUserIdCount = Var_TargetProductUserIds.Num();
			EOS_ProductUserId* TargetProductUserIds = new EOS_ProductUserId[Var_TargetProductUserIds.Num()];
			for (int i = 0; i < Var_TargetProductUserIds.Num(); i++)
			{
				TargetProductUserIds[i] = Var_TargetProductUserIds[i].GetValueAsEosType();
			}
			QueryProductUserIdMappingsOptions.ProductUserIds = TargetProductUserIds;
			EOS_Connect_QueryProductUserIdMappings(EOSRef->ConnectHandle, &QueryProductUserIdMappingsOptions, this, &UEIK_Connect_QueryProductUserIdMappings::OnQueryProductUserIdMappingsCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query product user id mappings either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_ProductUserId(), EEIK_Result::EOS_ServiceFailure);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
