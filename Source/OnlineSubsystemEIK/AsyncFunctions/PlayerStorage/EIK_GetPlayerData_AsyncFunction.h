// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 5
#include "Online/CoreOnline.h"
#else
#include "UObject/CoreOnline.h"
#endif
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_GetPlayerData_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetDataResult, bool, bWasSuccess, const TArray<uint8>&,SavedData);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetPlayerData_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FGetDataResult OnSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FGetDataResult OnFail;

	bool bDelegateCalled = false;

	FString FileName;
	
	/*
	This C++ method gets the player data in the online subsystem using the selected method and sets up a callback function to handle the response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/playerdata/
	For Input Parameters, please refer to the documentation link above.
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Player Storage", meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Storage")
	static UEIK_GetPlayerData_AsyncFunction* GetPlayerData( FString FileName);

	virtual void Activate() override;

	void GetPlayerData();

	void OnGetFileComplete(bool bSuccess, const FUniqueNetId& UserID, const FString& V_FileName);

};
