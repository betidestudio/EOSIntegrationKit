// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_GetReportReasonDefinition.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_GetReportReasonDefinition : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Player Reports")
	static UEIK_API_GetReportReasonDefinition* GetReportReasonDefinition(FString Authorization);

private:
	
	virtual void Activate() override;
	FString Var_Authorization;
};
