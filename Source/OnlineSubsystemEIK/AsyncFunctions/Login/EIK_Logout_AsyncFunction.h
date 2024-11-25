// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_Logout_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLogout_Delegate);

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Logout_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FLogout_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FLogout_Delegate OnFail;
	/*
	This C++ method logs out in a user to an online subsystem using the selected method and sets up a callback function to handle the login response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Logout Using EIK",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Login")
	static UEIK_Logout_AsyncFunction* LogoutUsingEIK();

	void Activate() override;

	void LogoutPlayer();

	void LogoutCallback(int32 LocalUserNum, bool bWasSuccess);

};
