// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Connect_QueryExternalAccountMappings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Connect_QueryExternalAccountMappings_Delegate, const FEIK_ProductUserId&, ProductUserId, TEnumAsByte<EEIK_Result>, Result);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Connect_QueryExternalAccountMappings : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Retrieve the equivalent Product User IDs from a list of external account IDs from supported account providers. The values will be cached and retrievable through EOS_Connect_GetExternalAccountMapping. Queries using external account IDs of another account system may not be available, depending on the account system specifics.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_QueryExternalAccountMappings")
	static UEIK_Connect_QueryExternalAccountMappings* EIK_Connect_QueryExternalAccountMappings(FEIK_ProductUserId ProductUserId, TEnumAsByte<EEIK_EExternalAccountType> AccountType, const TArray<FString>& ExternalAccountIds);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_Connect_QueryExternalAccountMappings_Delegate OnCallback;
private:
	static void OnQueryExternalAccountMappingsCallback(const EOS_Connect_QueryExternalAccountMappingsCallbackInfo* Data);
	virtual void Activate() override;
	
	FEIK_ProductUserId Var_ProductUserId;
	TEnumAsByte<EEIK_EExternalAccountType> Var_AccountType;
	TArray<FString> Var_ExternalAccountIds;
	

	
};
