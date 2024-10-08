// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_SessionSearch_Find.h"

#include "Async/Async.h"

UEIK_SessionSearch_Find* UEIK_SessionSearch_Find::EIK_SessionSearch_Find(FEIK_HSessionSearch SessionSearchHandle,
                                                                         FEIK_ProductUserId LocalUserId)
{
	UEIK_SessionSearch_Find* Node = NewObject<UEIK_SessionSearch_Find>();
	Node->Var_SessionSearchHandle = SessionSearchHandle;
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}

void UEIK_SessionSearch_Find::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(!Var_SessionSearchHandle.Ref)
			{
				UE_LOG(LogEIK, Error, TEXT("Failed to find session because SessionSearchHandle is not valid."));
				OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
				SetReadyToDestroy();
				#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
				return;
			}
			EOS_SessionSearch_FindOptions SessionSearchFindOptions = { };
			SessionSearchFindOptions.ApiVersion = EOS_SESSIONSEARCH_FIND_API_LATEST;
			SessionSearchFindOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_SessionSearch_Find(*Var_SessionSearchHandle.Ref, &SessionSearchFindOptions, this, &UEIK_SessionSearch_Find::OnSessionSearch_FindCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to find session either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
void UEIK_SessionSearch_Find::OnSessionSearch_FindCallback(const EOS_SessionSearch_FindCallbackInfo* Data)
{
	if (UEIK_SessionSearch_Find* Node = static_cast<UEIK_SessionSearch_Find*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
