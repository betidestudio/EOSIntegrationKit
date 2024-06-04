// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_connect_types.h"
#include "eos_connect.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_LinkAccount_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLinkAccountCallback, TEnumAsByte<EEIK_Result>, Result);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_LinkAccount_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | Connect | Link Account")
	static UEIK_LinkAccount_AsyncFunction* LinkAccount(const FString& LocalProductUserId, const FEIK_ContinuanceToken& ContinuanceToken);
	
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FOnLinkAccountCallback OnCallback;

private:
	FString Var_LocalProductUserId;
	FEIK_ContinuanceToken Var_ContinuanceToken;
	static void EOS_CALL OnLinkAccountCallback(const EOS_Connect_LinkAccountCallbackInfo* Data);
	virtual void Activate() override;
};
