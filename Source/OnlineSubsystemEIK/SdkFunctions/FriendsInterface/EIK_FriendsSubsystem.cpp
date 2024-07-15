// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_FriendsSubsystem.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

void UEIK_FriendsSubsystem::OnBlockedUserUpdateCallback(const EOS_Friends_OnBlockedUsersUpdateInfo* Data)
{
	if(UEIK_FriendsSubsystem* Subsystem = static_cast<UEIK_FriendsSubsystem*>(Data->ClientData))
	{
		{
			AsyncTask(ENamedThreads::GameThread, [Data, Subsystem]()
			{
				bool LocalData = false;
				if (Data->bBlocked == EOS_TRUE)
				{
					LocalData = true;
				}
				Subsystem->OnBlockedUserUpdate.ExecuteIfBound(Data->LocalUserId, Data->TargetUserId, LocalData);
			});
		}
	}
}

FEIK_NotificationId UEIK_FriendsSubsystem::EIK_Friends_AddNotifyBlockedUsersUpdate(FEIK_Friends_OnBlockedUsersUpdateCallback Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			OnBlockedUserUpdate = Callback;
			EOS_Friends_AddNotifyBlockedUsersUpdateOptions Options = {};
			Options.ApiVersion = EOS_FRIENDS_ADDNOTIFYBLOCKEDUSERSUPDATE_API_LATEST;
			return EOS_Friends_AddNotifyBlockedUsersUpdate(EOSRef->FriendsHandle, &Options, this, &UEIK_FriendsSubsystem::OnBlockedUserUpdateCallback);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to add notify blocked users update either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_NotificationId();
}

void UEIK_FriendsSubsystem::OnFriendsUpdateCallback(const EOS_Friends_OnFriendsUpdateInfo* Data)
{
	if(UEIK_FriendsSubsystem* Subsystem = static_cast<UEIK_FriendsSubsystem*>(Data->ClientData))
	{
		{
			AsyncTask(ENamedThreads::GameThread, [Data, Subsystem]()
			{
				Subsystem->OnFriendsUpdate.ExecuteIfBound(Data->LocalUserId, Data->TargetUserId, static_cast<EEIK_EFriendsStatus>(Data->PreviousStatus), static_cast<EEIK_EFriendsStatus>(Data->CurrentStatus));
			});
		}
	}
}

FEIK_NotificationId UEIK_FriendsSubsystem::EIK_Friends_AddNotifyFriendsUpdate(
	FEIK_Friends_OnFriendsUpdateCallback Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			OnFriendsUpdate = Callback;
			EOS_Friends_AddNotifyFriendsUpdateOptions Options = {};
			Options.ApiVersion = EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST;
			return EOS_Friends_AddNotifyFriendsUpdate(EOSRef->FriendsHandle, &Options, this, &UEIK_FriendsSubsystem::OnFriendsUpdateCallback);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to add notify friends update either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_NotificationId();
}

FEIK_EpicAccountId UEIK_FriendsSubsystem::EIK_Friends_GetBlockedUserAtIndex(FEIK_EpicAccountId LocalUserId, int32 Index)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_GetBlockedUserAtIndexOptions Options = {};
			Options.ApiVersion = EOS_FRIENDS_GETBLOCKEDUSERATINDEX_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.Index = Index;
			return EOS_Friends_GetBlockedUserAtIndex(EOSRef->FriendsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get blocked user at index either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_EpicAccountId();
}

int32 UEIK_FriendsSubsystem::EIK_Friends_GetBlockedUsersCount(FEIK_EpicAccountId LocalUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_GetBlockedUsersCountOptions Options = {};
			Options.ApiVersion = EOS_FRIENDS_GETBLOCKEDUSERSCOUNT_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			return EOS_Friends_GetBlockedUsersCount(EOSRef->FriendsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get blocked users count either OnlineSubsystem is not valid or EOSRef is not valid."));
	return 0;
}

FEIK_EpicAccountId UEIK_FriendsSubsystem::EIK_Friends_GetFriendAtIndex(FEIK_EpicAccountId LocalUserId, int32 Index)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_GetFriendAtIndexOptions Options = {};
			Options.ApiVersion = EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.Index = Index;
			return EOS_Friends_GetFriendAtIndex(EOSRef->FriendsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get friend at index either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_EpicAccountId();
}

int32 UEIK_FriendsSubsystem::EIK_Friends_GetFriendsCount(FEIK_EpicAccountId LocalUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_GetFriendsCountOptions Options = {};
			Options.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			return EOS_Friends_GetFriendsCount(EOSRef->FriendsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get friends count either OnlineSubsystem is not valid or EOSRef is not valid."));
	return 0;
}

TEnumAsByte<EEIK_EFriendsStatus> UEIK_FriendsSubsystem::EIK_Friends_GetStatus(FEIK_EpicAccountId LocalUserId,
	FEIK_EpicAccountId TargetUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_GetStatusOptions Options = {};
			Options.ApiVersion = EOS_FRIENDS_GETSTATUS_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			return static_cast<EEIK_EFriendsStatus>(EOS_Friends_GetStatus(EOSRef->FriendsHandle, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get friends status either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_EFriendsStatus::EIK_FS_NotFriends;
}

void UEIK_FriendsSubsystem::EIK_Friends_RemoveNotifyBlockedUsersUpdate(FEIK_NotificationId NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_RemoveNotifyBlockedUsersUpdate(EOSRef->FriendsHandle, NotificationId.NotificationId);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to remove notify blocked users update either OnlineSubsystem is not valid or EOSRef is not valid."));
}

void UEIK_FriendsSubsystem::EIK_Friends_RemoveNotifyFriendsUpdate(FEIK_NotificationId NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Friends_RemoveNotifyFriendsUpdate(EOSRef->FriendsHandle, NotificationId.NotificationId);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to remove notify friends update either OnlineSubsystem is not valid or EOSRef is not valid."));
}
