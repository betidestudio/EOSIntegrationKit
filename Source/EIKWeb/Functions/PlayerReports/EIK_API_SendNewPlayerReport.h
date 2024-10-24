// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_SendNewPlayerReport.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_SendNewPlayerReport : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Player Reports")
	static UEIK_API_SendNewPlayerReport* SendNewPlayerReport(FString Authorization, FString ReportingPlayerId, FString ReportedPlayerId, FString Time, int32 ReasonId, FString Message, FString Context);

private:
	
	virtual void Activate() override;
	FString Var_ReportedPlayerId;
	FString Var_ReportingPlayerId;
	FString Var_Time;
	int32 Var_ReasonId;
	FString Var_Message;
	FString Var_Authorization;
	FString Var_Context;
};
