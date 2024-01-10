//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_FindSessions_AsyncFunction.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "Online/OnlineSessionNames.h"

UEIK_FindSessions_AsyncFunction* UEIK_FindSessions_AsyncFunction::FindEIKSessions(
	TMap<FName, FEIKAttribute> SessionSettings, EMatchType MatchType, int32 MaxResults, ERegionInfo RegionToSearch)

{
	UEIK_FindSessions_AsyncFunction* Ueik_FindSessionObject= NewObject<UEIK_FindSessions_AsyncFunction>();
	Ueik_FindSessionObject->E_MatchType = MatchType;
	Ueik_FindSessionObject->E_RegionToSearch = RegionToSearch;
	Ueik_FindSessionObject->SessionSettings = SessionSettings;
	Ueik_FindSessionObject->I_MaxResults = MaxResults;
	return Ueik_FindSessionObject;
}

void UEIK_FindSessions_AsyncFunction::Activate()
{
	FindSession();
	Super::Activate();
}

void UEIK_FindSessions_AsyncFunction::FindSession()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->QuerySettings.SearchParams.Empty();
			SessionSearch->bIsLanQuery = false;
			if(E_MatchType == EMatchType::MT_MatchMakingSession)
			{
				if(E_RegionToSearch!=ERegionInfo::RE_NoSelection)
				{
					SessionSearch->QuerySettings.Set(FName(TEXT("RegionInfo")),UEnum::GetValueAsString(E_RegionToSearch), EOnlineComparisonOp::Equals);
				}
			}
			else
			{
				SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			}
			if (!SessionSettings.IsEmpty()) {
				for (auto& Settings_SingleValue : SessionSettings) {
					if (Settings_SingleValue.Key.IsNone()) {
						continue;
					}
					FOnlineSessionSetting Setting;
					Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
					Setting.Data = Settings_SingleValue.Value.GetVariantData();
					if(Settings_SingleValue.Value.AttributeType == EEIKAttributeType::String)
					{
						SessionSearch->QuerySettings.Set(Settings_SingleValue.Key, Settings_SingleValue.Value.StringValue, EOnlineComparisonOp::Equals);
					}
					else if(Settings_SingleValue.Value.AttributeType == EEIKAttributeType::Integer)
					{
						SessionSearch->QuerySettings.Set(Settings_SingleValue.Key, Settings_SingleValue.Value.IntValue, EOnlineComparisonOp::Equals);
					}
					else if(Settings_SingleValue.Value.AttributeType == EEIKAttributeType::Bool)
					{
						SessionSearch->QuerySettings.Set(Settings_SingleValue.Key, Settings_SingleValue.Value.BoolValue, EOnlineComparisonOp::Equals);
					}
				}
			}
			SessionSearch->MaxSearchResults = I_MaxResults;
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEIK_FindSessions_AsyncFunction::OnFindSessionCompleted);
			SessionPtrRef->FindSessions(0,SessionSearch.ToSharedRef());
		}
		else
		{
			if(bDelegateCalled)
			{
				return;
			}
			OnFail.Broadcast(TArray<FSessionFindStruct>());
			SetReadyToDestroy();
		}
	}
	else
	{
		if(bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast(TArray<FSessionFindStruct>());
		SetReadyToDestroy();
	}
}

void UEIK_FindSessions_AsyncFunction::OnFindSessionCompleted(bool bWasSuccess)
{
	if(!bWasSuccess)
	{
		if(bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast(TArray<FSessionFindStruct>());
		bDelegateCalled = true;
		SetReadyToDestroy();
	}
	if (const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		TArray<FSessionFindStruct> SessionResult_Array;
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			if (SessionSearch->SearchResults.Num() > 0)
			{
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					FBlueprintSessionResult SessionResult;
					SessionResult.OnlineResult = SessionSearch->SearchResults[SearchIdx];
					FOnlineSessionSettings LocalSessionSettings  = SessionResult.OnlineResult.Session.SessionSettings;
					TMap<FName, FString> AllSettingsWithData;
					TMap<FName, FOnlineSessionSetting>::TIterator It(LocalSessionSettings.Settings);

					TMap<FString, FEIKAttribute> LocalArraySettings;
					while (It)
					{
						const FName& SettingName = It.Key();
						const FOnlineSessionSetting& Setting = It.Value();
						LocalArraySettings.Add(*SettingName.ToString(), Setting.Data);
						++It;
					}
					
					bool IsServer = LocalArraySettings.Contains("IsDedicatedServer") ? true : false;
					FSessionFindStruct LocalStruct;
					LocalStruct.SessionName = "GameSession";
					LocalStruct.CurrentNumberOfPlayers = (SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections + SessionResult.OnlineResult.Session.SessionSettings.NumPrivateConnections) - (SessionResult.OnlineResult.Session.NumOpenPublicConnections + SessionResult.OnlineResult.Session.NumOpenPrivateConnections);
					LocalStruct.MaxNumberOfPlayers = SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections + SessionResult.OnlineResult.Session.SessionSettings.NumPrivateConnections;
					LocalStruct.SessionResult= SessionResult;
					LocalStruct.SessionSettings = LocalArraySettings;
					LocalStruct.bIsDedicatedServer = IsServer;
					SessionResult_Array.Add(LocalStruct);
				}
			}
		}
		if(bDelegateCalled)
		{
			return;
		}
		bDelegateCalled = true;
		OnSuccess.Broadcast(SessionResult_Array);
		SetReadyToDestroy();
	}
}
