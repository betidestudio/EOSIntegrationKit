// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_FindPlayerReports.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_FindPlayerReports : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Player Reports")
	static UEIK_API_FindPlayerReports* FindPlayerReports(FString Authorization, FString DeploymentId, FString ReportingPlayerId, FString ReportedPlayerId, int32 ReasonId, FString StartTime, FString EndTime, bool bPagination, int32 Offset = 0, int32 Limit = 50, FString Order = "time:desc");

private:
	
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	FString Var_ReportingPlayerId;
	FString Var_ReportedPlayerId;
	int32 Var_ReasonId;
	FString Var_StartTime;
	FString Var_EndTime;
	bool Var_bPagination;
	int32 Var_Offset;
	int32 Var_Limit;
	FString Var_Order;
};
