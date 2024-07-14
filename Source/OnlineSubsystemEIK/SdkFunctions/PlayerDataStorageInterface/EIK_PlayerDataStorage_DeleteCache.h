// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_PlayerDataStorage_DeleteCache.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_PlayerDataStorage_DeleteCacheDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_ProductUserId&, LocalUserId);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorage_DeleteCache : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Clear previously cached file data. This operation will be done asynchronously. All cached files except those corresponding to the transfers in progress will be removed. Warning: Use this with care. Cache system generally tries to clear old and unused cached files from time to time. Unnecessarily clearing cache can degrade performance as SDK will have to re-download data.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_DeleteCache")
	static UEIK_PlayerDataStorage_DeleteCache* EIK_PlayerDataStorage_DeleteCache(FEIK_ProductUserId LocalUserId);

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_DeleteCacheDelegate OnCallback;
private:
	virtual void Activate() override;
	FEIK_ProductUserId Var_LocalUserId;
	static void EOS_CALL EOS_PlayerDataStorage_OnDeleteCacheComplete(const EOS_PlayerDataStorage_DeleteCacheCallbackInfo* Data);
};
