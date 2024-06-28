// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_friends.h"
#include "eos_friends_types.h"
THIRD_PARTY_INCLUDES_END
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_FriendsSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_Friends_OnBlockedUsersUpdateCallback, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId, bool, bBlocked);
DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_Friends_OnFriendsUpdateCallback, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId, const TEnumAsByte<EEIK_EFriendsStatus>&, PreviousStatus, const TEnumAsByte<EEIK_EFriendsStatus>&, CurrentStatus);
UCLASS(DisplayName="Friends Interface",meta=(DisplayName="Friends Interface"))
class ONLINESUBSYSTEMEIK_API UEIK_FriendsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FEIK_Friends_OnBlockedUsersUpdateCallback OnBlockedUserUpdate;
	static void EOS_CALL OnBlockedUserUpdateCallback(const EOS_Friends_OnBlockedUsersUpdateInfo* Data);
	/*
	 *Listen for changes to blocklist for a particular account.
	 @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_AddNotifyBlockedUsersUpdate")
	FEIK_NotificationId EIK_Friends_AddNotifyBlockedUsersUpdate(FEIK_Friends_OnBlockedUsersUpdateCallback Callback);

	FEIK_Friends_OnFriendsUpdateCallback OnFriendsUpdate;
	static void EOS_CALL OnFriendsUpdateCallback(const EOS_Friends_OnFriendsUpdateInfo* Data);
	//Listen for changes to friends for a particular account.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_AddNotifyFriendsUpdate")
	FEIK_NotificationId EIK_Friends_AddNotifyFriendsUpdate(FEIK_Friends_OnFriendsUpdateCallback Callback);


	//Retrieves the Epic Account ID of an entry from the blocklist that has already been retrieved by the EOS_Friends_QueryFriends API.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_GetBlockedUserAtIndex")
	FEIK_EpicAccountId EIK_Friends_GetBlockedUserAtIndex(FEIK_EpicAccountId LocalUserId, int32 Index);

	//Retrieves the number of blocked users on the blocklist that has already been retrieved by the EOS_Friends_QueryFriends API.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_GetBlockedUsersCount")
	int32 EIK_Friends_GetBlockedUsersCount(FEIK_EpicAccountId LocalUserId);

	//Retrieves the Epic Account ID of an entry from the friends list that has already been retrieved by the EOS_Friends_QueryFriends API. The Epic Account ID returned by this function may belong to an account that has been invited to be a friend or that has invited the local user to be a friend. To determine if the Epic Account ID returned by this function is a friend or a pending friend invitation, use the EOS_Friends_GetStatus function.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_GetFriendAtIndex")
	FEIK_EpicAccountId EIK_Friends_GetFriendAtIndex(FEIK_EpicAccountId LocalUserId, int32 Index);

	//Retrieves the number of friends on the friends list that has already been retrieved by the EOS_Friends_QueryFriends API.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_GetFriendsCount")
	int32 EIK_Friends_GetFriendsCount(FEIK_EpicAccountId LocalUserId);

	//Retrieve the friendship status between the local user and another user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_GetStatus")
	TEnumAsByte<EEIK_EFriendsStatus> EIK_Friends_GetStatus(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId);

	//Stop listening for blocklist changes on a previously bound handler.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_RemoveNotifyBlockedUsersUpdate")
	void EIK_Friends_RemoveNotifyBlockedUsersUpdate(FEIK_NotificationId NotificationId);

	//Stop listening for friends changes on a previously bound handler.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_RemoveNotifyFriendsUpdate")
	void EIK_Friends_RemoveNotifyFriendsUpdate(FEIK_NotificationId NotificationId);

};

