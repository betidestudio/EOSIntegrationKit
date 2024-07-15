// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_sessions.h"
#include "eos_sessions_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_Sessions_StartSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnStartSessionCallback, const TEnumAsByte<EEIK_Result>&, ResultCode);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_StartSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Mark a session as started, making it unable to find if session properties indicate "join in progress" is not available
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_StartSession")
	static UEIK_Sessions_StartSession* EIK_Sessions_StartSession(FString SessionName);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnStartSessionCallback OnCallback;

private:
	FString Var_SessionName;
	virtual void Activate() override;
	static void EOS_CALL OnStartSessionCallback(const EOS_Sessions_StartSessionCallbackInfo* Data);
	
	
};
