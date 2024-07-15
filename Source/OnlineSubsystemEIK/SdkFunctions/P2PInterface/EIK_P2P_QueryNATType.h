// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_P2P_QueryNATType.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_P2P_QueryNATTypeDelegate, const TEnumAsByte<EEIK_Result>&, Result, const TEnumAsByte<EEIK_ENATType>&, NATType);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_P2P_QueryNATType : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query the current NAT-type of our connection.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | P2P Interface", DisplayName="EOS_P2P_QueryNATType")
	static UEIK_P2P_QueryNATType* EIK_P2P_QueryNATType();

	UPROPERTY(BlueprintAssignable)
	FEIK_P2P_QueryNATTypeDelegate OnCallback;
	
private:
	virtual void Activate() override;
	static void EOS_CALL EOS_P2P_QueryNATType_Callback(const EOS_P2P_OnQueryNATTypeCompleteInfo* Data);
	
};
