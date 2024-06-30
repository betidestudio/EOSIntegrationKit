// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "eos_userinfo.h"
#include "EIK_UserInfo_QueryUserInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_OnQueryUserInfoCallback, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_UserInfo_QueryUserInfo : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//EOS_UserInfo_QueryUserInfo is used to start an asynchronous query to retrieve information, such as display name, about another account. Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | User Info Interface", DisplayName="EOS_UserInfo_QueryUserInfo")
	static UEIK_UserInfo_QueryUserInfo* EIK_UserInfo_QueryUserInfo(const FEIK_EpicAccountId& LocalUserId, const FEIK_EpicAccountId& TargetUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_OnQueryUserInfoCallback OnCallback;

private:
	FEIK_EpicAccountId Var_LocalUserId;
	FEIK_EpicAccountId Var_TargetUserId;
	virtual void Activate() override;
	static void EOS_CALL OnQueryUserInfoCallback(const EOS_UserInfo_QueryUserInfoCallbackInfo* Data);
};
