// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "eos_connect.h"
#include "eos_connect_types.h"
#include "EIK_GetPUIDFromEpicId_AsyncFunc.generated.h"

USTRUCT(BlueprintType)
struct FProductUserIdAndEpicId
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	FString EpicAccountId = "";

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit || UserInfo")
	FString ProductUserId = "";

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetPUIDFromEpicIdDelegate, const TArray<FProductUserIdAndEpicId>&, UserInfo);


UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetPUIDFromEpicId_AsyncFunc : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, DisplayName = "Get Product User Id From Epic Id", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || UserInfo")
	static UEIK_GetPUIDFromEpicId_AsyncFunc* EIKGetPUIDFromEpicId(TArray<FString> TargetEpicAccountIds, FString LocalProductUserId);

	TArray<FString> Var_TargetUserIds;

	FString Var_LocalProductUserId;

	void GetPUIDFromEpicId();

	static void OnQueryExternalAccountMappingsComplete(const EOS_Connect_QueryExternalAccountMappingsCallbackInfo* Data);

	void QueryExternalAccountMappingsSuccess();

	void QueryExternalAccountMappingsFailure();

	virtual void Activate() override; 


	UPROPERTY(BlueprintAssignable)
	FGetPUIDFromEpicIdDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetPUIDFromEpicIdDelegate Failure;
	
};
