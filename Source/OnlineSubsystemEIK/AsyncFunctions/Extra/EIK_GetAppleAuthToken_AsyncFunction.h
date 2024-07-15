// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "UserManagerEOS.h"
#include "EIK_GetAppleAuthToken_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnGetAppleAuthTokenComplete, const FString&, AuthToken);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetAppleAuthToken_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), DisplayName="Get Apple Auth Token", Category = "EOS Integration Kit|Extra")
	static UEIK_GetAppleAuthToken_AsyncFunction* GetAppleAuthToken();

	UPROPERTY(BlueprintAssignable)
	FEIK_OnGetAppleAuthTokenComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEIK_OnGetAppleAuthTokenComplete OnFailure;

private:
	void OnLoginComplete(int LocalUserNum, bool bWasSuccess, const FUniqueNetId& UniqueNetId, const FString& Error);
	virtual void Activate() override;
};
