// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_CreateUser.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"


UEIK_Connect_CreateUser* UEIK_Connect_CreateUser::CreateUser(FEIK_ContinuanceToken ContinuanceToken)
{
	UEIK_Connect_CreateUser* Node = NewObject<UEIK_Connect_CreateUser>();
	Node->Var_ContinuanceToken = ContinuanceToken;
	return Node;
}

void UEIK_Connect_CreateUser::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_CreateUserOptions CreateUserOptions = { };
			CreateUserOptions.ApiVersion = EOS_CONNECT_CREATEUSER_API_LATEST;
			CreateUserOptions.ContinuanceToken = Var_ContinuanceToken.GetValueAsEosType();
			EOS_Connect_CreateUser(EOSRef->ConnectHandle, &CreateUserOptions, this, &UEIK_Connect_CreateUser::OnCreateUserCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to create user either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Connect_CreateUser::OnCreateUserCallback(const EOS_Connect_CreateUserCallbackInfo* Data)
{
	UEIK_Connect_CreateUser* Node = static_cast<UEIK_Connect_CreateUser*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
