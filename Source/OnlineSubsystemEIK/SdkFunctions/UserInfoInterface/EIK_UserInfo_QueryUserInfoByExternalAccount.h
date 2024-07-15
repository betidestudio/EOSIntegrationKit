// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_UserInfo_QueryUserInfoByExternalAccount.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FEIK_OnQueryUserInfoByExternalAccountCallback, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId, const TEnumAsByte<EEIK_EExternalAccountType>&, ExternalAccountType, const FString&, ExternalAccountId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_UserInfo_QueryUserInfoByExternalAccount : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//EOS_UserInfo_QueryUserInfoByExternalAccount is used to start an asynchronous query to retrieve user information by external accounts. This can be useful for getting the EOS_EpicAccountId for external accounts. Once the callback has been fired with a successful ResultCode, it is possible to call CopyUserInfo to receive an EOS_UserInfo containing the available information.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | User Info Interface", DisplayName="EOS_UserInfo_QueryUserInfoByExternalAccount")
	static UEIK_UserInfo_QueryUserInfoByExternalAccount* EIK_UserInfo_QueryUserInfoByExternalAccount(const FEIK_EpicAccountId& LocalUserId, const TEnumAsByte<EEIK_EExternalAccountType>& AccountType, const FString& ExternalAccountId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_OnQueryUserInfoByExternalAccountCallback OnCallback;

private:
	FEIK_EpicAccountId Var_LocalUserId;
	TEnumAsByte<EEIK_EExternalAccountType> Var_AccountType;
	FString Var_ExternalAccountId;
	virtual void Activate() override;
	static void EOS_CALL OnQueryUserInfoByExternalAccountCallback(const EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo* Data);
};
