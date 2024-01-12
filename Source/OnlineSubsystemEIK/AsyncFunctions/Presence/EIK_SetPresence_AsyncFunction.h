// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "IOnlineSubsystemEOS.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "EIK_SetPresence_AsyncFunction.generated.h"

/**
 * 
 */
 UENUM(BlueprintType)
		 enum class EPresenceStatus : uint8 {
		 PR_Online       UMETA(DisplayName = "Online"),
		 PR_Offline              UMETA(DisplayName = "Offline"),
		 PR_Away        UMETA(DisplayName = "Away"),
		 PR_ExtendedAway        UMETA(DisplayName = "ExtendedAway"),
		 PR_DoNotDisturb              UMETA(DisplayName = "DoNotDisturb"),
	 };

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SetPresence_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	FString RichPresence;
	
	EPresenceStatus PresenceStatus;

	virtual void Activate() override;

	void SetPresence();

	UFUNCTION(BlueprintCallable, DisplayName = "Set EIK Presence", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || Presence")
	static UEIK_SetPresence_AsyncFunction* SetEOSPresence(FString RichPresense, EPresenceStatus PresenceStatus);

};
