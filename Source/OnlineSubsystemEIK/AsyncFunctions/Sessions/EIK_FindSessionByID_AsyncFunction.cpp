// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_FindSessionByID_AsyncFunction.h"
#if ENGINE_MAJOR_VERSION == 5
#include "Online/OnlineSessionNames.h"
#endif

UEIK_FindSessionByID_AsyncFunction* UEIK_FindSessionByID_AsyncFunction::FindEIKSessionByID(FString SessionID)
{
	UEIK_FindSessionByID_AsyncFunction* Ueik_FindSessionObject= NewObject<UEIK_FindSessionByID_AsyncFunction>();
	Ueik_FindSessionObject->Var_SessionID = SessionID;
	return Ueik_FindSessionObject;
}

void UEIK_FindSessionByID_AsyncFunction::Activate()
{
	FindSession();
	Super::Activate();
}



void UEIK_FindSessionByID_AsyncFunction::FindSession()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			const FUniqueNetIdPtr SearchingUserId = IOnlineSubsystem::Get()->GetIdentityInterface()->GetUniquePlayerId(0);
			const FUniqueNetIdRef UserNetId = FUniqueNetIdString::Create(SearchingUserId.Get()->ToString(),"EIK");
			const FUniqueNetIdRef SessionID = FUniqueNetIdString::Create(Var_SessionID,"EIK");
#if ENGINE_MAJOR_VERSION == 5
			const FUniqueNetIdRef SearchingUserIdRef = FUniqueNetIdString::EmptyId();
#else
			const FUniqueNetIdRef SearchingUserIdRef = FUniqueNetIdString::Create(SearchingUserId.Get()->ToString(),"EIK");
#endif
			const FOnSingleSessionResultCompleteDelegate OnSingleSessionResultCompleteDelegate = FOnSingleSessionResultCompleteDelegate::CreateUObject(this, &UEIK_FindSessionByID_AsyncFunction::OnFindSessionCompleted);
			SessionPtrRef->FindSessionById(*UserNetId,*SessionID,*UserNetId,OnSingleSessionResultCompleteDelegate);
		}
		else
		{
			if(bDelegateCalled)
			{
				return;
			}
			OnFail.Broadcast(FSessionFindStruct());
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
	else
	{
		if(bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast(FSessionFindStruct());
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}


void UEIK_FindSessionByID_AsyncFunction::OnFindSessionCompleted(int I, bool bWasSuccessful,
	const FOnlineSessionSearchResult& OnlineSessionSearchResult)
{
	if (bWasSuccessful)
	{
		if (const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
		{
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid())
			{
				FBlueprintSessionResult SessionResult;
				SessionResult.OnlineResult = OnlineSessionSearchResult;
				FOnlineSessionSettings LocalSessionSettings = SessionResult.OnlineResult.Session.SessionSettings;

				TMap<FName, FOnlineSessionSetting>::TIterator It(LocalSessionSettings.Settings);

				TMap<FString, FEIKAttribute> LocalSettings;
				while (It)
				{
					const FName& SettingName = It.Key();
					const FOnlineSessionSetting& Setting = It.Value();
					LocalSettings.Add(*SettingName.ToString(), Setting.Data);
					++It;
				}

				bool IsServer = LocalSessionSettings.bIsDedicated;
				FSessionFindStruct LocalStruct;
				LocalStruct.SessionName = "GameSession";
				LocalStruct.CurrentNumberOfPlayers = (SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections + SessionResult.OnlineResult.Session.SessionSettings.NumPrivateConnections) - (SessionResult.OnlineResult.Session.NumOpenPublicConnections + SessionResult.OnlineResult.Session.NumOpenPrivateConnections);
				LocalStruct.MaxNumberOfPlayers = SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections + SessionResult.OnlineResult.Session.SessionSettings.NumPrivateConnections;
				LocalStruct.SessionResult = SessionResult;
				LocalStruct.SessionSettings = LocalSettings;
				LocalStruct.bIsDedicatedServer = IsServer;


				OnSuccess.Broadcast(LocalStruct);
				bDelegateCalled = true;
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif
			}
		}
		else
		{
			if (bDelegateCalled)
			{
				return;
			}
			OnFail.Broadcast(FSessionFindStruct());
			bDelegateCalled = true;
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
		
	}
	else
	{
		if (bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast(FSessionFindStruct());
		bDelegateCalled = true;
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}	
}
