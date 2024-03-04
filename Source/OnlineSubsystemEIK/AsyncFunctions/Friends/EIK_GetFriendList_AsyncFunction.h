// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_GetFriendList_AsyncFunction.generated.h"


USTRUCT(BlueprintType)
struct FEIK_FriendData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Friends")
	FString DisplayName = ""; 

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Friends")
	bool bIsOnline = false;
	
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Friends")
	FString InviteStatus = "";

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Friends")
	FEIKUniqueNetId UserId = FEIKUniqueNetId();

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Friends")
	FString PresenceStatus = "";
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetFriendListComplete,const TArray<FEIK_FriendData>&, FriendList, FString, Error);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetFriendList_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Friends")
	static UEIK_GetFriendList_AsyncFunction* GetFriendList();
	
private:
	void OnFriendsReadComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnGetFriendListComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnGetFriendListComplete OnFailure;
	
};
