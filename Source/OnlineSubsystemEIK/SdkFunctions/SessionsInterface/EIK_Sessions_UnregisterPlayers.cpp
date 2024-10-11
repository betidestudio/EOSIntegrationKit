// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sessions_UnregisterPlayers.h"

#include "Async/Async.h"

UEIK_Sessions_UnregisterPlayers* UEIK_Sessions_UnregisterPlayers::EIK_Sessions_UnregisterPlayers(FString SessionName,
                                                                                                 TArray<FEIK_ProductUserId> PlayersToUnregister)
{
	UEIK_Sessions_UnregisterPlayers* Node = NewObject<UEIK_Sessions_UnregisterPlayers>();
	Node->Var_SessionName = SessionName;
	Node->Var_PlayersToUnregister = PlayersToUnregister;
	return Node;
}

void UEIK_Sessions_UnregisterPlayers::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_UnregisterPlayersOptions UnregisterPlayersOptions = { };
			UnregisterPlayersOptions.ApiVersion = EOS_SESSIONS_UNREGISTERPLAYERS_API_LATEST;
			UnregisterPlayersOptions.SessionName = TCHAR_TO_ANSI(*Var_SessionName);
			UnregisterPlayersOptions.PlayersToUnregisterCount = Var_PlayersToUnregister.Num();
			UnregisterPlayersOptions.PlayersToUnregister = new EOS_ProductUserId[Var_PlayersToUnregister.Num()];
			for (int i = 0; i < Var_PlayersToUnregister.Num(); i++)
			{
				UnregisterPlayersOptions.PlayersToUnregister[i] = Var_PlayersToUnregister[i].GetValueAsEosType();
			}
			EOS_Sessions_UnregisterPlayers(EOSRef->SessionsHandle, &UnregisterPlayersOptions, this, &UEIK_Sessions_UnregisterPlayers::OnUnregisterPlayersCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to unregister players either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, TArray<FEIK_ProductUserId>());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
	

void UEIK_Sessions_UnregisterPlayers::OnUnregisterPlayersCallback(
	const EOS_Sessions_UnregisterPlayersCallbackInfo* Data)
{
	if (UEIK_Sessions_UnregisterPlayers* Node = static_cast<UEIK_Sessions_UnregisterPlayers*>(Data->ClientData))
	{
		TArray<FEIK_ProductUserId> UnregisteredPlayers;
		for (int i = 0; i < (int32)Data->UnregisteredPlayersCount; i++)
		{
			UnregisteredPlayers.Add(FEIK_ProductUserId(Data->UnregisteredPlayers[i]));
		}
		AsyncTask(ENamedThreads::GameThread, [Node, Data, UnregisteredPlayers]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), UnregisteredPlayers);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
