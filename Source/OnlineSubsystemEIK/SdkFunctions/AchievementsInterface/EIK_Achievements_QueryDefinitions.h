// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Achievements_QueryDefinitions.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEIK_Achievements_QueryDefinitionsComplete, TEnumAsByte<EEIK_Result>, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Achievements_QueryDefinitions : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query for a list of definitions for all existing achievements, including localized text, icon IDs and whether an achievement is hidden.
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit | SDK Functions | Achievements Interface", DisplayName="EOS_Achievements_QueryDefinitions")
	static UEIK_Achievements_QueryDefinitions* EIK_Achievements_QueryDefinitions(FEIK_ProductUserId UserId);

	UPROPERTY(BlueprintAssignable)
	FOnEIK_Achievements_QueryDefinitionsComplete OnCallback;
	
private:

	virtual void Activate() override;
	FEIK_ProductUserId Var_UserId;
	
};
