// Fill out your copyright notice in the Description page of Project Settings.


#include "EIK_SetPresence_AsyncFunction.h"
#include "IOnlineSubsystemEOS.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Online/OnlineSessionNames.h"

void UEIK_SetPresence_AsyncFunction::Activate()
{
	SetPresence();
	Super::Activate(); 
}


void UEIK_SetPresence_AsyncFunction::SetPresence()
{
	if (IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld())) 
	{
		//Updating Presence
		IOnlineSubsystem* Subsystem = Online::GetSubsystem(this->GetWorld());
		IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
		IOnlinePresencePtr Presence = Subsystem->GetPresenceInterface();
		FOnlineUserPresenceStatus Status;

		/*//Switch On Presence Status
		switch (PresenceStatus) 
		{
		case EPresenceStatus::PR_Online:
			Status.State = EOnlinePresenceState::Online;
			UE_LOG(LogTemp, Log, TEXT("Presence Set To Online"));
			break;
		case EPresenceStatus::PR_Offline:
			UE_LOG(LogTemp, Error, TEXT("Cant Change Presence To Offline When User Is Playing"));
			break;
		case EPresenceStatus::PR_Away:
			Status.State = EOnlinePresenceState::Away;
			UE_LOG(LogTemp, Error, TEXT("Presence Set To Away"));
			break;
		case EPresenceStatus::PR_ExtendedAway:
			Status.State = EOnlinePresenceState::ExtendedAway;
			UE_LOG(LogTemp, Error, TEXT("Presence Set To ExtendedAway"));
			break;
		case EPresenceStatus::PR_DoNotDisturb:
			Status.State = EOnlinePresenceState::DoNotDisturb;
			UE_LOG(LogTemp, Error, TEXT("Presence Set To DoNotDisturb"));
			break;
		}
		
		FString RichPresenceStatus = RichPresense;
		Status.StatusStr = RichPresenceStatus;
		UE_LOG(LogTemp, Warning, TEXT("Presence Updated With Status: %s"), *RichPresenceStatus);
		Presence->SetPresence(
			*Identity->GetUniquePlayerId(0).Get(),
			Status,
			IOnlinePresence::FOnPresenceTaskCompleteDelegate::CreateLambda([](
				const class FUniqueNetId& UserId,
				const bool bWasSuccessful)
				{
					if (bWasSuccessful) // Check bWasSuccessful.
						UE_LOG(LogTemp, Log, TEXT("Presence Updated Successfully"));
				}));
		SetReadyToDestroy();
	}*/
	}
}

UEIK_SetPresence_AsyncFunction* UEIK_SetPresence_AsyncFunction::SetEOSPresence(FString RichPresense)
{
	UEIK_SetPresence_AsyncFunction* Obj = NewObject< UEIK_SetPresence_AsyncFunction>();
	// Ueik_SetPresenceObject->RichPresence = RichPresense;
	// Ueik_SetPresenceObject->PresenceStatus = PresenceStatus;
	return Obj;
}
