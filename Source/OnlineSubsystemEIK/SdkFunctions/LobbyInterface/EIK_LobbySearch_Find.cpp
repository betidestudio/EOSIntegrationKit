// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_LobbySearch_Find.h"


UEIK_LobbySearch_Find* UEIK_LobbySearch_Find::EIK_LobbySearch_Find(FEIK_HLobbySearch Handle,
	FEIK_ProductUserId LocalUserId)
{
	UEIK_LobbySearch_Find* Node = NewObject<UEIK_LobbySearch_Find>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_Handle = Handle;
	return Node;
}

void UEIK_LobbySearch_Find::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_FindOptions Options = { };
			Options.ApiVersion = EOS_LOBBYSEARCH_FIND_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_LobbySearch_Find(Var_Handle.Ref, &Options, this, &UEIK_LobbySearch_Find::OnFindComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to find lobby either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_LobbySearch_Find::OnFindComplete(const EOS_LobbySearch_FindCallbackInfo* Data)
{
	if (UEIK_LobbySearch_Find* Node = static_cast<UEIK_LobbySearch_Find*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}
