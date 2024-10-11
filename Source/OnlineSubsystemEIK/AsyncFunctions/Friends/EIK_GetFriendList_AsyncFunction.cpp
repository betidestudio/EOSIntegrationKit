// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_GetFriendList_AsyncFunction.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlinePresenceInterface.h"

UEIK_GetFriendList_AsyncFunction* UEIK_GetFriendList_AsyncFunction::GetFriendList()
{
	UEIK_GetFriendList_AsyncFunction* UEIK_GetFriendListObject= NewObject<UEIK_GetFriendList_AsyncFunction>();
	return UEIK_GetFriendListObject;
}

void UEIK_GetFriendList_AsyncFunction::OnFriendsReadComplete(int32 LocalUserNum, bool bWasSuccessful,
	const FString& ListName, const FString& ErrorStr)
{
	if(bWasSuccessful)
	{
		TArray<FEIK_FriendData> FriendList = {};
		if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
		{
			if(const IOnlineFriendsPtr FriendsInterface = SubsystemRef->GetFriendsInterface())
			{
				TArray<TSharedRef<FOnlineFriend>> Friends;
				FriendsInterface->GetFriendsList(LocalUserNum, ListName, Friends);
				for(const TSharedRef<FOnlineFriend>& Friend : Friends)
				{
					FEIK_FriendData FriendData;
					FriendData.DisplayName = Friend->GetDisplayName();
					FriendData.bIsOnline = Friend->GetPresence().bIsOnline;
					FriendData.InviteStatus = ToString(Friend->GetInviteStatus());
					FEIKUniqueNetId FriendId;
					FriendId.SetUniqueNetId(Friend->GetUserId());
					FriendData.UserId = FriendId;
					FriendList.Add(FriendData);
				}
				OnSuccess.Broadcast(FriendList, "");
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif		
			}
			else
			{
				OnFailure.Broadcast(TArray<FEIK_FriendData>(), "Failed to get Friends Interface");
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
			OnFailure.Broadcast(TArray<FEIK_FriendData>(), "Failed to get Subsystem");
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
		OnFailure.Broadcast(TArray<FEIK_FriendData>(), ErrorStr);
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
	}

void UEIK_GetFriendList_AsyncFunction::Activate()
{
	Super::Activate();
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if(const IOnlineFriendsPtr FriendsInterface = SubsystemRef->GetFriendsInterface())
		{
			FriendsInterface->ReadFriendsList(0, "",FOnReadFriendsListComplete::CreateUObject(this, &UEIK_GetFriendList_AsyncFunction::OnFriendsReadComplete));
		}
		else
		{
			OnFailure.Broadcast(TArray<FEIK_FriendData>(), "Failed to get Friends Interface");
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
		OnFailure.Broadcast(TArray<FEIK_FriendData>(), "Failed to get Subsystem");
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}
