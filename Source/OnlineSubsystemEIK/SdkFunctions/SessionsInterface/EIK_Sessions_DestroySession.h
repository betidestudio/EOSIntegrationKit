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
#include "EIK_Sessions_DestroySession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnDestroySessionCallback, const TEnumAsByte<EEIK_Result>&, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_DestroySession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	
	//Destroy a session given a session name
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_DestroySession")
	static UEIK_Sessions_DestroySession* EIK_Sessions_DestroySession(FString SessionName);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_OnDestroySessionCallback OnCallback;
private:
	FString Var_SessionName;
	virtual void Activate() override;
	static void EOS_CALL OnDestroySessionCallback(const EOS_Sessions_DestroySessionCallbackInfo* Data);
};
