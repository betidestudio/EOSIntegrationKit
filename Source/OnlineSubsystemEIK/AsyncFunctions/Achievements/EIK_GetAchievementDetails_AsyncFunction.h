// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 5
#include "Online/CoreOnline.h"
#else
#include "UObject/CoreOnline.h"
#endif
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_GetAchievement_AsyncFunction.h"
#include "EIK_GetAchievementDetails_AsyncFunction.generated.h"

USTRUCT(BlueprintType)
struct FEIK_AchievementDescription
{
	GENERATED_BODY()

public:

	/** The id of the achievement */
	UPROPERTY(BlueprintReadOnly, Category="EIK Struct")
	FString Id = "";

	/** The progress towards completing this achievement: 0.0-100.0 */
	UPROPERTY(BlueprintReadOnly, Category="EIK Struct")
	float Progress = 0.0;
	
	/** The localized title of the achievement */
	UPROPERTY(BlueprintReadOnly, Category="EIK Struct")
	FText Title = FText::FromString("");

	/** The localized locked description of the achievement */
	UPROPERTY(BlueprintReadOnly, Category="EIK Struct")
	FText LockedDesc = FText::FromString("");

	/** The localized unlocked description of the achievement */
	UPROPERTY(BlueprintReadOnly, Category="EIK Struct")
	FText UnlockedDesc = FText::FromString("");

	/** Flag for whether the achievement is hidden */
	UPROPERTY(BlueprintReadOnly, Category="EIK Struct")
	bool bIsHidden = false;

	/** The date/time the achievement was unlocked */
	UPROPERTY(BlueprintReadOnly, Category="EIK Struct")
	FDateTime UnlockTime;


};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAchievementDetails_Delegate, const FEIK_AchievementDescription&, AchievementDescription);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetAchievementDetails_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	FEIK_Achievement Var_Achievement;

	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FAchievementDetails_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FAchievementDetails_Delegate OnFail;
	/*
	This C++ method gets the achievement's details from the EOS backend.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Achievement Description",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Achievements")
	static UEIK_GetAchievementDetails_AsyncFunction* GetEIKAchievementDescription(FEIK_Achievement Achievement);

	void Activate() override;

	void GetAchievementDescription();

	bool bDelegateCalled = false;

	void OnAchievementDescriptionCompleted(const FUniqueNetId& UniqueNetId, bool bWasSuccess);

};
