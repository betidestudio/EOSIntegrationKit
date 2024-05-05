// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "eos_auth.h"
#include "eos_auth_types.h"

#include "EIK_GetIdToken_AsyncFunction.generated.h"

USTRUCT(BlueprintType, Category = "EOS Integration Kit || Login")
struct FEIKCopyAuthToken
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Login")
	FString EpicAccountId = "";

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || Login")
	FString IdToken_JWT = "";

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetIdToken_Delegate, const FEIKCopyAuthToken, EIKCopyAuthToken);


UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetIdToken_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	FString Var_EpicAccountId;

	EOS_Auth_IdToken* Var_IdToken;

	//Delegates
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Login")
	FGetIdToken_Delegate Success;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Login")
	FGetIdToken_Delegate Failure;
	
	UFUNCTION(BlueprintCallable, DisplayName = "Get EIK Auth Token", meta = (BlueprintInternalUseOnly = "true", ToolTip = "Get id token, often used for OpenID. Returns an ID token as a JSON Web Token (JWT) in string format. Only works with Epic accounts."), Category = "EOS Integration Kit || Login")
	static UEIK_GetIdToken_AsyncFunction* GetEIKAuthToken(FString EpicAccountId);

	void GetAuthToken();

	void Activate() override;
};
