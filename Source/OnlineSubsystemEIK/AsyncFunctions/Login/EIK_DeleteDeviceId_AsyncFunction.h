// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "OnlineSubsystemEOS.h"
#include "eos_connect.h"
#include "eos_connect_types.h"

#include "EIK_DeleteDeviceId_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeleteDeviceIDResponsDelegate);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_DeleteDeviceId_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Login")
	static UEIK_DeleteDeviceId_AsyncFunction* DeleteDeviceId();

	static void OnDeleteDeviceIdComplete(const EOS_Connect_DeleteDeviceIdCallbackInfo* Data);
	virtual void Activate() override;


	UPROPERTY(BlueprintAssignable)
	FDeleteDeviceIDResponsDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FDeleteDeviceIDResponsDelegate OnFailure;
};
