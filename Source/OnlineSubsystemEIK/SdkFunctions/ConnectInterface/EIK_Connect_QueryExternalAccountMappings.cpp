// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_QueryExternalAccountMappings.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_Connect_QueryExternalAccountMappings* UEIK_Connect_QueryExternalAccountMappings::EIK_Connect_QueryExternalAccountMappings(FEIK_ProductUserId ProductUserId,
                                                                                                                               TEnumAsByte<EEIK_EExternalAccountType> AccountType, const TArray<FString>& ExternalAccountIds)
{
	UEIK_Connect_QueryExternalAccountMappings* Node = NewObject<UEIK_Connect_QueryExternalAccountMappings>();
	Node->Var_ProductUserId = ProductUserId;
	Node->Var_AccountType = AccountType;
	Node->Var_ExternalAccountIds = ExternalAccountIds;
	return Node;
}

void UEIK_Connect_QueryExternalAccountMappings::OnQueryExternalAccountMappingsCallback(
	const EOS_Connect_QueryExternalAccountMappingsCallbackInfo* Data)
{
	if (UEIK_Connect_QueryExternalAccountMappings* Proxy = static_cast<UEIK_Connect_QueryExternalAccountMappings*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Proxy, Data]()
		{
			Proxy->OnCallback.Broadcast(Proxy->Var_ProductUserId, static_cast<EEIK_Result>(Data->ResultCode));
		});
		Proxy->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Proxy->MarkAsGarbage();
#else
		Proxy->MarkPendingKill();
#endif
	}
}

void UEIK_Connect_QueryExternalAccountMappings::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_QueryExternalAccountMappingsOptions QueryExternalAccountMappingsOptions = { };
			QueryExternalAccountMappingsOptions.ApiVersion = EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_API_LATEST;
			QueryExternalAccountMappingsOptions.LocalUserId = Var_ProductUserId.GetValueAsEosType();
			QueryExternalAccountMappingsOptions.AccountIdType = static_cast<EOS_EExternalAccountType>(Var_AccountType.GetValue());
			QueryExternalAccountMappingsOptions.ExternalAccountIdCount = Var_ExternalAccountIds.Num();
			const char** ExternalAccountIds = new const char*[Var_ExternalAccountIds.Num()];
			for (int i = 0; i < Var_ExternalAccountIds.Num(); i++)
			{
				ExternalAccountIds[i] = TCHAR_TO_ANSI(*Var_ExternalAccountIds[i]);
			}
			QueryExternalAccountMappingsOptions.ExternalAccountIds = ExternalAccountIds;
			EOS_Connect_QueryExternalAccountMappings(EOSRef->ConnectHandle, &QueryExternalAccountMappingsOptions, this, &UEIK_Connect_QueryExternalAccountMappings::OnQueryExternalAccountMappingsCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query external account mappings either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_ProductUserId(), EEIK_Result::EOS_ServiceFailure);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif	
}

