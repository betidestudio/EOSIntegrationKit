// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "EIK_PlayerDataStorage_ReadFile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_PlayerDataStorage_ReadFileDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_ProductUserId&, LocalUserId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEIK_PlayerDataStorage_OnReadFileDataCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FEIK_PlayerDataStorage_OnFileTransferProgressCallback, const FEIK_ProductUserId&, LocalUserId, const FString&, Filename, int32, BytesTransferred, int32, TotalFileSizeBytes);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorage_ReadFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Retrieve the contents of a specific file, potentially downloading the contents if we do not have a local copy, from the cloud. This request will occur asynchronously, potentially over multiple frames. All callbacks for this function will come from the same thread that the SDK is ticked from. If specified, the FileTransferProgressCallback will always be called at least once if the request is started successfully.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_ReadFile")
	static UEIK_PlayerDataStorage_ReadFile* EIK_PlayerDataStorage_ReadFile(FEIK_ProductUserId LocalUserId, FString Filename, int32 ReadChunkLengthBytes);

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_ReadFileDelegate OnCallback;
	
	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_OnReadFileDataCallback OnReadFileDataCallback;

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_OnFileTransferProgressCallback OnFileTransferProgressCallback;	
	
private:
	virtual void Activate() override;
	static void EOS_CALL EOS_PlayerDataStorage_OnReadFileComplete(const EOS_PlayerDataStorage_ReadFileCallbackInfo* Data);
	static EOS_PlayerDataStorage_EReadResult EOS_CALL EOS_PlayerDataStorage_OnReadFileData(const EOS_PlayerDataStorage_ReadFileDataCallbackInfo* Data);
	static void EOS_CALL EOS_PlayerDataStorage_OnFileTransferProgress(const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	FString Var_Filename;
	int32 Var_ReadChunkLengthBytes;
	
};
