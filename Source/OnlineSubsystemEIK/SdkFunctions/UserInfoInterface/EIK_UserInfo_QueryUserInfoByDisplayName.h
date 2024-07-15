// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "eos_userinfo.h"
#include "EIK_UserInfo_QueryUserInfoByDisplayName.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FEIK_OnQueryUserInfoByDisplayNameCallback, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId, const FString&, DisplayName);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_UserInfo_QueryUserInfoByDisplayName : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//EOS_UserInfo_QueryUserInfoByDisplayName is used to start an asynchronous query to retrieve user information by display name. This can be useful for getting the EOS_EpicAccountId for a display name. Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | User Info Interface", DisplayName="EOS_UserInfo_QueryUserInfoByDisplayName")
	static UEIK_UserInfo_QueryUserInfoByDisplayName* EIK_UserInfo_QueryUserInfoByDisplayName(const FEIK_EpicAccountId& LocalUserId, const FString& DisplayName);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_OnQueryUserInfoByDisplayNameCallback OnCallback;
private:
	FEIK_EpicAccountId Var_LocalUserId;
	FString Var_DisplayName;
	virtual void Activate() override;
	static void EOS_CALL OnQueryUserInfoByDisplayNameCallback(const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data);
};
