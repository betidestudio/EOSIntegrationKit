// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "EIK_FriendSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_FriendSubsystem : public USubsystem
{
	GENERATED_BODY()

public:

	void AddFriend(FString FriendProductId);
	void RemoveFriend(FString FriendProductId);
	void GetFriends();
	void GetFriendsStatus();
	void GetFriendsPresence();
	void GetFriendsAvatar();







	void OnWriteFileComplete(bool bSuccess, const FUniqueNetId& UserID, const FString& Var_FileName);

};
