// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Friends_QueryFriends.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Friends_QueryFriends* UEIK_Friends_QueryFriends::EIK_Friends_QueryFriends(FEIK_EpicAccountId LocalUserId)
{
	UEIK_Friends_QueryFriends* Node = NewObject<UEIK_Friends_QueryFriends>();
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}

void UEIK_Friends_QueryFriends::OnQueryFriendsCallback(const EOS_Friends_QueryFriendsCallbackInfo* Data)
{
	if (UEIK_Friends_QueryFriends* Node = static_cast<UEIK_Friends_QueryFriends*>(Data->ClientData))
	{
		Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId);
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Friends_QueryFriends::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_QueryFriendsOptions QueryFriendsOptions = {};
			QueryFriendsOptions.ApiVersion = EOS_FRIENDS_QUERYFRIENDS_API_LATEST;
			QueryFriendsOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_Friends_QueryFriends(EOSRef->FriendsHandle, &QueryFriendsOptions, this, &UEIK_Friends_QueryFriends::OnQueryFriendsCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query friends either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_EpicAccountId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
