// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_EcomSubsystem.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="Ecom Interface"), Category="EOS Integration Kit", DisplayName="Ecom Interface")
class ONLINESUBSYSTEMEIK_API UEIK_EcomSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//Release the memory associated with an EOS_Ecom_CatalogItem structure. This must be called on data retrieved from EOS_Ecom_CopyOfferItemByIndex.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CatalogItem_Release")
	void EIK_Ecom_CatalogItem_Release(FEIK_Ecom_CatalogItem CatalogItem);
};
