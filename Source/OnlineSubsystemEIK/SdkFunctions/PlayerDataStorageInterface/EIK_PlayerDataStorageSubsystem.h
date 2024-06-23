// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_PlayerDataStorageSubsystem.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Player Data Storage Interface", meta=(DisplayName="Player Data Storage Interface"))
class ONLINESUBSYSTEMEIK_API UEIK_PlayerDataStorageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//Get the cached copy of a file's metadata by index. The metadata will be for the last retrieved or successfully saved version, and will not include any local changes that have not been committed by calling SaveFile. The returned pointer must be released by the user when no longer needed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_CopyFileMetadataAtIndex")
	TEnumAsByte<EEIK_Result>  EIK_PlayerDataStorage_CopyFileMetadataAtIndex(FEIK_ProductUserId LocalUserId, int32 Index, FEIK_PlayerDataStorage_FileMetadata& OutMetadata);

	//Create the cached copy of a file's metadata by filename. The metadata will be for the last retrieved or successfully saved version, and will not include any changes that have not completed writing. The returned pointer must be released by the user when no longer needed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_CopyFileMetadataByFilename")
	TEnumAsByte<EEIK_Result>  EIK_PlayerDataStorage_CopyFileMetadataByFilename(FEIK_ProductUserId LocalUserId, FString Filename, FEIK_PlayerDataStorage_FileMetadata& OutMetadata);

	//Free the memory used by the file metadata
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_FileMetadata_Release")
	void EIK_PlayerDataStorage_FileMetadata_Release(FEIK_PlayerDataStorage_FileMetadata& Metadata);

	//Get the count of files we have previously queried information for and files we have previously read from / written to.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorage_GetFileMetadataCount")
	TEnumAsByte<EEIK_Result>  EIK_PlayerDataStorage_GetFileMetadataCount(FEIK_ProductUserId LocalUserId, int32& OutFileMetadataCount);

	//Get the file name of the file this request is for. OutStringLength will always be set to the string length of the file name if it is not NULL.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorageFileTransferRequest_GetFilename")
	TEnumAsByte<EEIK_Result>  EIK_PlayerDataStorageFileTransferRequest_GetFilename(FEIK_HPlayerDataStorageFileTransferRequest TransferRequestHandle, FString& OutFilename);

	//Get the current state of a file request.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorageFileTransferRequest_GetFileRequestState")
	TEnumAsByte<EEIK_Result>  EIK_PlayerDataStorageFileTransferRequest_GetFileRequestState(FEIK_HPlayerDataStorageFileTransferRequest TransferRequestHandle);

	//Free the memory used by a cloud-storage file request handle. This will not cancel a request in progress.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Player Data Storage Interface", DisplayName="EOS_PlayerDataStorageFileTransferRequest_Release")
	void EIK_PlayerDataStorageFileTransferRequest_Release(FEIK_HPlayerDataStorageFileTransferRequest TransferRequestHandle);
};
