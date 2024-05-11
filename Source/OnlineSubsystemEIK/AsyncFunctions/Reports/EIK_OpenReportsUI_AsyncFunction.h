// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_ui.h"
#include "eos_ui_types.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_OpenReportsUI_AsyncFunction.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReportsUIDelegate);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_OpenReportsUI_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	//PUID's'
	FString LocalReporterPUID;
	FString TargetPlayerPUID;

	//Delegates
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Reports")
	FReportsUIDelegate Success;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit || Reports")
	FReportsUIDelegate Failure;

	UFUNCTION(BlueprintCallable, DisplayName = "Show Epic Player Report UI", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || Reports")
	static UEIK_OpenReportsUI_AsyncFunction* ShowEIKPlayerReportUIAsyncFunction(FString LocalReporterPUID, FString TargetPlayerPUID);

	void SendReportFunc();

	static void EOS_CALL SendReportFuncCallback(const EOS_UI_OnShowReportPlayerCallbackInfo* Data);

	void ResultFaliure();

	void ResultSuccess();

	void Activate() override;
};
