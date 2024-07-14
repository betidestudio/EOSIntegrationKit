// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_auth_types.h"
#include <eos_base.h>
#include <eos_auth.h>
THIRD_PARTY_INCLUDES_END
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_Auth_Login.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Auth_LoginCallbackInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit")
	TEnumAsByte<EEIK_Result> Result = EEIK_Result::EOS_ServiceFailure;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit")
	FEIK_EpicAccountId LocalUserId;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit")
	FEIK_Auth_PinGrantInfo PinGrantInfo;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit")
	FEIK_ContinuanceToken ContinuanceToken;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit")
	FEIK_EpicAccountId SelectedAccountId;

	FEIK_Auth_LoginCallbackInfo() = default;

	FEIK_Auth_LoginCallbackInfo(EOS_Auth_LoginCallbackInfo Data)
	{
		Result = static_cast<EEIK_Result>(Data.ResultCode);
		LocalUserId = Data.LocalUserId;
		PinGrantInfo = *Data.PinGrantInfo;
		ContinuanceToken = Data.ContinuanceToken;
		SelectedAccountId = Data.SelectedAccountId;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Auth_LoginOptions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	FEIK_Auth_Credentials Credentials;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit", meta=(Bitmask, BitmaskEnum = "/Script/OnlineSubsystemEIK.EEIK_EAuthScopeFlags"))
	int32 ScopeFlags;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit")
	int64 LoginFlags;

	FEIK_Auth_LoginOptions()
	{
		ScopeFlags = EEIK_EAuthScopeFlags::EIK_AS_NoFlags;
		LoginFlags = 0;
	}
	EOS_Auth_LoginOptions ToEOSAuthLoginOptions()
	{
		EOS_Auth_LoginOptions Options;
		Options.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
		EOS_Auth_Credentials CredentialsRef = Credentials.GetValueAsEosType();
		Options.Credentials = &CredentialsRef;
		Options.ScopeFlags = static_cast<EOS_EAuthScopeFlags>(ScopeFlags);
		Options.LoginFlags = LoginFlags;
		return Options;
	}
	
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_Auth_LoginCallback, const FEIK_Auth_LoginCallbackInfo&, Data);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_Login : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Login/Authenticate with user credentials.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_Login")
	static UEIK_Auth_Login* Login(FEIK_Auth_LoginOptions Options);
	
	UPROPERTY(BlueprintAssignable)
	FEIK_Auth_LoginCallback OnCallback;
	
private:
	FEIK_Auth_LoginOptions Var_Options;
	virtual void Activate() override;
	static void EOS_CALL Internal_OnLoginComplete(const EOS_Auth_LoginCallbackInfo* Data);
};
