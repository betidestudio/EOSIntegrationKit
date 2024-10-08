// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PlayerDataStorage_ReadFile.h"




void UEIK_PlayerDataStorage_ReadFile::EOS_PlayerDataStorage_OnReadFileComplete(
	const EOS_PlayerDataStorage_ReadFileCallbackInfo* Data)
{
	if (UEIK_PlayerDataStorage_ReadFile* Node = static_cast<UEIK_PlayerDataStorage_ReadFile*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId);
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}

EOS_PlayerDataStorage_EReadResult UEIK_PlayerDataStorage_ReadFile::EOS_PlayerDataStorage_OnReadFileData(
	const EOS_PlayerDataStorage_ReadFileDataCallbackInfo* Data)
{
	EOS_PlayerDataStorage_EReadResult Result = EOS_PlayerDataStorage_EReadResult::EOS_RR_ContinueReading;
	return Result;
}


void UEIK_PlayerDataStorage_ReadFile::EOS_PlayerDataStorage_OnFileTransferProgress(
	const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo* Data)
{
	if (UEIK_PlayerDataStorage_ReadFile* Node = static_cast<UEIK_PlayerDataStorage_ReadFile*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnFileTransferProgressCallback.Broadcast(Data->LocalUserId, Data->Filename, Data->BytesTransferred, Data->TotalFileSizeBytes);
		});
	}
}

UEIK_PlayerDataStorage_ReadFile* UEIK_PlayerDataStorage_ReadFile::EIK_PlayerDataStorage_ReadFile(
	FEIK_ProductUserId LocalUserId, FString Filename, int32 ReadChunkLengthBytes)
{
	UEIK_PlayerDataStorage_ReadFile* Node = NewObject<UEIK_PlayerDataStorage_ReadFile>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_Filename = Filename;
	Node->Var_ReadChunkLengthBytes = ReadChunkLengthBytes;
	return Node;
}

void UEIK_PlayerDataStorage_ReadFile::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_ReadFileOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_READFILE_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.Filename = TCHAR_TO_ANSI(*Var_Filename);
			Options.ReadChunkLengthBytes = Var_ReadChunkLengthBytes;
			Options.ReadFileDataCallback = &UEIK_PlayerDataStorage_ReadFile::EOS_PlayerDataStorage_OnReadFileData;
			Options.FileTransferProgressCallback = &UEIK_PlayerDataStorage_ReadFile::EOS_PlayerDataStorage_OnFileTransferProgress;
			EOS_PlayerDataStorage_ReadFile(EOSRef->PlayerDataStorageHandle, &Options, this, &UEIK_PlayerDataStorage_ReadFile::EOS_PlayerDataStorage_OnReadFileComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_ReadFile: OnlineSubsystemEOS is not valid"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
