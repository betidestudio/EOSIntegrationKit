// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "EIK_PlayerDataStorage_WriteFile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_PlayerDataStorage_WriteFileDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_ProductUserId&, LocalUserId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FEIK_PlayerDataStorage_OnFileWriteTransferProgressCallback, const FEIK_ProductUserId&, LocalUserId, const FString&, Filename, int32, BytesTransferred, int32, TotalFileSizeBytes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEIK_PlayerDataStorage_OnWriteFileDataCallback);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorage_WriteFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Write new data to a specific file, potentially overwriting any existing file by the same name, to the cloud. This request will occur asynchronously, potentially over multiple frames. All callbacks for this function will come from the same thread that the SDK is ticked from. If specified, the FileTransferProgressCallback will always be called at least once if the request is started successfully.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_WriteFile")
	static UEIK_PlayerDataStorage_WriteFile* EIK_PlayerDataStorage_WriteFile(FEIK_ProductUserId LocalUserId, FString Filename, const TArray<uint8>& Data, int32 DataLengthBytes, int32 ChunkLengthBytes);

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_WriteFileDelegate OnCallback;

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_OnFileWriteTransferProgressCallback OnFileTransferProgressCallback;

	UPROPERTY(BlueprintAssignable)
	FEIK_PlayerDataStorage_OnWriteFileDataCallback OnWriteFileDataCallback;
	
private:
	static void EOS_CALL EOS_PlayerDataStorage_OnFileTransferProgress(const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo* Data);
	static EOS_PlayerDataStorage_EWriteResult EOS_PlayerDataStorage_OnWriteFileData(const EOS_PlayerDataStorage_WriteFileDataCallbackInfo* Data, void* OutDataBuffer, uint32_t* OutDataWritten);
	virtual void Activate() override;
	static void EOS_CALL EOS_PlayerDataStorage_OnWriteFileComplete(const EOS_PlayerDataStorage_WriteFileCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	FString Var_Filename;
	TArray<uint8> Var_Data;
	int32 Var_DataLengthBytes;
	int32 BytesWritten = 0;
	int32 Var_ChunkLengthBytes = 1024;
};
