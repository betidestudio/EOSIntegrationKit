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
#include "EIK_Sessions_UpdateSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_OnUpdateSessionCallback, const TEnumAsByte<EEIK_Result>&, ResultCode, const FString&, SessionName, const FString&, SessionId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_UpdateSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Update a session given a session modification handle created by EOS_Sessions_CreateSessionModification or EOS_Sessions_UpdateSessionModification
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_UpdateSession")
	static UEIK_Sessions_UpdateSession* EIK_Sessions_UpdateSession(FEIK_HSessionModification SessionModificationHandle);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnUpdateSessionCallback OnCallback;

private:
	FEIK_HSessionModification Var_SessionModificationHandle;
	virtual void Activate() override;
	static void EOS_CALL OnUpdateSessionCallback(const EOS_Sessions_UpdateSessionCallbackInfo* Data);
};
