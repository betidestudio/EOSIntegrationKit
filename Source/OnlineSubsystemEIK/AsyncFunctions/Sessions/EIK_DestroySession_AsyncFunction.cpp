//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_DestroySession_AsyncFunction.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

UEIK_DestroySession_AsyncFunction* UEIK_DestroySession_AsyncFunction::DestroyEIKSessions(FName SessionName)
{
	UEIK_DestroySession_AsyncFunction* Ueik_DestroySessionObject = NewObject<UEIK_DestroySession_AsyncFunction>();
	Ueik_DestroySessionObject->Var_SessionName = SessionName;
	return Ueik_DestroySessionObject;
}

void UEIK_DestroySession_AsyncFunction::Activate()
{
	DestroySession();
	Super::Activate();
}

void UEIK_DestroySession_AsyncFunction::DestroySession()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this,&UEIK_DestroySession_AsyncFunction::OnDestroySessionCompleted);
			SessionPtrRef->DestroySession(Var_SessionName);
		}
		else
		{
			if(bDelegateCalled)
			{
				return;
			}
			OnFail.Broadcast();
			bDelegateCalled = true;
		}
	}
	else
	{
		if(bDelegateCalled)
		{
			return;
		}
		OnFail.Broadcast();
		bDelegateCalled = true;
	}
}

void UEIK_DestroySession_AsyncFunction::OnDestroySessionCompleted(FName SessionName, bool bWasSuccess) const
{
	if(bDelegateCalled)
	{
		return;
	}
	if(bWasSuccess)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}
}
