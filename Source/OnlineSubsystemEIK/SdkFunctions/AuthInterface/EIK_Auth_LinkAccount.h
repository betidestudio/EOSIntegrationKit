// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_auth_types.h"
#include "eos_auth.h"
THIRD_PARTY_INCLUDES_END
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_Auth_LinkAccount.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Auth_LinkAccountCallbackInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	TEnumAsByte<EEIK_Result> ResultCode;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FEIK_EpicAccountId LocalUserId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FEIK_Auth_PinGrantInfo PinGrantInfo;
	
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FEIK_EpicAccountId SelectedAccountId;

	FEIK_Auth_LinkAccountCallbackInfo()
	{
		ResultCode = EEIK_Result::EOS_Success;
	}

	FEIK_Auth_LinkAccountCallbackInfo(const EOS_Auth_LinkAccountCallbackInfo* data)
	{
		ResultCode = static_cast<EEIK_Result>(data->ResultCode);
		LocalUserId = data->LocalUserId;
		PinGrantInfo = *data->PinGrantInfo;
		SelectedAccountId = data->SelectedAccountId;
	}
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_Auth_OnLinkAccountCallback, FEIK_Auth_LinkAccountCallbackInfo, Data);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_LinkAccount : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/*
	 *Link external account by continuing previous login attempt with a continuance token. The user will be presented with Epic Accounts onboarding flow managed by the SDK. On success, the user will be logged in at the completion of this action. This will commit this external account to the Epic Account and cannot be undone in the SDK.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_LinkAccount")
	static UEIK_Auth_LinkAccount* LinkAccount(FEIK_ContinuanceToken ContinuanceToken, FEIK_EpicAccountId LocalUserId, TEnumAsByte<EEIK_ELinkAccountFlags> LinkAccountFlags);
	
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_Auth_OnLinkAccountCallback OnCallback;
	
private:
	FEIK_ContinuanceToken Var_ContinuanceToken;
	FEIK_EpicAccountId Var_LocalUserId;
	TEnumAsByte<EEIK_ELinkAccountFlags> Var_LinkAccountFlags;
	virtual void Activate() override;
	static void EOS_CALL OnLinkAccountCallback(const EOS_Auth_LinkAccountCallbackInfo* Data);
};
