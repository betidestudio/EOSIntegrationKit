// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_UserInfo_QueryUserInfoByDisplayName.h"

#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_UserInfo_QueryUserInfoByDisplayName* UEIK_UserInfo_QueryUserInfoByDisplayName::
EIK_UserInfo_QueryUserInfoByDisplayName(const FEIK_EpicAccountId& LocalUserId, const FString& DisplayName)
{
	UEIK_UserInfo_QueryUserInfoByDisplayName* Node = NewObject<UEIK_UserInfo_QueryUserInfoByDisplayName>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_DisplayName = DisplayName;
	return Node;
}

void UEIK_UserInfo_QueryUserInfoByDisplayName::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UserInfo_QueryUserInfoByDisplayNameOptions Options;
			Options.ApiVersion = EOS_USERINFO_QUERYUSERINFOBYDISPLAYNAME_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.DisplayName = TCHAR_TO_ANSI(*Var_DisplayName);
			EOS_UserInfo_QueryUserInfoByDisplayName(EOSRef->UserInfoHandle, &Options, this, &UEIK_UserInfo_QueryUserInfoByDisplayName::OnQueryUserInfoByDisplayNameCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UserInfo_QueryUserInfoByDisplayName::Activate: Unable to get EOS subsystem."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, Var_LocalUserId, FEIK_EpicAccountId(), Var_DisplayName);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_UserInfo_QueryUserInfoByDisplayName::OnQueryUserInfoByDisplayNameCallback(
	const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data)
{
	if (UEIK_UserInfo_QueryUserInfoByDisplayName* Node = static_cast<UEIK_UserInfo_QueryUserInfoByDisplayName*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->TargetUserId, Data->DisplayName);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
