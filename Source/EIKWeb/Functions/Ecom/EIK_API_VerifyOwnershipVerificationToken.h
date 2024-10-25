// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_VerifyOwnershipVerificationToken.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_VerifyOwnershipVerificationToken : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Ecom")
	static UEIK_API_VerifyOwnershipVerificationToken* VerifyOwnershipVerificationToken(FString KeyId);

private:
	
	virtual void Activate() override;
	FString Var_KeyId;
};
