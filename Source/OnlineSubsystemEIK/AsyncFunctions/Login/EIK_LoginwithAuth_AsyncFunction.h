// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION == 5
#include "Online/CoreOnline.h"
#else
#include "UObject/CoreOnline.h"
#endif
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_LoginwithAuth_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_LoginwithAuthDelegate, FEIK_EpicAccountId, EpicUserID, FEIK_ProductUserId, ProductUserID, FString, Error);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_LoginwithAuth_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/*
		This function is used to login using EIK using the Auth Interface easily without having to worry about the underlying implementation.
		@param CredentialType: The login method to use.
		@param DisplayName: The display name of the user.
		@param Token: The token to use for login.

		Please note that the Auth Interface means that an EPIC ACCOUNT is required to login. This means you can use features that require an Epic Account like Friends, Overlay, etc.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit | Login", DisplayName = "Login using Auth Interface")
	static UEIK_LoginwithAuth_AsyncFunction* LoginWithAuth(TEnumAsByte<EEIK_ELoginCredentialType> CredentialType = EEIK_ELoginCredentialType::EIK_LCT_AccountPortal, TEnumAsByte<EEIK_EExternalCredentialType> ExternalCredentialType = EEIK_EExternalCredentialType::EIK_ECT_EPIC, FString Id = "", FString Token = "");

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Login")
	FEIK_LoginwithAuthDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Login")
	FEIK_LoginwithAuthDelegate OnFailure;
private:
	EEIK_ELoginCredentialType Var_CredentialType;
	FString Var_Id;
	FString Var_Token;
	EEIK_EExternalCredentialType Var_ExternalCredentialType;
	virtual void Activate() override;
	void LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);

};
