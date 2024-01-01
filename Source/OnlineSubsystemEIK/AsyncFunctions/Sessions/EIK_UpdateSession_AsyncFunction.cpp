//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_UpdateSession_AsyncFunction.h"


UEIK_UpdateSession_AsyncFunction* UEIK_UpdateSession_AsyncFunction::UpdateEIKSessions(UObject* WorldContextObject,TMap<FString, FString> SessionSettings, bool bShouldAdvertise, bool bAllowJoinInProgress, bool bAllowInvites, bool bUsesPresence, int32 NumberOfPublicConnections,int32 NumberOfPrivateConnections,bool bRefreshOnlineData)
{
	UEIK_UpdateSession_AsyncFunction* UpdateSession = NewObject<UEIK_UpdateSession_AsyncFunction>();
	UpdateSession->Var_WorldContextObject = WorldContextObject;
	UpdateSession->Var_SessionSettings = SessionSettings;
	UpdateSession->Var_bShouldAdvertise = bShouldAdvertise;
	UpdateSession->Var_bAllowJoinInProgress = bAllowJoinInProgress;
	UpdateSession->Var_bAllowInvites = bAllowInvites;
	UpdateSession->Var_bUsesPresence = bUsesPresence;
	UpdateSession->Var_NumberOfPublicConnections = NumberOfPublicConnections;
	UpdateSession->Var_NumberOfPrivateConnections = NumberOfPrivateConnections;
	return UpdateSession;
}

void UEIK_UpdateSession_AsyncFunction::OnUpdateSessionComplete(FName Name, bool bArg)
{
	if(bArg)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
	SetReadyToDestroy();
}

void UEIK_UpdateSession_AsyncFunction::Activate()
{
	Super::Activate();
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bAllowJoinInProgress = Var_bAllowJoinInProgress;
			SessionSettings.bAllowInvites = Var_bAllowInvites;
			SessionSettings.bShouldAdvertise = Var_bShouldAdvertise;
			SessionSettings.bUsesPresence = Var_bUsesPresence;
			SessionSettings.NumPublicConnections = Var_NumberOfPublicConnections;
			SessionSettings.NumPrivateConnections = Var_NumberOfPrivateConnections;
			for (auto& Settings_SingleValue : Var_SessionSettings)
			{
				if (Settings_SingleValue.Key.Len() == 0)
				{
					continue;
				}
				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Data.SetValue(Settings_SingleValue.Value);
				SessionSettings.Settings.Add( FName(*Settings_SingleValue.Key), Setting);
			}
			SessionPtrRef->OnUpdateSessionCompleteDelegates.AddUObject(this, &UEIK_UpdateSession_AsyncFunction::OnUpdateSessionComplete);
			SessionPtrRef->UpdateSession(NAME_GameSession, SessionSettings, Var_bRefreshOnlineData);
 		}
		else
		{
			OnFailure.Broadcast();
			SetReadyToDestroy();
		}
	}
	else
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
	}
}
