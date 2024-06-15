// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
THIRD_PARTY_INCLUDES_START
#include <eos_auth.h>
THIRD_PARTY_INCLUDES_END
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Auth_VerifyIdToken.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Auth_VerifyIdTokenCallbackInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	TEnumAsByte<EEIK_Result> ResultCode;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString ApplicationId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString ClientId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString ProductId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString SandboxId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString DeploymentId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	bool bIsExternalAccountInfoPresent;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	TEnumAsByte<EEIK_EExternalAccountType> ExternalAccountIdType;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString ExternalAccountId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString ExternalAccountDisplayName;
	
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FString Platform;

	FEIK_Auth_VerifyIdTokenCallbackInfo()
	{
		ResultCode = EEIK_Result::EOS_NotFound;
		ApplicationId = "";
		ClientId = "";
		ProductId = "";
		SandboxId = "";
		DeploymentId = "";
		DisplayName = "";
		bIsExternalAccountInfoPresent = false;
		ExternalAccountIdType = EEIK_EExternalAccountType::EIK_EAT_EPIC;
		ExternalAccountId = "";
		ExternalAccountDisplayName = "";
		Platform = "";
	}
	FEIK_Auth_VerifyIdTokenCallbackInfo(const EOS_Auth_VerifyIdTokenCallbackInfo& Data)
	{
		ResultCode = static_cast<EEIK_Result>(Data.ResultCode);
		ApplicationId = FString(UTF8_TO_TCHAR(Data.ApplicationId));
		ClientId = FString(UTF8_TO_TCHAR(Data.ClientId));
		ProductId = FString(UTF8_TO_TCHAR(Data.ProductId));
		SandboxId = FString(UTF8_TO_TCHAR(Data.SandboxId));
		DeploymentId = FString(UTF8_TO_TCHAR(Data.DeploymentId));
		DisplayName = FString(UTF8_TO_TCHAR(Data.DisplayName));
		if(Data.bIsExternalAccountInfoPresent == EOS_TRUE)
		{
			bIsExternalAccountInfoPresent = true;
		}
		else
		{
			bIsExternalAccountInfoPresent = false;
		}
		ExternalAccountIdType = static_cast<EEIK_EExternalAccountType>(Data.ExternalAccountIdType);
		ExternalAccountId = FString(UTF8_TO_TCHAR(Data.ExternalAccountId));
		ExternalAccountDisplayName = FString(UTF8_TO_TCHAR(Data.ExternalAccountDisplayName));
		Platform = FString(UTF8_TO_TCHAR(Data.Platform));
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_Auth_OnVerifyIdTokenCallback, const FEIK_Auth_VerifyIdTokenCallbackInfo&, Data);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_VerifyIdToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Verify a given ID token for authenticity and validity.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_VerifyIdToken")
	static UEIK_Auth_VerifyIdToken* VerifyIdToken(FEIK_Auth_IdToken IdToken);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_Auth_OnVerifyIdTokenCallback OnCallback;
private:
	FEIK_Auth_IdToken Var_IdToken;
	static void EOS_CALL OnVerifyIdTokenCallback(const EOS_Auth_VerifyIdTokenCallbackInfo* Data);
	virtual void Activate() override;

};
