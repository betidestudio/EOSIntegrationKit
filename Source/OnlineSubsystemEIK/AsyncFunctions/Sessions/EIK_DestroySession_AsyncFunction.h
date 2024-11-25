// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_DestroySession_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroySession_Delegate);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_DestroySession_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	bool bDelegateCalled = false;
	FName Var_SessionName;
	
	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FDestroySession_Delegate OnSuccess;
	
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FDestroySession_Delegate OnFail;

	/*
	 *This C++ method destroys a session using the EOS SDK and sets up a callback function to handle the destroy session response.
	 *Documentation link: https://eik.betide.studio/multiplayer/sessions/matchmaking-sessions/
	 *For Input Parameters, please refer to the documentation link above.
	 */
	UFUNCTION(BlueprintCallable, DisplayName="Destroy EIK Session",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Sessions")
	static UEIK_DestroySession_AsyncFunction* DestroyEIKSessions(FName SessionName);


	virtual void Activate() override;

	void DestroySession();

	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccess);
};
