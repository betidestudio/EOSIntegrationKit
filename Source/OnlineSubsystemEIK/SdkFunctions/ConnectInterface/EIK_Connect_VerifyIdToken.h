// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Connect_VerifyIdToken.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Connect_VerifyIdTokenCallbackInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	TEnumAsByte<EEIK_Result> Result;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FEIK_ProductUserId LocalUserId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	bool bIsAccountInfoPresent;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	TEnumAsByte<EEIK_EExternalAccountType> AccountType;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString AccountId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString Platform;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString DeviceType;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString ClientId;
	
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString ProductId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString SandboxId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString DeploymentId;

	FEIK_Connect_VerifyIdTokenCallbackInfo()
	{
		Result = EEIK_Result::EOS_Success;
		LocalUserId = FEIK_ProductUserId();
		bIsAccountInfoPresent = false;
		AccountType = EEIK_EExternalAccountType::EIK_EAT_EPIC;
		AccountId = "";
		Platform = "";
		DeviceType = "";
		ClientId = "";
		ProductId = "";
		SandboxId = "";
		DeploymentId = "";
	}
	FEIK_Connect_VerifyIdTokenCallbackInfo(const EOS_Connect_VerifyIdTokenCallbackInfo* Data)
	{
		Result = static_cast<EEIK_Result>(Data->ResultCode);
		LocalUserId = Data->ProductUserId;
		if(Data->bIsAccountInfoPresent == EOS_TRUE)
		{
			bIsAccountInfoPresent = true;
		}
		else
		{
			bIsAccountInfoPresent = false;
		}
		AccountType = static_cast<EEIK_EExternalAccountType>(Data->AccountIdType);
		AccountId = Data->AccountId;
		Platform = Data->Platform;
		DeviceType = Data->DeviceType;
		ClientId = Data->ClientId;
		ProductId = Data->ProductId;
		SandboxId = Data->SandboxId;
		DeploymentId = Data->DeploymentId;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_Connect_VerifyIdToken_Delegate, const FEIK_Connect_VerifyIdTokenCallbackInfo&, Data);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Connect_VerifyIdToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Verify a given ID token for authenticity and validity.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_VerifyIdToken")
	static UEIK_Connect_VerifyIdToken* EIK_Connect_VerifyIdToken(FEIK_Connect_IdToken IdToken);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_Connect_VerifyIdToken_Delegate OnCallback;
private:
	static void OnVerifyIdTokenCallback(const EOS_Connect_VerifyIdTokenCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_Connect_IdToken Var_IdToken;
};
