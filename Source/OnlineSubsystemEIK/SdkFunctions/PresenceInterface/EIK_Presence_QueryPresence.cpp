// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Presence_QueryPresence.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Presence_QueryPresence* UEIK_Presence_QueryPresence::EIK_Presence_QueryPresence(FEIK_EpicAccountId LocalUserId,
                                                                                     FEIK_EpicAccountId TargetUserId)
{
	UEIK_Presence_QueryPresence* PresenceQuery = NewObject<UEIK_Presence_QueryPresence>();
	PresenceQuery->Var_LocalUserId = LocalUserId;
	PresenceQuery->Var_TargetUserId = TargetUserId;
	return PresenceQuery;
}

void UEIK_Presence_QueryPresence::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_QueryPresenceOptions Options = {};
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			EOS_Presence_QueryPresence(EOSRef->PresenceHandle, &Options, this, &UEIK_Presence_QueryPresence::Internal_OnQueryPresenceComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_Presence_QueryPresence::Activate: Unable to get EOS SDK"));
	OnCallback.Broadcast(Var_LocalUserId, Var_TargetUserId, EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Presence_QueryPresence::Internal_OnQueryPresenceComplete(const EOS_Presence_QueryPresenceCallbackInfo* Data)
{
	UEIK_Presence_QueryPresence* PresenceQuery = static_cast<UEIK_Presence_QueryPresence*>(Data->ClientData);
	if (PresenceQuery)
	{
		AsyncTask(ENamedThreads::GameThread, [PresenceQuery, Data]()
		{
			PresenceQuery->OnCallback.Broadcast(PresenceQuery->Var_LocalUserId, PresenceQuery->Var_TargetUserId, static_cast<EEIK_Result>(Data->ResultCode));
			PresenceQuery->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
PresenceQuery->MarkAsGarbage();
#else
PresenceQuery->MarkPendingKill();
#endif
		});
	}
}
