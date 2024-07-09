// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_GetPlatformAuthToken_AsyncFunction.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "UserManagerEOS.h"

UEIK_GetPlatformAuthToken_AsyncFunction* UEIK_GetPlatformAuthToken_AsyncFunction::GetPlatformAuthToken()
{
	UEIK_GetPlatformAuthToken_AsyncFunction* Node = NewObject<UEIK_GetPlatformAuthToken_AsyncFunction>();
	return Node;
}

void UEIK_GetPlatformAuthToken_AsyncFunction::OnGetPlatformAuthTokenComplete(int I, bool bArg,
                                                                             const FExternalAuthToken& ExternalAuthToken)
{
	if(bArg)
	{
		OnSuccess.Broadcast(ExternalAuthToken.TokenString);
	}
	else
	{
		OnFailure.Broadcast(ExternalAuthToken.TokenString);
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UEIK_GetPlatformAuthToken_AsyncFunction::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(FUserManagerEOS* UserManager = EOSRef->UserManager.Get())
			{
				FUserManagerEOS::FOnGetLinkedAccountAuthTokenCompleteDelegate Delegate = FUserManagerEOS::FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateUObject(this, &UEIK_GetPlatformAuthToken_AsyncFunction::OnGetPlatformAuthTokenComplete);
				UserManager->GetPlatformAuthToken(0,Delegate);
				return;
			}
		}
	}
	
	SetReadyToDestroy();
	MarkAsGarbage();
}
