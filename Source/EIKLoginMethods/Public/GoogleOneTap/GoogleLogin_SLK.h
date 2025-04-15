// Copyright (C) 2024 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

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
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|GoogleSubsystem")
	static UGoogleLogin_SLK* GoogleLogin(UObject* WorldContextObject, const FString& ClientID);

	void Activate() override;
	void BeginDestroy() override;
	FString Var_ClientID;
	static TWeakObjectPtr<UGoogleLogin_SLK> staticInstance;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|GoogleSubsystem")
	FGoogleSignInCallback Success;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|GoogleSubsystem")
	FGoogleSignInCallback Failure;

	void GoogleLoginLocal();
};
