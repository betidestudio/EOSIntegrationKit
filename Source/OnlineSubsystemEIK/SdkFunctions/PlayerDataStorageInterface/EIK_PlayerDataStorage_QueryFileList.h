// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "EIK_PlayerDataStorage_QueryFileList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_PlayerDataStorage_QueryFileListDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_ProductUserId&, LocalUserId, int32, FileCount);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorage_QueryFileList : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query the file metadata, such as file names, size, and a MD5 hash of the data, for all files owned by this user for this application. This is not required before a file may be opened, saved, copied, or deleted.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_QueryFileList")
	static UEIK_PlayerDataStorage_QueryFileList* EIK_PlayerDataStorage_QueryFileList(FEIK_ProductUserId LocalUserId);

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_QueryFileListDelegate OnCallback;

private:
	virtual void Activate() override;
	static void EOS_CALL EOS_PlayerDataStorage_OnQueryFileListComplete(const EOS_PlayerDataStorage_QueryFileListCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
};
