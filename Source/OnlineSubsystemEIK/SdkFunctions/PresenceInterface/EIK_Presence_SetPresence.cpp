// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Presence_SetPresence.h"

#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Presence_SetPresence* UEIK_Presence_SetPresence::EIK_Presence_SetPresence(FEIK_EpicAccountId LocalUserId,
                                                                               FEIK_HPresenceModification PresenceModificationHandle)
{
	UEIK_Presence_SetPresence* Node = NewObject<UEIK_Presence_SetPresence>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_PresenceModificationHandle = PresenceModificationHandle;
	return Node;
}

void UEIK_Presence_SetPresence::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_SetPresenceOptions Options = {};
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.PresenceModificationHandle = Var_PresenceModificationHandle.Ref;
			EOS_Presence_SetPresence(EOSRef->PresenceHandle, &Options, this, &UEIK_Presence_SetPresence::Internal_OnSetPresenceComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_Presence_SetPresence::Activate: Unable to get EOS SDK"));
	OnCallback.Broadcast(Var_LocalUserId, EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Presence_SetPresence::Internal_OnSetPresenceComplete(const EOS_Presence_SetPresenceCallbackInfo* Data)
{
	if (UEIK_Presence_SetPresence* Node = static_cast<UEIK_Presence_SetPresence*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(Node->Var_LocalUserId, static_cast<EEIK_Result>(Data->ResultCode));
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}
