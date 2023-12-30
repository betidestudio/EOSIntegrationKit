//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_JoinSession_AsyncFunction.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

UEIK_JoinSession_AsyncFunction* UEIK_JoinSession_AsyncFunction::JoinEIKSessions(UObject* WorldContextObject, FSessionFindStruct SessionToJoin)
{
	UEIK_JoinSession_AsyncFunction* Ueik_JoinSessionObject = NewObject<UEIK_JoinSession_AsyncFunction>();
	Ueik_JoinSessionObject->Var_SessionToJoin = SessionToJoin;
	Ueik_JoinSessionObject->Var_WorldContextObject = WorldContextObject;
	return Ueik_JoinSessionObject;
}

void UEIK_JoinSession_AsyncFunction::Activate()
{
	JoinSession();
	Super::Activate();
}

void UEIK_JoinSession_AsyncFunction::JoinSession()
{
	const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
 			SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEIK_JoinSession_AsyncFunction::OnJoinSessionCompleted);
			SessionPtrRef->JoinSession(0, NAME_GameSession,Var_SessionToJoin.SessionResult.OnlineResult);
		}
		else
		{
			if(bDelegateCalled)
			{
				return;
			}
			OnFail.Broadcast(EEIKJoinResult::UnknownError, FString());
			bDelegateCalled = true;
			SetReadyToDestroy();
		}
	}
	else
	{
		if(bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast(EEIKJoinResult::UnknownError, FString());
		bDelegateCalled = true;
		SetReadyToDestroy();
	}
}
void UEIK_JoinSession_AsyncFunction::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (bDelegateCalled)
	{
		return;
	}
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(Var_WorldContextObject, 0))
		{
			if (const IOnlineSubsystem* SubsystemRef = IOnlineSubsystem::Get())
			{
				const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
				if (SessionPtrRef.IsValid())
				{
					FString JoinAddress;
					SessionPtrRef->GetResolvedConnectString(SessionName, JoinAddress);
					if (Var_SessionToJoin.bIsDedicatedServer)
					{
						FString PortInfo = "7777";
						if (Var_SessionToJoin.SessionSettings.Contains("PortInfo"))
						{
							Var_SessionToJoin.SessionResult.OnlineResult.Session.SessionSettings.Get("PortInfo", PortInfo);
						}
						TArray<FString> IpPortArray;
						JoinAddress.ParseIntoArray(IpPortArray, TEXT(":"), true);
						const FString IpAddress = IpPortArray[0];
						JoinAddress = IpAddress + ":" + PortInfo;
					}

					if (!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
						OnSuccess.Broadcast(EEIKJoinResult::Success, JoinAddress);
						bDelegateCalled = true;
						SetReadyToDestroy();
						return;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("EIK: Could not retrieve address"));
						OnFail.Broadcast(EEIKJoinResult::CouldNotRetrieveAddress, FString());
						bDelegateCalled = true;
						SetReadyToDestroy();
						return;
					}
				}
			}
		}
		else
		{
			OnFail.Broadcast(EEIKJoinResult::UnknownError, FString());
			bDelegateCalled = true;
			SetReadyToDestroy();
			return;
		}
	}
	else
	{
		switch (Result)
		{
			case EOnJoinSessionCompleteResult::SessionIsFull:
				OnFail.Broadcast(EEIKJoinResult::SessionIsFull, FString());
				break;
			case EOnJoinSessionCompleteResult::SessionDoesNotExist:
				OnFail.Broadcast(EEIKJoinResult::SessionDoesNotExist, FString());
				break;
			case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
				OnFail.Broadcast(EEIKJoinResult::CouldNotRetrieveAddress, FString());
				break;
			case EOnJoinSessionCompleteResult::AlreadyInSession:
				OnFail.Broadcast(EEIKJoinResult::AlreadyInSession, FString());
				break;
			default:
				OnFail.Broadcast(EEIKJoinResult::UnknownError, FString());
		}
		SetReadyToDestroy();
		bDelegateCalled = true;
	}
}


