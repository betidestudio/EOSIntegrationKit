//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_CreateLobby_AsyncFunction.h"

#include "Online/OnlineSessionNames.h"

void UEIK_CreateLobby_AsyncFunction::Activate()
{
	CreateLobby();
	Super::Activate();
}

void UEIK_CreateLobby_AsyncFunction::CreateLobby()
{
	if(IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = false;
			SessionCreationInfo.bAllowInvites = bAllowInvites;
			SessionCreationInfo.bIsLANMatch = bIsLan;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.NumPrivateConnections = NumberOfPrivateConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = true;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = bUseVoiceChat;
			SessionCreationInfo.bUsesPresence =bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresence = bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = bUsePresence;
			SessionCreationInfo.bShouldAdvertise = bShouldAdvertise;
			SessionCreationInfo.bAllowJoinInProgress = bAllowJoinInProgress;
			
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FString(SessionName), EOnlineDataAdvertisementType::ViaOnlineService);
			for (auto& Settings_SingleValue : SessionSettings)
			{
				if (Settings_SingleValue.Key.Len() == 0)
				{
					continue;
				}
				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Data.SetValue(Settings_SingleValue.Value);
				SessionCreationInfo.Set(FName(*Settings_SingleValue.Key), Setting);
			}
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEIK_CreateLobby_AsyncFunction::OnCreateLobbyCompleted);
			SessionPtrRef->CreateSession(0,*SessionName,SessionCreationInfo);
		}
		else
		{
			if(bDelegateCalled == false)
			{
				OnFail.Broadcast("");
				bDelegateCalled = true;
			}
		}
	}
	else
	{
		if(bDelegateCalled == false)
		{
			OnFail.Broadcast("");
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
			OnSuccess.Broadcast(VSessionName);
			bDelegateCalled = true;
		}
	}
	else
	{
		if(bDelegateCalled == false)
		{
			OnFail.Broadcast("");
			bDelegateCalled = true;
		}
	}
}

UEIK_CreateLobby_AsyncFunction* UEIK_CreateLobby_AsyncFunction::CreateEIKLobby(FString SessionName,
                                                                               bool bAllowInvites, bool bIsLan, int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections,
                                                                               bool bShouldAdvertise, bool bAllowJoinInProgress,  bool bUseVoiceChat, bool bUsePresence, ERegionInfo Region, TMap<FString, FString> SessionSettings)
{
	UEIK_CreateLobby_AsyncFunction* Ueik_CreateLobbyObject= NewObject<UEIK_CreateLobby_AsyncFunction>();
	Ueik_CreateLobbyObject->Region = Region;
	Ueik_CreateLobbyObject->SessionName = SessionName;
	Ueik_CreateLobbyObject->bAllowInvites = bAllowInvites;
	Ueik_CreateLobbyObject->bIsLan = bIsLan;
	Ueik_CreateLobbyObject->NumberOfPublicConnections = NumberOfPublicConnections;
	Ueik_CreateLobbyObject->NumberOfPrivateConnections = NumberOfPrivateConnections;
	Ueik_CreateLobbyObject->bShouldAdvertise = bShouldAdvertise;
	Ueik_CreateLobbyObject->bAllowJoinInProgress = bAllowJoinInProgress;
	Ueik_CreateLobbyObject->SessionSettings = SessionSettings;
	Ueik_CreateLobbyObject->bUseVoiceChat = bUseVoiceChat;
	Ueik_CreateLobbyObject->bUsePresence = bUsePresence;
	return Ueik_CreateLobbyObject;
}
