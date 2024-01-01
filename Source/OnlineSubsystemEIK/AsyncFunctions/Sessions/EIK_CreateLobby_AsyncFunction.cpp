//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_CreateLobby_AsyncFunction.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "Online/OnlineSessionNames.h"

void UEIK_CreateLobby_AsyncFunction::Activate()
{
	CreateLobby();
	Super::Activate();
}

void UEIK_CreateLobby_AsyncFunction::CreateLobby()
{
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = false;
			SessionCreationInfo.bAllowInvites = Var_CreateLobbySettings.bAllowInvites;
			SessionCreationInfo.bIsLANMatch = false;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.NumPrivateConnections = Var_CreateLobbySettings.NumberOfPrivateConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = true;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = Var_CreateLobbySettings.bUseVoiceChat;
			SessionCreationInfo.bUsesPresence = Var_CreateLobbySettings.bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresence = Var_CreateLobbySettings.bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = Var_CreateLobbySettings.bUsePresence;
			SessionCreationInfo.bShouldAdvertise = Var_CreateLobbySettings.bShouldAdvertise;
			SessionCreationInfo.bAllowJoinInProgress = Var_CreateLobbySettings.bAllowJoinInProgress;
			SessionCreationInfo.SessionIdOverride = Var_CreateLobbySettings.LobbyIDOverride;
			SessionCreationInfo.Set(SETTING_HOST_MIGRATION, Var_CreateLobbySettings.bSupportHostMigration, EOnlineDataAdvertisementType::ViaOnlineService);
			//SessionCreationInfo.Set(SEARCH_KEYWORDS, NAME_GameSession, EOnlineDataAdvertisementType::ViaOnlineService);
			for (auto& Settings_SingleValue : SessionSettings)
			{
				if (Settings_SingleValue.Key.Len() == 0)
				{
					continue;
				}
				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Data = Settings_SingleValue.Value.GetVariantData();
				SessionCreationInfo.Set(FName(*Settings_SingleValue.Key), Setting);
			}
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEIK_CreateLobby_AsyncFunction::OnCreateLobbyCompleted);
			SessionPtrRef->CreateSession(0,NAME_GameSession,SessionCreationInfo);
		}
		else
		{
			if(bDelegateCalled == false)
			{
				UE_LOG(LogOnline, Warning, TEXT("EIK: SessionPtrRef is null"));
				OnFail.Broadcast("");
				SetReadyToDestroy();
				bDelegateCalled = true;
			}
		}
	}
	else
	{
		if(bDelegateCalled == false)
		{
			UE_LOG(LogOnline, Warning, TEXT("EIK: SubsystemRef is null"));
			OnFail.Broadcast("");
			SetReadyToDestroy();
			bDelegateCalled = true;
		}
	}
}

void UEIK_CreateLobby_AsyncFunction::OnCreateLobbyCompleted(FName VSessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if(bDelegateCalled == false)
		{
			const IOnlineSessionPtr Sessions = IOnlineSubsystem::Get()->GetSessionInterface();
			if(const FOnlineSession* CurrentSession = Sessions->GetNamedSession(NAME_GameSession))
			{
				OnSuccess.Broadcast(CurrentSession->SessionInfo.Get()->GetSessionId().ToString());
				bDelegateCalled = true;
				SetReadyToDestroy();
			}
			else
			{
				OnSuccess.Broadcast("");
				bDelegateCalled = true;
				SetReadyToDestroy();
			}
			OnSuccess.Broadcast(VSessionName.ToString());
			bDelegateCalled = true;
			SetReadyToDestroy();
		}
	}
	else
	{
		if(bDelegateCalled == false)
		{
			UE_LOG(LogOnline, Warning, TEXT("EIK: CreateLobby failed and response was false. Check logs for more information."));
			OnFail.Broadcast("");
			SetReadyToDestroy();
			bDelegateCalled = true;
		}
	}
}

UEIK_CreateLobby_AsyncFunction* UEIK_CreateLobby_AsyncFunction::CreateEIKLobby(TMap<FString, FEIKAttribute> SessionSettings,
	int32 NumberOfPublicConnections, FCreateLobbySettings ExtraSettings)
{
	UEIK_CreateLobby_AsyncFunction* Ueik_CreateLobbyObject= NewObject<UEIK_CreateLobby_AsyncFunction>();
	Ueik_CreateLobbyObject->NumberOfPublicConnections = NumberOfPublicConnections;
	Ueik_CreateLobbyObject->SessionSettings = SessionSettings;
	Ueik_CreateLobbyObject->Var_CreateLobbySettings = ExtraSettings;
	return Ueik_CreateLobbyObject;
}
