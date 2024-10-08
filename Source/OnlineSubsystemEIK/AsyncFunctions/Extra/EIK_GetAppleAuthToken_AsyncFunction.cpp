// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_GetAppleAuthToken_AsyncFunction.h"


UEIK_GetAppleAuthToken_AsyncFunction* UEIK_GetAppleAuthToken_AsyncFunction::GetAppleAuthToken()
{
	UEIK_GetAppleAuthToken_AsyncFunction* Node = NewObject<UEIK_GetAppleAuthToken_AsyncFunction>();
	return Node;
}

void UEIK_GetAppleAuthToken_AsyncFunction::OnLoginComplete(int LocalUserNum, bool bWasSuccess,
                                                           const FUniqueNetId& UniqueNetId, const FString& Error)
{
	if (bWasSuccess)
	{
		if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(APPLE_SUBSYSTEM))
		{
			if (IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface())
			{
				if (!Identity->GetAuthToken(0).IsEmpty())
				{
					UE_LOG(LogEIK, Display, TEXT("Apple Auth Token found"));
					OnSuccess.Broadcast(Identity->GetAuthToken(0));
					SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
					MarkAsGarbage();
#else
					MarkPendingKill();
#endif
					return;
				}
			}
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get Apple Auth Token due to login failure : %s"), *Error);
	OnFailure.Broadcast(TEXT(""));
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_GetAppleAuthToken_AsyncFunction::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(APPLE_SUBSYSTEM))
	{
		if (IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface())
		{
			if(Identity->GetAuthToken(0).IsEmpty())
			{
				UE_LOG(LogEIK, Display, TEXT("No Apple Auth Token found, attempting to login"));
				Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateUObject(this, &UEIK_GetAppleAuthToken_AsyncFunction::OnLoginComplete));
				Identity->AutoLogin(0);
				return;
			}
			UE_LOG(LogEIK, Display, TEXT("Apple Auth Token found"));
			OnSuccess.Broadcast(Identity->GetAuthToken(0));
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get Apple Auth Token due to missing OnlineSubsystem or IdentityInterface"));
	OnFailure.Broadcast(TEXT(""));
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
