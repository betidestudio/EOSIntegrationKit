//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_JoinSession_AsyncFunction.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

UEIK_JoinSession_AsyncFunction* UEIK_JoinSession_AsyncFunction::JoinEIKSessions(FSessionFindStruct SessionToJoin)
{
	UEIK_JoinSession_AsyncFunction* Ueik_JoinSessionObject = NewObject<UEIK_JoinSession_AsyncFunction>();
	Ueik_JoinSessionObject->Var_SessionToJoin = SessionToJoin;
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
	if(!Var_SessionToJoin.SessionName.IsEmpty())
	{
		if(SubsystemRef)
		{
			if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
			{
				SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEIK_JoinSession_AsyncFunction::OnJoinSessionCompleted);
				SessionPtrRef->JoinSession(0, FName(Var_SessionToJoin.SessionName),Var_SessionToJoin.SessionResult.OnlineResult);
			}
			else
			{
				if(bDelegateCalled)
				{
					return;
				}
				OnFail.Broadcast(EEIKJoinResult::UnknownError, FString());
				bDelegateCalled = true;
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
	}
}

void UEIK_JoinSession_AsyncFunction::OnJoinSessionCompleted(FName SessionName,
	EOnJoinSessionCompleteResult::Type Result)
{
	if(bDelegateCalled)
	{
		return;
	}
	if(Result==EOnJoinSessionCompleteResult::Success)
	{
		if(APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(),0))
		{
			if(const IOnlineSubsystem *SubsystemRef =  IOnlineSubsystem::Get())
			{
				if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
				{
					FString JoinAddress;
					SessionPtrRef->GetResolvedConnectString(SessionName,JoinAddress);
					if(Var_SessionToJoin.bIsDedicatedServer)
					{
						int32 PortInfo = 7777;
						if(Var_SessionToJoin.SessionSettings.Contains("PortInfo"))
						{
							Var_SessionToJoin.SessionResult.OnlineResult.Session.SessionSettings.Get("PortInfo", PortInfo);							
						}
						TArray<FString> IpPortArray;
						JoinAddress.ParseIntoArray(IpPortArray, TEXT(":"), true);
						const FString IpAddress = IpPortArray[0];
						const FString NewCustomIP = IpAddress + ":" + FString::FromInt(PortInfo);
						JoinAddress = NewCustomIP;
					}
					if(!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress,ETravelType::TRAVEL_Absolute);
						OnSuccess.Broadcast(EEIKJoinResult::Success, JoinAddress);
						bDelegateCalled = true;
						return;
					}
					else
					{
						bDelegateCalled = true;
						OnFail.Broadcast(EEIKJoinResult::CouldNotRetrieveAddress, FString());
						return;
					}
				}
				else
				{
					switch (Result) { case EOnJoinSessionCompleteResult::Success:
						OnFail.Broadcast(EEIKJoinResult::Success, FString());
						bDelegateCalled = true;
						break;
					case EOnJoinSessionCompleteResult::SessionIsFull:
						OnFail.Broadcast(EEIKJoinResult::SessionIsFull, FString());
						bDelegateCalled = true;
						break;
					case EOnJoinSessionCompleteResult::SessionDoesNotExist:
						OnFail.Broadcast(EEIKJoinResult::SessionDoesNotExist, FString());
						bDelegateCalled = true;
						break;
					case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
						OnFail.Broadcast(EEIKJoinResult::CouldNotRetrieveAddress, FString());
						bDelegateCalled = true;
						break;
					case EOnJoinSessionCompleteResult::AlreadyInSession:
						OnFail.Broadcast(EEIKJoinResult::AlreadyInSession, FString());
						bDelegateCalled = true;
						break;
					case EOnJoinSessionCompleteResult::UnknownError:
						OnFail.Broadcast(EEIKJoinResult::UnknownError, FString());
						bDelegateCalled = true;
						break;
					default:
						OnFail.Broadcast(EEIKJoinResult::UnknownError, FString());
						bDelegateCalled = true;;
					}
				}
				bDelegateCalled = true;
			}
		}
	}
}

