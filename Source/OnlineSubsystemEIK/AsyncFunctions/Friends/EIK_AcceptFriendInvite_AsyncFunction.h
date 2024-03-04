// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_AcceptFriendInvite_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAcceptFriendInviteComplete, const FString&, Error);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_AcceptFriendInvite_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Friends", DisplayName = "Accept Friend Invite")
	static UEIK_AcceptFriendInvite_AsyncFunction* AcceptFriendInvite(FEIKUniqueNetId FriendId);

private:
	void OnAcceptComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr);
	virtual void Activate() override;
	FEIKUniqueNetId Var_FriendId;

	UPROPERTY(BlueprintAssignable)
	FOnAcceptFriendInviteComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnAcceptFriendInviteComplete OnFailure;
	
};
