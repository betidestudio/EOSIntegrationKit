// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_InviteFriend_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInviteFriendComplete, const FString&, Error);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_InviteFriend_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Friends", DisplayName = "Add New Friend")
	static UEIK_InviteFriend_AsyncFunction* InviteFriend(FEIKUniqueNetId FriendId);
	
private:
	FEIKUniqueNetId Var_FriendId;
	void OnInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr);
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnInviteFriendComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnInviteFriendComplete OnFailure;
};
