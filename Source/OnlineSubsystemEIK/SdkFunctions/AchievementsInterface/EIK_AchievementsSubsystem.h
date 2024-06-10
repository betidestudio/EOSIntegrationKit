// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_AchievementsSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAchievementsUnlockedV2Callback, FEIK_ProductUserId, UserId, FString, AchievementId, int64, UnlockTime);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_AchievementsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FOnAchievementsUnlockedV2Callback OnAchievementsUnlockedV2;

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit | SDK Functions | Achievements Interface", DisplayName="EOS_Achievements_AddNotifyAchievementsUnlockedV2")
	FEIK_NotificationId EIK_Achievements_AddNotifyAchievementsUnlockedV2();

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit | SDK Functions | Achievements Interface", DisplayName="EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId")
	TEnumAsByte<EEIK_Result> EIK_Achievements_CopyAchievementDefinitionV2ByAchievementId(FString AchievementId, FEIK_Achievements_DefinitionV2& OutAchievementDefinition);

};
