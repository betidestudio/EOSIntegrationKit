// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_SanctionsAsyncFunction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSanctionsDelegate, const TArray<FEIK_Sanctions_PlayerSanction>&, Sanctions);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SanctionsAsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/*
	This C++ method gets the logged in user's achievements from the EOS backend.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Player Sanctions",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Others")
	static UEIK_SanctionsAsyncFunction* GetEikPlayerSanctionsAsyncFunction(FString LocalProductUserID, FString TargetProductUserID);

	FString Var_LocalProductUserID;
	FString Var_TargetProductUserID;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Sanctions")
	FSanctionsDelegate Success;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Sanctions")
	FSanctionsDelegate Failure;
	
	void Func_GetSanctions();

	static void EOS_CALL ReturnFunc(const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo* Data);

	void GetFinalValues();
	void FireFailure();
	void Activate() override;
};
