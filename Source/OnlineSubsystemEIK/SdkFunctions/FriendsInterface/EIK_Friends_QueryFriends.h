// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
THIRD_PARTY_INCLUDES_START
#include "eos_friends.h"
#include "eos_friends_types.h"
THIRD_PARTY_INCLUDES_END
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Friends_QueryFriends.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Friends_QueryFriendsCallback, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_EpicAccountId&, LocalUserId);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Friends_QueryFriends : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/*
	 *Starts an asynchronous task that reads the user's friends list and blocklist from the backend service, caching it for future use.

	When the Social Overlay is enabled then this will be called automatically. The Social Overlay is enabled by default (see EOS_PF_DISABLE_SOCIAL_OVERLAY).
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_QueryFriends")
	static UEIK_Friends_QueryFriends* EIK_Friends_QueryFriends(FEIK_EpicAccountId LocalUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Friends Interface")
	FEIK_Friends_QueryFriendsCallback OnCallback;

private:
	static void EOS_CALL OnQueryFriendsCallback(const EOS_Friends_QueryFriendsCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_EpicAccountId Var_LocalUserId;
};
