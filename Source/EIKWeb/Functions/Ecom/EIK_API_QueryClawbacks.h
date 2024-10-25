// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_QueryClawbacks.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_QueryClawbacks : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Ecom")
	static UEIK_API_QueryClawbacks* QueryClawbacks(FString Authorization, FString NameSpace, FString ClawbackDate, int32 Count=10);

private:
	
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_NameSpace;
	FString Var_ClawbackDate;
	int32 Var_Count;
};
