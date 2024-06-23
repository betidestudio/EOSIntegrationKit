// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "EIK_PlayerDataStorage_QueryFile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQueryFileComplete, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_ProductUserId&, LocalUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorage_QueryFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query a specific file's metadata, such as file names, size, and a MD5 hash of the data. This is not required before a file may be opened, saved, copied, or deleted. Once a file has been queried, its metadata will be available by the EOS_PlayerDataStorage_CopyFileMetadataAtIndex and EOS_PlayerDataStorage_CopyFileMetadataByFilename functions.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_QueryFile")
	static UEIK_PlayerDataStorage_QueryFile* EIK_PlayerDataStorage_QueryFile(FEIK_ProductUserId LocalUserId, FString Filename);

	UPROPERTY(BlueprintAssignable)
	FOnQueryFileComplete OnCallback;
	
private:
	virtual void Activate() override;
	static void EOS_CALL EOS_PlayerDataStorage_OnQueryFileComplete(const EOS_PlayerDataStorage_QueryFileCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	FString Var_Filename;
};
