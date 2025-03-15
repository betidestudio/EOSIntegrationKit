// Copyright (C) 2024 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GoogleLogout_SLK.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoogleSignOutCallback, const FString&, Error);

/**
 * 
 */
UCLASS()
class EIKLOGINMETHODS_API UGoogleLogout_SLK : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|GoogleSubsystem")
	static UGoogleLogout_SLK* GoogleLogout(UObject* WorldContextObject);

	void Activate() override;
	void BeginDestroy() override;
	static TWeakObjectPtr<UGoogleLogout_SLK> staticInstance;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|GoogleSubsystem")
	FGoogleSignOutCallback Success;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|GoogleSubsystem")
	FGoogleSignOutCallback Failure;

	void GoogleLogoutLocal();
};
