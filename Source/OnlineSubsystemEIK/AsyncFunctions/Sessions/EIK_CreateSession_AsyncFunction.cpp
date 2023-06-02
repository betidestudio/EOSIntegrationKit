//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_CreateSession_AsyncFunction.h"

#include "Online/OnlineSessionNames.h"

void UEIK_CreateSession_AsyncFunction::Activate()
{
	CreateSession();
	Super::Activate();
}

void UEIK_CreateSession_AsyncFunction::CreateSession()
{
	if(IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
			SessionCreationInfo.bUsesPresence =true;
			SessionCreationInfo.bAllowJoinViaPresence = true;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = false;
			SessionCreationInfo.bAllowInvites = true;
			if(bIsDedicatedServer)
			{
				SessionCreationInfo.bUsesPresence = false;
				SessionCreationInfo.bAllowJoinViaPresence = false;
				SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = false;
				SessionCreationInfo.bAllowInvites = false;
			}
			SessionCreationInfo.bIsLANMatch = bIsLan;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.NumPrivateConnections = NumberOfPrivateConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = false;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = false;
			SessionCreationInfo.bShouldAdvertise = bShouldAdvertise;
			SessionCreationInfo.bAllowJoinInProgress = bAllowJoinInProgress;
			
			SessionCreationInfo.Settings.Add( FName(TEXT("REGIONINFO")), FOnlineSessionSetting(UEnum::GetValueAsString(Region), EOnlineDataAdvertisementType::ViaOnlineService));
			if(bIsDedicatedServer)
			{
				SessionCreationInfo.Settings.Add( FName(TEXT("PortInfo")), FOnlineSessionSetting(GetWorld()->URL.Port, EOnlineDataAdvertisementType::ViaOnlineService));
				SessionCreationInfo.Settings.Add( FName(TEXT("IsDedicatedServer")), FOnlineSessionSetting(true, EOnlineDataAdvertisementType::ViaOnlineService));
			}
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
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEIK_CreateSession_AsyncFunction::OnCreateSessionCompleted);
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

void UEIK_CreateSession_AsyncFunction::OnCreateSessionCompleted(FName VSessionName, bool bWasSuccessful)
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

UEIK_CreateSession_AsyncFunction* UEIK_CreateSession_AsyncFunction::CreateEIKSession(FString SessionName,
                                                                                     bool bIsDedicatedServer, bool bIsLan, int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections,
                                                                                     bool bShouldAdvertise, bool bAllowJoinInProgress, ERegionInfo Region, TMap<FString, FString> SessionSettings)
{
	UEIK_CreateSession_AsyncFunction* Ueik_CreateSessionObject= NewObject<UEIK_CreateSession_AsyncFunction>();
	Ueik_CreateSessionObject->Region = Region;
	Ueik_CreateSessionObject->SessionName = SessionName;
	Ueik_CreateSessionObject->bIsDedicatedServer = bIsDedicatedServer;
	Ueik_CreateSessionObject->bIsLan = bIsLan;
	Ueik_CreateSessionObject->NumberOfPublicConnections = NumberOfPublicConnections;
	Ueik_CreateSessionObject->NumberOfPrivateConnections = NumberOfPrivateConnections;
	Ueik_CreateSessionObject->bShouldAdvertise = bShouldAdvertise;
	Ueik_CreateSessionObject->bAllowJoinInProgress = bAllowJoinInProgress;
	Ueik_CreateSessionObject->SessionSettings = SessionSettings;
	return Ueik_CreateSessionObject;
}
