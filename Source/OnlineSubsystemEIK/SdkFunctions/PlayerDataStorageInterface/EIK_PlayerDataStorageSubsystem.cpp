// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PlayerDataStorageSubsystem.h"

#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

TEnumAsByte<EEIK_Result> UEIK_PlayerDataStorageSubsystem::EIK_PlayerDataStorage_CopyFileMetadataAtIndex(FEIK_ProductUserId LocalUserId, int32 Index, FEIK_PlayerDataStorage_FileMetadata& OutMetadata)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_CopyFileMetadataAtIndexOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_COPYFILEMETADATAATINDEX_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.Index = Index;
			EOS_PlayerDataStorage_FileMetadata* Metadata = nullptr;
			auto Result = EOS_PlayerDataStorage_CopyFileMetadataAtIndex(EOSRef->PlayerDataStorageHandle, &Options, &Metadata);
			if (Metadata)
			{
				OutMetadata = *Metadata;
			}
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_CopyFileMetadataAtIndex: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PlayerDataStorageSubsystem::EIK_PlayerDataStorage_CopyFileMetadataByFilename(
	FEIK_ProductUserId LocalUserId, FString Filename, FEIK_PlayerDataStorage_FileMetadata& OutMetadata)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_CopyFileMetadataByFilenameOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_COPYFILEMETADATABYFILENAME_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.Filename = TCHAR_TO_ANSI(*Filename);
			EOS_PlayerDataStorage_FileMetadata* Metadata = nullptr;
			auto Result = EOS_PlayerDataStorage_CopyFileMetadataByFilename(EOSRef->PlayerDataStorageHandle, &Options, &Metadata);
			if (Metadata)
			{
				OutMetadata = *Metadata;
			}
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_CopyFileMetadataByFilename: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_PlayerDataStorageSubsystem::EIK_PlayerDataStorage_FileMetadata_Release(FEIK_PlayerDataStorage_FileMetadata& Metadata)
{
	EOS_PlayerDataStorage_FileMetadata_Release(&Metadata.Ref);
}

TEnumAsByte<EEIK_Result> UEIK_PlayerDataStorageSubsystem::EIK_PlayerDataStorage_GetFileMetadataCount(
	FEIK_ProductUserId LocalUserId, int32& OutFileMetadataCount)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_GetFileMetadataCountOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_GETFILEMETADATACOUNT_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			int32_t Count = 0;
			auto Result = EOS_PlayerDataStorage_GetFileMetadataCount(EOSRef->PlayerDataStorageHandle, &Options, &Count);
			OutFileMetadataCount = Count;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_GetFileMetadataCount: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PlayerDataStorageSubsystem::EIK_PlayerDataStorageFileTransferRequest_GetFilename(
	FEIK_HPlayerDataStorageFileTransferRequest TransferRequestHandle, FString& OutFilename)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			char Filename[256];
			int32_t* OutStringLength = nullptr;
			auto Result = EOS_PlayerDataStorageFileTransferRequest_GetFilename(TransferRequestHandle.Ref, 256, Filename, OutStringLength);
			OutFilename = FString(ANSI_TO_TCHAR(Filename));
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorageFileTransferRequest_GetFilename: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PlayerDataStorageSubsystem::EIK_PlayerDataStorageFileTransferRequest_GetFileRequestState(
	FEIK_HPlayerDataStorageFileTransferRequest TransferRequestHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			auto Result = EOS_PlayerDataStorageFileTransferRequest_GetFileRequestState(TransferRequestHandle.Ref);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorageFileTransferRequest_GetFileRequestState: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_PlayerDataStorageSubsystem::EIK_PlayerDataStorageFileTransferRequest_Release(
	FEIK_HPlayerDataStorageFileTransferRequest TransferRequestHandle)
{
	EOS_PlayerDataStorageFileTransferRequest_Release(TransferRequestHandle.Ref);
}
