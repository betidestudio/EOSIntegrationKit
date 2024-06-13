// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "eos_achievements.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Achievements_UnlockAchievements.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEIK_Achievements_UnlockAchievementsComplete, TEnumAsByte<EEIK_Result>, ResultCode, FEIK_ProductUserId, UserId, int32, AchievementCount);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Achievements_UnlockAchievements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit | SDK Functions | Achievements Interface", DisplayName="EOS_Achievements_UnlockAchievements")
	static UEIK_Achievements_UnlockAchievements* EIK_Achievements_UnlockAchievements(FEIK_ProductUserId UserId, TArray<FString> AchievementIds);

	UPROPERTY(BlueprintAssignable)
	FOnEIK_Achievements_UnlockAchievementsComplete OnCallback;

private:
	virtual void Activate() override;
	FEIK_ProductUserId Var_UserId;
	TArray<FString> Var_AchievementIds;
};
