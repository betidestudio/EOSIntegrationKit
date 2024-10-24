// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_SyncSanctionsToExternalService.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_SyncSanctionsToExternalService : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_SyncSanctionsToExternalService* SyncSanctionsToExternalService(FString Authorization, FString LastLogId);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_LastLogId;
};
