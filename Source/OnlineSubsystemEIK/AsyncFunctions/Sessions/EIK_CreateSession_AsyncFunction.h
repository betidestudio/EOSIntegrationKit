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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSession_Delegate, const FString&, SessionID);

USTRUCT(BlueprintType)
struct FDedicatedServerSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bIsDedicatedServer = false;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	int32 PortInfo;
	
};

USTRUCT(BlueprintType)
struct FCreateSessionExtraSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	int32 NumberOfPrivateConnections = 0;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bShouldAdvertise = true;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bAllowJoinInProgress = true;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	ERegionInfo Region = ERegionInfo::RE_NoSelection;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bUsePresence = false;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bAllowJoinViaPresence = false;
	
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bAllowJoinViaPresenceFriendsOnly = false;
};

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CreateSession_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	int32 NumberOfPublicConnections;
	TMap<FString, FEIKAttribute> SessionSettings;
	FDedicatedServerSettings DedicatedServerSettings;
	FCreateSessionExtraSettings ExtraSettings;

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
	static UEIK_CreateSession_AsyncFunction* CreateEIKSession(
        TMap<FString, FEIKAttribute> SessionSettings,
		int32 NumberOfPublicConnections = 15,
		FDedicatedServerSettings DedicatedServerSettings = FDedicatedServerSettings(), 
		FCreateSessionExtraSettings ExtraSettings = FCreateSessionExtraSettings() 
	);};
