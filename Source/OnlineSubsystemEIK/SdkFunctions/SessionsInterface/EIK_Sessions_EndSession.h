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
#include "EIK_Sessions_EndSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnEndSessionCallback, const TEnumAsByte<EEIK_Result>&, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_EndSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Mark a session as ended, making it unavailable to find if "join in progress" was disabled. The session may be started again if desired
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_EndSession")
	static UEIK_Sessions_EndSession* EIK_Sessions_EndSession(FString SessionName);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnEndSessionCallback OnCallback;

private:
	FString Var_SessionName;
	virtual void Activate() override;
	static void EOS_CALL OnEndSessionCallback(const EOS_Sessions_EndSessionCallbackInfo* Data);
	
};
