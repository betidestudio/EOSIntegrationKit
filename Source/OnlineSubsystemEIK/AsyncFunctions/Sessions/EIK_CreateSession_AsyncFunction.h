//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_CreateSession_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSession_Delegate, FName, SessionName);

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CreateSession_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	FString SessionName;
	bool bIsDedicatedServer;
	bool bIsLan;
	int32 NumberOfPublicConnections;
	int32 NumberOfPrivateConnections;
	bool bShouldAdvertise;
	bool bAllowJoinInProgress;
	ERegionInfo Region;
	TMap<FString, FString> SessionSettings;

	bool bDelegateCalled = false;
	UPROPERTY(BlueprintAssignable)
	FCreateSession_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FCreateSession_Delegate OnFail;

	virtual void Activate() override;

	void CreateSession();

	void OnCreateSessionCompleted(FName VSessionName, bool bWasSuccessful);

	/*
	This C++ method creates a session in EOS using the selected method and sets up a callback function to handle the response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/sessions/
	For Input Parameters, please refer to the documentation link above.
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Create EIK Session", meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Sessions")
	static UEIK_CreateSession_AsyncFunction* CreateEIKSession(FString SessionName, bool bIsDedicatedServer, bool bIsLan, int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections, bool bShouldAdvertise, bool bAllowJoinInProgress, ERegionInfo Region, TMap<FString, FString> SessionSettings);
};
