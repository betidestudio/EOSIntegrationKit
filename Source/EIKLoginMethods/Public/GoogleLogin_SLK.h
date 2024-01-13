// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GoogleLogin_SLK.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGoogleSignInCallback, const FString&, Token, const FString&, Error);

/**
 * 
 */
UCLASS()
class EIKLOGINMETHODS_API UGoogleLogin_SLK : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UGoogleLogin_SLK* GoogleLogin(UObject* WorldContextObject, const FString& ClientID);

	void Activate() override;
	void BeginDestroy() override;
	FString Var_ClientID;
	static TWeakObjectPtr<UGoogleLogin_SLK> staticInstance;

	UPROPERTY(BlueprintAssignable, Category="GoogleSubsystem")
	FGoogleSignInCallback Success;

	UPROPERTY(BlueprintAssignable, Category="GoogleSubsystem")
	FGoogleSignInCallback Failure;

	void GoogleLoginLocal();
};
