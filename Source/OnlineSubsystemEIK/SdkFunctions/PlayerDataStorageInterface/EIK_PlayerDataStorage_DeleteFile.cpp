// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PlayerDataStorage_DeleteFile.h"



UEIK_PlayerDataStorage_DeleteFile* UEIK_PlayerDataStorage_DeleteFile::EIK_PlayerDataStorage_DeleteFile(
	FEIK_ProductUserId LocalUserId, FString Filename)
{
	UEIK_PlayerDataStorage_DeleteFile* Action = NewObject<UEIK_PlayerDataStorage_DeleteFile>();
	Action->Var_LocalUserId = LocalUserId;
	Action->Var_Filename = Filename;
	return Action;
}

void UEIK_PlayerDataStorage_DeleteFile::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_DeleteFileOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_DELETEFILE_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.Filename = TCHAR_TO_ANSI(*Var_Filename);
			EOS_PlayerDataStorage_DeleteFile(EOSRef->PlayerDataStorageHandle, &Options, this, &UEIK_PlayerDataStorage_DeleteFile::EOS_PlayerDataStorage_OnDeleteFileComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_DeleteFile: OnlineSubsystemEOS is not valid"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_PlayerDataStorage_DeleteFile::EOS_PlayerDataStorage_OnDeleteFileComplete(
	const EOS_PlayerDataStorage_DeleteFileCallbackInfo* Data)
{
	if (UEIK_PlayerDataStorage_DeleteFile* Node = static_cast<UEIK_PlayerDataStorage_DeleteFile*>(Data->ClientData))
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
