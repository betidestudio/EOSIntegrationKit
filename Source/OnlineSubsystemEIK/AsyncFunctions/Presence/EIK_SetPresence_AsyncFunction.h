// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "IOnlineSubsystemEOS.h"
#include "OnlineSubsystemUtils.h"
#include "Runtime/Launch/Resources/Version.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSetPresence_Delegate, const FString&, RichPresenceStr, const EPresenceStatus&, PresenceStatusEnum);


UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SetPresence_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable, DisplayName = "Success")
	FSetPresence_Delegate OnSuccess;

	UPROPERTY(BlueprintAssignable, DisplayName = "Faliure")
	FSetPresence_Delegate OnFaliure;

	FString RichPresence;
	
	EPresenceStatus PresenceStatus;

	virtual void Activate() override;

	void SetPresence();

	void OnSetPresenceCompleted(const class FUniqueNetId& UserId, const bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, DisplayName = "Set EIK Presence", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || Presence")
	static UEIK_SetPresence_AsyncFunction* SetEOSPresence(FString RichPresense, EPresenceStatus PresenceStatus);
};
