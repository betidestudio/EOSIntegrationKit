// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PlayerDataStorage_DeleteCache.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_PlayerDataStorage_DeleteCache* UEIK_PlayerDataStorage_DeleteCache::EIK_PlayerDataStorage_DeleteCache(
	FEIK_ProductUserId LocalUserId)
{
	UEIK_PlayerDataStorage_DeleteCache* Node = NewObject<UEIK_PlayerDataStorage_DeleteCache>();
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}
void UEIK_PlayerDataStorage_DeleteCache::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_DeleteCacheOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_DELETECACHE_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_PlayerDataStorage_DeleteCache(EOSRef->PlayerDataStorageHandle, &Options, this, &UEIK_PlayerDataStorage_DeleteCache::EOS_PlayerDataStorage_OnDeleteCacheComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_DeleteCache: OnlineSubsystemEOS is not valid"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_PlayerDataStorage_DeleteCache::EOS_PlayerDataStorage_OnDeleteCacheComplete(
	const EOS_PlayerDataStorage_DeleteCacheCallbackInfo* Data)
{
	if (UEIK_PlayerDataStorage_DeleteCache* Node = static_cast<UEIK_PlayerDataStorage_DeleteCache*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId);
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}
