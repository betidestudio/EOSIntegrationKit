// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_TransferDeviceIdAccount.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_Connect_TransferDeviceIdAccount* UEIK_Connect_TransferDeviceIdAccount::EIK_Connect_TransferDeviceIdAccount(
	FEIK_ProductUserId PrimaryLocalUserId, FEIK_ProductUserId LocalUserId, FEIK_ProductUserId ProductUserIdToPreserve)
{
	UEIK_Connect_TransferDeviceIdAccount* Node = NewObject<UEIK_Connect_TransferDeviceIdAccount>();
	Node->Var_PrimaryLocalUserId = PrimaryLocalUserId;
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_ProductUserIdToPreserve = ProductUserIdToPreserve;
	return Node;
}

void UEIK_Connect_TransferDeviceIdAccount::OnTransferDeviceIdAccountCallback(
	const EOS_Connect_TransferDeviceIdAccountCallbackInfo* Data)
{
	if (UEIK_Connect_TransferDeviceIdAccount* Proxy = static_cast<UEIK_Connect_TransferDeviceIdAccount*>(Data->ClientData))
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

void UEIK_Connect_TransferDeviceIdAccount::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_TransferDeviceIdAccountOptions TransferDeviceIdAccountOptions = {};
			TransferDeviceIdAccountOptions.ApiVersion = EOS_CONNECT_TRANSFERDEVICEIDACCOUNT_API_LATEST;
			TransferDeviceIdAccountOptions.LocalDeviceUserId = Var_LocalUserId.GetValueAsEosType();
			TransferDeviceIdAccountOptions.ProductUserIdToPreserve = Var_ProductUserIdToPreserve.GetValueAsEosType();
			TransferDeviceIdAccountOptions.PrimaryLocalUserId = Var_PrimaryLocalUserId.GetValueAsEosType();
			EOS_Connect_TransferDeviceIdAccount(EOSRef->ConnectHandle, &TransferDeviceIdAccountOptions, this, &UEIK_Connect_TransferDeviceIdAccount::OnTransferDeviceIdAccountCallback);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to transfer device id account either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_ProductUserId(), EEIK_Result::EOS_ServiceFailure);
}
