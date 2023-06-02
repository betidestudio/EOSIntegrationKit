//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_CreateLobby_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateLobby_Delegate, FName, SessionName);


/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CreateLobby_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	FString SessionName;
	bool bAllowInvites;
	bool bIsLan;
	int32 NumberOfPublicConnections;
	int32 NumberOfPrivateConnections;
	bool bShouldAdvertise;
	bool bAllowJoinInProgress;
	ERegionInfo Region;
	TMap<FString, FString> SessionSettings;
	 bool bUseVoiceChat;
	 bool bUsePresence;

	bool bDelegateCalled = false;
	
	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FCreateLobby_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FCreateLobby_Delegate OnFail;

	virtual void Activate() override;

	void CreateLobby();

	void OnCreateLobbyCompleted(FName VSessionName, bool bWasSuccessful);

	/*
	This C++ method creates a lobby in EOS using the selected method and sets up a callback function to handle the response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/sessions/
	For Input Parameters, please refer to the documentation link above.
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Create EIK Lobby",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Sessions")
	static UEIK_CreateLobby_AsyncFunction* CreateEIKLobby(FString SessionName, bool bAllowInvites, bool bIsLan, int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections, bool bShouldAdvertise, bool bAllowJoinInProgress, bool bUseVoiceChat, bool bUsePresence, ERegionInfo Region, TMap<FString, FString> SessionSettings);
};
