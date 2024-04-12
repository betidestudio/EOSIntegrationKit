// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_GetTitleData_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGetTitleDataResult, bool, bWasSuccess, int64, ProgressPercentage,const TArray<uint8>&,Data);

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetTitleData_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FGetTitleDataResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FGetTitleDataResult OnProgress;
	
	UPROPERTY(BlueprintAssignable)
	FGetTitleDataResult OnFail;

	bool bDelegateCalled = false;

	FString FileName;

	/*
	This C++ method gets the player data in the online subsystem using the selected method and sets up a callback function to handle the response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/playerdata/
	For Input Parameters, please refer to the documentation link above.
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Title Data", meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Storage")
	static UEIK_GetTitleData_AsyncFunction* GetTitleData( FString FileName);

	virtual void Activate() override;

	void OnGetFileProgress(const FString& FileName1, uint64 BytesRead);
	void GetTitleData();

	void OnGetFileComplete(bool bSuccess,  const FString& V_FileName);
};
