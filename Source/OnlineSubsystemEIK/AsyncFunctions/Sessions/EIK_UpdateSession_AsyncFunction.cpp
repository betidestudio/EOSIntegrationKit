// Copyright Epic Games, Inc. All Rights Reserved.

#include "EIK_UpdateSession_AsyncFunction.h"


UEIK_UpdateSession_AsyncFunction* UEIK_UpdateSession_AsyncFunction::UpdateEIKSessions(UObject* WorldContextObject,
	TMap<FString, FEIKAttribute> SessionSettings, TArray<FEIK_MemberSpecificAttribute> MemberSettings, FName SessionName,
	bool bShouldAdvertise, bool bAllowJoinInProgress, bool bAllowInvites, bool bUsesPresence, bool bIsLANMatch,
	bool bIsDedicatedServer, bool bIsLobbySession, int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections)
{
	UEIK_UpdateSession_AsyncFunction* UpdateSession = NewObject<UEIK_UpdateSession_AsyncFunction>();
	UpdateSession->Var_WorldContextObject = WorldContextObject;
	UpdateSession->Var_SessionSettings = SessionSettings;
	UpdateSession->Var_bShouldAdvertise = bShouldAdvertise;
	UpdateSession->Var_bAllowJoinInProgress = bAllowJoinInProgress;
	UpdateSession->Var_MemberSettings = MemberSettings;
	UpdateSession->Var_bAllowInvites = bAllowInvites;
	UpdateSession->Var_SessionName = SessionName;
	UpdateSession->Var_bUsesPresence = bUsesPresence;
	UpdateSession->Var_NumberOfPublicConnections = NumberOfPublicConnections;
	UpdateSession->Var_NumberOfPrivateConnections = NumberOfPrivateConnections;
	UpdateSession->Var_bIsLANMatch = bIsLANMatch;
	UpdateSession->Var_bIsDedicatedServer = bIsDedicatedServer;
	UpdateSession->Var_bIsLobbySession = bIsLobbySession;
	return UpdateSession;
}

void UEIK_UpdateSession_AsyncFunction::OnUpdateSessionComplete(FName Name, bool bArg)
{
	if (bArg)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_UpdateSession_AsyncFunction::Activate()
{
	Super::Activate();
	if (const IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if (const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bAllowJoinInProgress = Var_bAllowJoinInProgress;
			SessionSettings.bAllowInvites = Var_bAllowInvites;
			SessionSettings.bShouldAdvertise = Var_bShouldAdvertise;
			SessionSettings.bUsesPresence = Var_bUsesPresence;
			SessionSettings.NumPublicConnections = Var_NumberOfPublicConnections;
			SessionSettings.NumPrivateConnections = Var_NumberOfPrivateConnections;
			SessionSettings.bIsLANMatch = Var_bIsLANMatch;
			SessionSettings.bUseLobbiesIfAvailable = Var_bIsLobbySession;
			SessionSettings.bIsDedicated = Var_bIsDedicatedServer;
			if(Var_MemberSettings.Num() > 0)
			{
				TUniqueNetIdMap<FSessionSettings> LocalMemberSettings;
				for(auto& MemberSetting : Var_MemberSettings)
				{
					FSessionSettings LocalSessionSettings;
					for(auto& LocalMemberSpecificAttribute : MemberSetting.Attributes)
					{
						LocalSessionSettings.Add(FName(*LocalMemberSpecificAttribute.Key), LocalMemberSpecificAttribute.Value.GetVariantData());
					}
					LocalMemberSettings.Add(MemberSetting.MemberId.GetUniqueNetId()->AsShared(), LocalSessionSettings);
				}
				SessionSettings.MemberSettings = LocalMemberSettings;
			}
			for (auto& Settings_SingleValue : Var_SessionSettings)
			{
				if (Settings_SingleValue.Key.Len() == 0)
				{
					continue;
				}
				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Data = Settings_SingleValue.Value.GetVariantData();
				SessionSettings.Settings.Add(FName(*Settings_SingleValue.Key), Setting);
			}
			SessionPtrRef->OnUpdateSessionCompleteDelegates.AddUObject(
				this, &UEIK_UpdateSession_AsyncFunction::OnUpdateSessionComplete);
			SessionPtrRef->UpdateSession(Var_SessionName, SessionSettings, true);
		}
		else
		{
			OnFailure.Broadcast();
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
		OnFailure.Broadcast();
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}
