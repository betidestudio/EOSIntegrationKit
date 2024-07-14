// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "EIK_PlayerDataStorage_DeleteFile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_PlayerDataStorage_DeleteFileDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_ProductUserId&, LocalUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorage_DeleteFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Deletes an existing file in the cloud. If successful, the file's data will be removed from our local cache.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_DeleteFile")
	static UEIK_PlayerDataStorage_DeleteFile* EIK_PlayerDataStorage_DeleteFile(FEIK_ProductUserId LocalUserId, FString Filename);

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_DeleteFileDelegate OnCallback;
	
private:
	virtual void Activate() override;
	static void EOS_CALL EOS_PlayerDataStorage_OnDeleteFileComplete(const EOS_PlayerDataStorage_DeleteFileCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	FString Var_Filename;
};
