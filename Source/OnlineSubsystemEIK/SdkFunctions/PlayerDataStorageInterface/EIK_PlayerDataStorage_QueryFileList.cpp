// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PlayerDataStorage_QueryFileList.h"

UEIK_PlayerDataStorage_QueryFileList* UEIK_PlayerDataStorage_QueryFileList::EIK_PlayerDataStorage_QueryFileList(
	FEIK_ProductUserId LocalUserId)
{
	UEIK_PlayerDataStorage_QueryFileList* Node = NewObject<UEIK_PlayerDataStorage_QueryFileList>();
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}

void UEIK_PlayerDataStorage_QueryFileList::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_QueryFileListOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_QUERYFILELIST_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_PlayerDataStorage_QueryFileList(EOSRef->PlayerDataStorageHandle, &Options, this, &UEIK_PlayerDataStorage_QueryFileList::EOS_PlayerDataStorage_OnQueryFileListComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_QueryFileList: OnlineSubsystemEOS is not valid"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId(), -1);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_PlayerDataStorage_QueryFileList::EOS_PlayerDataStorage_OnQueryFileListComplete(const EOS_PlayerDataStorage_QueryFileListCallbackInfo* Data)
{
	if (UEIK_PlayerDataStorage_QueryFileList* Node = static_cast<UEIK_PlayerDataStorage_QueryFileList*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->FileCount);
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}