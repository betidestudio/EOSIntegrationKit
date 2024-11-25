// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_UpdateSession_AsyncFunction.generated.h"


USTRUCT(BlueprintType)
struct FEIK_MemberSpecificAttribute
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	FEIKUniqueNetId MemberId;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit")
	TMap<FString, FEIKAttribute> Attributes;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateSessionDelegate);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_UpdateSession_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Update EIK Session",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
		Category="EOS Integration Kit || Sessions")
	static UEIK_UpdateSession_AsyncFunction* UpdateEIKSessions(UObject* WorldContextObject,
	                                                           TMap<FString, FEIKAttribute> SessionSettings,
	                                                           TArray<FEIK_MemberSpecificAttribute> MemberSettings,
	                                                           FName SessionName = "GameSession",
	                                                           bool bShouldAdvertise = true,
	                                                           bool bAllowJoinInProgress = true,
	                                                           bool bAllowInvites = true,
	                                                           bool bUsesPresence = false,
	                                                           bool bIsLANMatch = false,
	                                                           bool bIsDedicatedServer = false,
	                                                           bool bIsLobbySession = false,
	                                                           int32 NumberOfPublicConnections = 10,
	                                                           int32 NumberOfPrivateConnections = 10);

	
	UPROPERTY()
	UObject* Var_WorldContextObject;
	TMap<FString, FEIKAttribute> Var_SessionSettings;
	TArray<FEIK_MemberSpecificAttribute> Var_MemberSettings;
	bool Var_bShouldAdvertise;
	bool Var_bAllowJoinInProgress;
	bool Var_bAllowInvites;
	bool Var_bIsLANMatch;
	bool Var_bIsDedicatedServer;
	bool Var_bUsesPresence;
	int32 Var_NumberOfPublicConnections;
	int32 Var_NumberOfPrivateConnections;
	FName Var_SessionName;
	bool Var_bIsLanMatch;
	bool Var_bIsLobbySession;

	void OnUpdateSessionComplete(FName Name, bool bArg);
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FUpdateSessionDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FUpdateSessionDelegate OnFailure;
};
