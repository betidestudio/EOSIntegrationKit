// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_InviteFriend_AsyncFunction.h"
#include "OnlineSubsystemUtils.h"

UEIK_InviteFriend_AsyncFunction* UEIK_InviteFriend_AsyncFunction::InviteFriend(FEIKUniqueNetId FriendId)
{
	UEIK_InviteFriend_AsyncFunction* UEIK_InviteFriendObject= NewObject<UEIK_InviteFriend_AsyncFunction>();
	UEIK_InviteFriendObject->Var_FriendId = FriendId;
	return UEIK_InviteFriendObject;
}

void UEIK_InviteFriend_AsyncFunction::OnInviteComplete(int32 LocalUserNum, bool bWasSuccessful,
	const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr)
{
	if(bWasSuccessful)
	{
		OnSuccess.Broadcast("");
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
	else
	{
		OnFailure.Broadcast(ErrorStr);
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}

void UEIK_InviteFriend_AsyncFunction::Activate()
{
	Super::Activate();
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if(const IOnlineFriendsPtr FriendsInterface = SubsystemRef->GetFriendsInterface())
		{
			FriendsInterface->SendInvite(0,*Var_FriendId.GetUniqueNetId(),"",FOnSendInviteComplete::CreateUObject(this, &UEIK_InviteFriend_AsyncFunction::OnInviteComplete));
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
		else
		{
			// Failed to get Friends Interface
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
	else
	{
		// Failed to get Subsystem
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}
