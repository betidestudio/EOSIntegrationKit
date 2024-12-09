// Copyright Epic Games, Inc. All Rights Reserved.

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

USTRUCT(BlueprintType)
struct FCreateLobbySettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bIsLanMatch = false;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bAllowInvites = true;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	int32 NumberOfPrivateConnections = 0;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bShouldAdvertise = true;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bAllowJoinInProgress = true;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	ERegionInfo Region = ERegionInfo::RE_NoSelection;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bUseVoiceChat = false;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bUsePresence = false;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	FString BucketID = "";

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bSupportHostMigration = false;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	bool bEnableJoinViaID = false;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	FString LobbyIDOverride = "";
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateLobby_Delegate, const FString&, LobbyID);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CreateLobby_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	int32 NumberOfPublicConnections;
	TMap<FString, FEIKAttribute> SessionSettings;
	TMap<FString, FEIKAttribute> MemberSettings;
	FCreateLobbySettings Var_CreateLobbySettings;
	bool bDelegateCalled = false;
	FName VSessionName;
	
	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FCreateLobby_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FCreateLobby_Delegate OnFail;

	virtual void Activate() override;

	void CreateLobby();

	void OnCreateLobbyCompleted(FName SessionName, bool bWasSuccessful);

	/*
	This function is used to create a lobby with the given settings and returns a result delegate which can be used to determine if the lobby was created successfully or not.
	@param SessionSettings - A map of session settings to be used when creating the lobby.
	@param NumberOfPublicConnections - The number of public connections to be used when creating the lobby.
	@param ExtraSettings - A struct containing extra settings to be used when creating the lobby which is completely optional.
	Documentation link: https://eik.betide.studio/multiplayer/sessions/lobbies/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Create EIK Lobby",meta = (BlueprintInternalUseOnly = "true",AutoCreateRefTerm="SessionSettings,MemberSettings" ), Category="EOS Integration Kit || Sessions")
	static UEIK_CreateLobby_AsyncFunction* CreateEIKLobby(
		TMap<FString, FEIKAttribute> SessionSettings,
		TMap<FString, FEIKAttribute> MemberSettings,
		FName SessionName,
		int32 NumberOfPublicConnections,
		FCreateLobbySettings ExtraSettings);
};
