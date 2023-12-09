//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Logout_AsyncFunction.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

UEIK_Logout_AsyncFunction* UEIK_Logout_AsyncFunction::LogoutUsingEIK()
{
	UEIK_Logout_AsyncFunction* BlueprintNode = NewObject<UEIK_Logout_AsyncFunction>();
	return BlueprintNode;
}

void UEIK_Logout_AsyncFunction::Activate()
{
	LogoutPlayer();
	Super::Activate();
}

void UEIK_Logout_AsyncFunction::LogoutPlayer()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			IdentityPointerRef->OnLogoutCompleteDelegates->AddUObject(this,&UEIK_Logout_AsyncFunction::LogoutCallback);
			IdentityPointerRef->Logout(0);
		}
		else
		{
			OnFail.Broadcast();
			SetReadyToDestroy();
		}
	}
	else
	{
		OnFail.Broadcast();
		SetReadyToDestroy();
	}
}

void UEIK_Logout_AsyncFunction::LogoutCallback(int32 LocalUserNum, bool bWasSuccess)
{
	if(bWasSuccess)
	{
		OnSuccess.Broadcast();
		SetReadyToDestroy();
		return;
	}
	else
	{
		OnFail.Broadcast();
		SetReadyToDestroy();
		return;
	}
}
