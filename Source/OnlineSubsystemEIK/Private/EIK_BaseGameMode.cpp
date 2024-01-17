// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_BaseGameMode.h"
#include "GameFramework/GameSession.h"
#include "Net/OnlineEngineInterface.h"

void AEIK_BaseGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                                 FString& ErrorMessage)
{
	ErrorMessage = GameSession->ApproveLogin(Options);
	FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
}
