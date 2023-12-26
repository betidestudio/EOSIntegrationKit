//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_FindSessionByID_AsyncFunction.h"

#include "Online/OnlineSessionNames.h"

UEIK_FindSessionByID_AsyncFunction* UEIK_FindSessionByID_AsyncFunction::FindEIKSessionByID(FString SessionID)
{
	UEIK_FindSessionByID_AsyncFunction* Ueik_FindSessionObject= NewObject<UEIK_FindSessionByID_AsyncFunction>();
	Ueik_FindSessionObject->Var_SessionID = SessionID;
	return Ueik_FindSessionObject;
}

void UEIK_FindSessionByID_AsyncFunction::Activate()
{
	FindSession();
	Super::Activate();
}



void UEIK_FindSessionByID_AsyncFunction::FindSession()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			const FUniqueNetIdPtr SearchingUserId = IOnlineSubsystem::Get()->GetIdentityInterface()->GetUniquePlayerId(0);
			const FUniqueNetIdRef UserNetId = FUniqueNetIdString::Create(SearchingUserId.Get()->ToString(),"EIK");
			const FUniqueNetIdRef SessionID = FUniqueNetIdString::Create(Var_SessionID,"EIK");
			const FUniqueNetIdRef SearchingUserIdRef = FUniqueNetIdString::EmptyId();
			const FOnSingleSessionResultCompleteDelegate OnSingleSessionResultCompleteDelegate = FOnSingleSessionResultCompleteDelegate::CreateUObject(this, &UEIK_FindSessionByID_AsyncFunction::OnFindSessionCompleted);
			SessionPtrRef->FindSessionById(*UserNetId,*SessionID,*UserNetId,OnSingleSessionResultCompleteDelegate);
		}
		else
		{
			if(bDelegateCalled)
			{
				return;
			}
			OnFail.Broadcast(FBlueprintSessionResult());
			SetReadyToDestroy();
		}
	}
	else
	{
		if(bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast(FBlueprintSessionResult());
		SetReadyToDestroy();
	}
}


void UEIK_FindSessionByID_AsyncFunction::OnFindSessionCompleted(int I, bool bArg,
	const FOnlineSessionSearchResult& OnlineSessionSearchResult)
{
	if(!bArg)
	{
		if(bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast(FBlueprintSessionResult());
		bDelegateCalled = true;
		SetReadyToDestroy();
	}
	FBlueprintSessionResult SessionResult;
	SessionResult.OnlineResult = OnlineSessionSearchResult;
	OnSuccess.Broadcast(SessionResult);
	bDelegateCalled = true;
	SetReadyToDestroy();
}
