// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PlayerDataStorage_DuplicateFile.h"

UEIK_PlayerDataStorage_DuplicateFile* UEIK_PlayerDataStorage_DuplicateFile::EIK_PlayerDataStorage_DuplicateFile(
	FEIK_ProductUserId LocalUserId, FString SourceFilename, FString DestinationFilename)
{
	UEIK_PlayerDataStorage_DuplicateFile* Node = NewObject<UEIK_PlayerDataStorage_DuplicateFile>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_SourceFilename = SourceFilename;
	Node->Var_DestinationFilename = DestinationFilename;
	return Node;
}

void UEIK_PlayerDataStorage_DuplicateFile::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_DuplicateFileOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_DUPLICATEFILE_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.SourceFilename = TCHAR_TO_ANSI(*Var_SourceFilename);
			Options.DestinationFilename = TCHAR_TO_ANSI(*Var_DestinationFilename);
			EOS_PlayerDataStorage_DuplicateFile(EOSRef->PlayerDataStorageHandle, &Options, this, &UEIK_PlayerDataStorage_DuplicateFile::EOS_PlayerDataStorage_OnDuplicateFileComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_DuplicateFile: OnlineSubsystemEOS is not valid"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_PlayerDataStorage_DuplicateFile::EOS_PlayerDataStorage_OnDuplicateFileComplete(const EOS_PlayerDataStorage_DuplicateFileCallbackInfo* Data)
{
	if (UEIK_PlayerDataStorage_DuplicateFile* Node = static_cast<UEIK_PlayerDataStorage_DuplicateFile*>(Data->ClientData))
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
