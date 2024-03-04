// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_RejectFriendInvite_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRejectFriendInviteComplete, const FString&, Error);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_RejectFriendInvite_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Friends", DisplayName = "Reject Friend Invite")
	static UEIK_RejectFriendInvite_AsyncFunction* RejectFriendInvite(FEIKUniqueNetId FriendId);

private:
	FEIKUniqueNetId Var_FriendId;
	virtual void Activate() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnRejectFriendInviteComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnRejectFriendInviteComplete OnFailure;
};
