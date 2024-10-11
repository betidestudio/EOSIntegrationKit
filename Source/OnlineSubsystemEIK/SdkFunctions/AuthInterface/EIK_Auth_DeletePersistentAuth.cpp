// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Auth_DeletePersistentAuth.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Auth_DeletePersistentAuth* UEIK_Auth_DeletePersistentAuth::DeletePersistentAuth(FString RefreshToken)
{
	UEIK_Auth_DeletePersistentAuth* Node = NewObject<UEIK_Auth_DeletePersistentAuth>();
	Node->Var_RefreshToken = RefreshToken;
	return Node;
}

void UEIK_Auth_DeletePersistentAuth::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_DeletePersistentAuthOptions Options = {};
			Options.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
			if(Var_RefreshToken.IsEmpty())
			{
				Options.RefreshToken = nullptr;
			}
			else
			{
				Options.RefreshToken = TCHAR_TO_ANSI(*Var_RefreshToken);
			}
			EOS_Auth_DeletePersistentAuth(EOSRef->AuthHandle, &Options, this, &UEIK_Auth_DeletePersistentAuth::OnDeletePersistentAuthCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to delete persistent auth either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Auth_DeletePersistentAuth::OnDeletePersistentAuthCallback(const EOS_Auth_DeletePersistentAuthCallbackInfo* Data)
{
	UEIK_Auth_DeletePersistentAuth* Node = static_cast<UEIK_Auth_DeletePersistentAuth*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
