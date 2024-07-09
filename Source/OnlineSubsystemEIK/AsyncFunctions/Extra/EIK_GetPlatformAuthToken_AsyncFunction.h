// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_GetPlatformAuthToken_AsyncFunction.generated.h"

UENUM(BlueprintType)
enum EEIK_PlatformToUse
{
	Steam,
	Apple,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetPlatformAuthTokenComplete, const FString&, AuthToken);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetPlatformAuthToken_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), DisplayName="Get Steam Auth Token", Category = "EOS Integration Kit|Extra")
	static UEIK_GetPlatformAuthToken_AsyncFunction* GetPlatformAuthToken();
	
	UPROPERTY(BlueprintAssignable)
	FOnGetPlatformAuthTokenComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnGetPlatformAuthTokenComplete OnFailure;
private:
	void OnGetPlatformAuthTokenComplete(int I, bool bArg, const FExternalAuthToken& ExternalAuthToken);
	virtual void Activate() override;
	
};
