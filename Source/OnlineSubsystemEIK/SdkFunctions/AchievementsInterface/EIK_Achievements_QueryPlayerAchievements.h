// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "eos_achievements.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Achievements_QueryPlayerAchievements.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEIK_Achievements_QueryPlayerAchievementsComplete, TEnumAsByte<EEIK_Result>, ResultCode, FEIK_ProductUserId, LocalUserId, FEIK_ProductUserId, TargetUserId);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Achievements_QueryPlayerAchievements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query for a list of achievements for a specific player, including progress towards completion for each achievement.
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit | SDK Functions | Achievements Interface", DisplayName="EOS_Achievements_QueryPlayerAchievements")
	static UEIK_Achievements_QueryPlayerAchievements* EIK_Achievements_QueryPlayerAchievements(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId TargetUserId);

	UPROPERTY(BlueprintAssignable)
	FOnEIK_Achievements_QueryPlayerAchievementsComplete OnCallback;
private:
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_ProductUserId Var_TargetUserId;
	virtual void Activate() override;
};
