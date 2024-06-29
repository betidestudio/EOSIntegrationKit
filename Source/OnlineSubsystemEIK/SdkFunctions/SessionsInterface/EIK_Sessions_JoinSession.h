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
#include "EIK_Sessions_JoinSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnJoinSessionCallback, const TEnumAsByte<EEIK_Result>&, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_JoinSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	
	//Join a session, creating a local session under a given session name. Backend will validate various conditions to make sure it is possible to join the session.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_JoinSession")
	static UEIK_Sessions_JoinSession* EIK_Sessions_JoinSession(FString SessionName, FEIK_ProductUserId LocalUserId, FEIK_HSessionDetails SessionHandle, bool bPresenceEnabled);
	
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnJoinSessionCallback OnCallback;
private:
	FString Var_SessionName;
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_HSessionDetails Var_SessionHandle;
	bool Var_bPresenceEnabled;
	virtual void Activate() override;
	static void EOS_CALL OnJoinSessionCallback(const EOS_Sessions_JoinSessionCallbackInfo* Data);
};
