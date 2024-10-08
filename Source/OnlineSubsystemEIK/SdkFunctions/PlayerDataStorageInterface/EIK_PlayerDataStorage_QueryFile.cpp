// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PlayerDataStorage_QueryFile.h"

UEIK_PlayerDataStorage_QueryFile* UEIK_PlayerDataStorage_QueryFile::EIK_PlayerDataStorage_QueryFile(
	FEIK_ProductUserId LocalUserId, FString Filename)
{
	UEIK_PlayerDataStorage_QueryFile* Action = NewObject<UEIK_PlayerDataStorage_QueryFile>();
	Action->Var_LocalUserId = LocalUserId;
	Action->Var_Filename = Filename;
	return Action;
}

void UEIK_PlayerDataStorage_QueryFile::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PlayerDataStorage_QueryFileOptions Options = {};
			Options.ApiVersion = EOS_PLAYERDATASTORAGE_QUERYFILE_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.Filename = TCHAR_TO_ANSI(*Var_Filename);
			EOS_PlayerDataStorage_QueryFile(EOSRef->PlayerDataStorageHandle, &Options, this, &UEIK_PlayerDataStorage_QueryFile::EOS_PlayerDataStorage_OnQueryFileComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PlayerDataStorage_QueryFile: OnlineSubsystemEOS is not valid"));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_PlayerDataStorage_QueryFile::EOS_PlayerDataStorage_OnQueryFileComplete(const EOS_PlayerDataStorage_QueryFileCallbackInfo* Data)
{
	if (UEIK_PlayerDataStorage_QueryFile* Node = static_cast<UEIK_PlayerDataStorage_QueryFile*>(Data->ClientData))
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
