// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_LinkAccount_AsyncFunction.h"

#include "Async/Async.h"


UEIK_LinkAccount_AsyncFunction* UEIK_LinkAccount_AsyncFunction::LinkAccount(const FString& LocalProductUserId,
                                                                            const FEIK_ContinuanceToken& ContinuanceToken)
{
	UEIK_LinkAccount_AsyncFunction* BlueprintNode = NewObject<UEIK_LinkAccount_AsyncFunction>();
	BlueprintNode->Var_LocalProductUserId = LocalProductUserId;
	BlueprintNode->Var_ContinuanceToken = ContinuanceToken;
	return BlueprintNode;
}

void UEIK_LinkAccount_AsyncFunction::OnLinkAccountCallback(const EOS_Connect_LinkAccountCallbackInfo* Data)
{
	TEnumAsByte<EEIK_Result> Result = static_cast<EEIK_Result>(Data->ResultCode);
	UEIK_LinkAccount_AsyncFunction* LinkAccountFunction = static_cast<UEIK_LinkAccount_AsyncFunction*>(Data->ClientData);
	if(LinkAccountFunction)
	{
		AsyncTask(ENamedThreads::GameThread, [LinkAccountFunction, Data]()
		{
			LinkAccountFunction->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
		});
		LinkAccountFunction->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		LinkAccountFunction->MarkAsGarbage();
#else
		LinkAccountFunction->MarkPendingKill();
#endif
	}
}

void UEIK_LinkAccount_AsyncFunction::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_LinkAccountOptions LinkAccountOptions = { };
			LinkAccountOptions.ApiVersion = EOS_CONNECT_LINKACCOUNT_API_LATEST;
			LinkAccountOptions.LocalUserId = EOS_ProductUserId_FromString(TCHAR_TO_ANSI(*Var_LocalProductUserId));
			LinkAccountOptions.ContinuanceToken = Var_ContinuanceToken.GetValueAsEosType();
			EOS_Connect_LinkAccount(EOSRef->ConnectHandle, &LinkAccountOptions, this, &UEIK_LinkAccount_AsyncFunction::OnLinkAccountCallback);
			return;
		}
	}
	OnCallback.Broadcast(EEIK_Result::EOS_ServiceFailure);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
