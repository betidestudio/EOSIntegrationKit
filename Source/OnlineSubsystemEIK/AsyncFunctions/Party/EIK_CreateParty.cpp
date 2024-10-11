// Copyright (c) 2024 Betide Studio. All Rights Reserved.


#include "EIK_CreateParty.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemUtils.h"

UEIK_CreateParty* UEIK_CreateParty::EIK_CreateParty(int32 MaxPublicPartyMembers,
	FEIK_PartyExtraSettings ExtraPartySettings)
{
	UEIK_CreateParty* PartyCreateParty = NewObject<UEIK_CreateParty>();
	PartyCreateParty->Var_MaxPublicPartyMembers = MaxPublicPartyMembers;
	PartyCreateParty->Var_CreatePartySettings = ExtraPartySettings;
	return PartyCreateParty;
}

void UEIK_CreateParty::Activate()
{
	Super::Activate();
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld(), "EIK"))
	{
		if(const IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface())
		{
			if(IdentityPtrRef->GetLoginStatus(0) != ELoginStatus::LoggedIn)
			{
				UE_LOG(LogEIK, Warning, TEXT("You must be logged in to create a party."));
				OnFailure.Broadcast();
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif
				return;
			}
		}
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = false;
			SessionCreationInfo.bAllowInvites = true;
			SessionCreationInfo.bIsLANMatch = false;
			SessionCreationInfo.NumPublicConnections = Var_MaxPublicPartyMembers;
			SessionCreationInfo.NumPrivateConnections = Var_CreatePartySettings.MaxPrivatePartyMembers;
			SessionCreationInfo.bUseLobbiesIfAvailable = true;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = Var_CreatePartySettings.bUseVoiceChat;
			SessionCreationInfo.bUsesPresence = Var_CreatePartySettings.bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresence = Var_CreatePartySettings.bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = false;
			SessionCreationInfo.bShouldAdvertise = true;
			SessionCreationInfo.bAllowJoinInProgress = true;
#if ENGINE_MAJOR_VERSION == 5
			SessionCreationInfo.Set(SETTING_HOST_MIGRATION, true, EOnlineDataAdvertisementType::ViaOnlineService);
#else
			SessionCreationInfo.Set("SETTING_HOST_MIGRATION", true, EOnlineDataAdvertisementType::ViaOnlineService);
#endif
			{
				FOnlineSessionSetting LocalVNameSetting;
				LocalVNameSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				LocalVNameSetting.Data = *Var_CreatePartySettings.SessionName.ToString();
				SessionCreationInfo.Set(FName(TEXT("SessionName")), LocalVNameSetting);
			}
			{
				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				FVariantData PartyData;
				PartyData.SetValue(true);
				Setting.Data = PartyData;
				SessionCreationInfo.Set(FName(TEXT("IsPartySession")), Setting);
			}
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEIK_CreateParty::OnCreatePartyCompleted);
			SessionPtrRef->CreateSession(0, Var_CreatePartySettings.SessionName, SessionCreationInfo);
			return;
		}
		UE_LOG(LogEIK, Warning, TEXT("Failed to create party session because the session interface is not valid."));
		OnFailure.Broadcast();
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
		return;
	}
	UE_LOG(LogEIK, Warning, TEXT("Failed to create party session because the subsystem is not valid."));
	OnFailure.Broadcast();
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_CreateParty::OnCreatePartyCompleted(FName SessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogEIK, Log, TEXT("Party session created successfully."));
		OnSuccess.Broadcast();
	}
	else
	{
		UE_LOG(LogEIK, Warning, TEXT("Failed to create party session."));
		OnFailure.Broadcast();
	}
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
