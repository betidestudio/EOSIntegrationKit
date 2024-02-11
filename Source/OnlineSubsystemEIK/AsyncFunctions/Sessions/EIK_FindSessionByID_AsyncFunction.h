// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_FindSessionByID_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindSessionByID_Delegate, const FSessionFindStruct&, FoundResult);

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_FindSessionByID_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FFindSessionByID_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FFindSessionByID_Delegate OnFail;
	/*
	This function is used to find a session by ID and return the result. The SessionID can be obtained from the CreateEIKSession function or CreateEIKLobby function.
	@param SessionID - The ID of the session to find.
	 
	 Documentation link: https://eik.betide.studio/multiplayer/sessions/lobbies/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Find EIK Session By ID",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Sessions")
	static UEIK_FindSessionByID_AsyncFunction* FindEIKSessionByID(FString SessionID);


	virtual void Activate() override;

	void OnFindSessionCompleted(int I, bool bArg, const FOnlineSessionSearchResult& OnlineSessionSearchResult);
	void FindSession();
	
	FString Var_SessionID;

	bool bDelegateCalled = false;

};
