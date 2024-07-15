// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Connect_CreateUser.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateUserCallback, EEIK_Result, ResultCode, FEIK_ProductUserId, LocalUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Connect_CreateUser : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Create an account association with the Epic Online Service as a product user given their external auth credentials.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_CreateUser")
	static UEIK_Connect_CreateUser* CreateUser(FEIK_ContinuanceToken ContinuanceToken);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FOnCreateUserCallback OnCallback;

private:
	virtual void Activate() override;
	FEIK_ContinuanceToken Var_ContinuanceToken;
	static void EOS_CALL OnCreateUserCallback(const EOS_Connect_CreateUserCallbackInfo* Data);
};
