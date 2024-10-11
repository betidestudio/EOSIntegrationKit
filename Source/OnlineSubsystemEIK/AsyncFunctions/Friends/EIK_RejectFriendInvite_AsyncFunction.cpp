// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_RejectFriendInvite_AsyncFunction.h"
#include "OnlineSubsystemUtils.h"

UEIK_RejectFriendInvite_AsyncFunction* UEIK_RejectFriendInvite_AsyncFunction::RejectFriendInvite(
	FEIKUniqueNetId FriendId)
{
	UEIK_RejectFriendInvite_AsyncFunction* UEIK_RejectFriendInviteObject= NewObject<UEIK_RejectFriendInvite_AsyncFunction>();
	UEIK_RejectFriendInviteObject->Var_FriendId = FriendId;
	return UEIK_RejectFriendInviteObject;
}

void UEIK_RejectFriendInvite_AsyncFunction::Activate()
{
	Super::Activate();
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if(const IOnlineFriendsPtr FriendsInterface = SubsystemRef->GetFriendsInterface())
		{
			FriendsInterface->RejectInvite(0,*Var_FriendId.GetUniqueNetId(),"");
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
			OnFailure.Broadcast("Failed to get Friends Interface");
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
		OnFailure.Broadcast("Failed to get Subsystem");
	}
}
