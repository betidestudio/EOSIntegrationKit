// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_KickMember.h"

UEIK_Lobby_KickMember* UEIK_Lobby_KickMember::EIK_Lobby_KickMember(FEIK_LobbyId LobbyId,
	FEIK_ProductUserId ProductUserId, FEIK_ProductUserId TargetUserId)
{
	UEIK_Lobby_KickMember* Node = NewObject<UEIK_Lobby_KickMember>();
	Node->Var_LobbyId = LobbyId;
	Node->Var_ProductUserId = ProductUserId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_Lobby_KickMember::OnKickMemberComplete(const EOS_Lobby_KickMemberCallbackInfo* Data)
{
	if (UEIK_Lobby_KickMember* Node = static_cast<UEIK_Lobby_KickMember*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LobbyId);
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}

void UEIK_Lobby_KickMember::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_KickMemberOptions Options = { };
			Options.ApiVersion = EOS_LOBBY_KICKMEMBER_API_LATEST;
			Options.LobbyId = Var_LobbyId.GetValueAsEosType();
			Options.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			Options.LocalUserId = Var_ProductUserId.GetValueAsEosType();
			EOS_Lobby_KickMember(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_KickMember::OnKickMemberComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to kick member either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
