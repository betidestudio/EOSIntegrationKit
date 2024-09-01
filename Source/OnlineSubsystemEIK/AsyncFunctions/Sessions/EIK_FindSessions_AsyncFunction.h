// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_FindSessions_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindSession_Delegate, const TArray<FSessionFindStruct>&, SessionResults);

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_FindSessions_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FFindSession_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FFindSession_Delegate OnFail;
	/*
	This C++ method searches for sessions in an online subsystem using the selected method and sets up a callback function to handle the search response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/sessions/
	For Input Parameters, please refer to the documentation link above.
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Find EIK Sessions",meta = (BlueprintInternalUseOnly = "true", AutoCreateRefTerm=SessionSettings), Category="EOS Integration Kit || Sessions")
	static UEIK_FindSessions_AsyncFunction* FindEIKSessions(TMap<FString, FEIKAttribute> SessionSettings, EMatchType MatchType = EMatchType::MT_Lobby, int32 MaxResults = 15, ERegionInfo RegionToSearch = ERegionInfo::RE_NoSelection, bool bLanSearch = false, bool bIncludePartySessions = false);


	virtual void Activate() override;

	void FindSession();

	void OnFindSessionCompleted(bool bWasSuccess);

	// This is a C++ variable for storing a reference to an online session search.
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	TMap<FString, FEIKAttribute> SessionSettings;
	ERegionInfo E_RegionToSearch;
	EMatchType E_MatchType;
	int32 I_MaxResults;
	bool B_bLanSearch;
	bool bIncludePartySessions = false;

	bool bDelegateCalled = false;

};
