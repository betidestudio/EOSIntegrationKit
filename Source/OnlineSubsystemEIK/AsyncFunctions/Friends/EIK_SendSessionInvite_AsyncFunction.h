// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_SendSessionInvite_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendSessionInviteComplete, const FString&, Error);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SendSessionInvite_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Friends", DisplayName = "Send Session Invite")
	static UEIK_SendSessionInvite_AsyncFunction* SendSessionInvite(
		FName SessionName,
		FEIKUniqueNetId FriendId);

private:
	FEIKUniqueNetId Var_FriendId;
	FName Var_SessionName;
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnSendSessionInviteComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnSendSessionInviteComplete OnFailure;	
};
