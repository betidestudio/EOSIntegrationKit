// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_UserInfo_QueryUserInfo.h"

#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_UserInfo_QueryUserInfo* UEIK_UserInfo_QueryUserInfo::EIK_UserInfo_QueryUserInfo(
	const FEIK_EpicAccountId& LocalUserId, const FEIK_EpicAccountId& TargetUserId)
{
	UEIK_UserInfo_QueryUserInfo* Node = NewObject<UEIK_UserInfo_QueryUserInfo>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_UserInfo_QueryUserInfo::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UserInfo_QueryUserInfoOptions Options;
			Options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			EOS_UserInfo_QueryUserInfo(EOSRef->UserInfoHandle, &Options, this, &UEIK_UserInfo_QueryUserInfo::OnQueryUserInfoCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UserInfo_QueryUserInfo::Activate: Unable to get EOS subsystem."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, Var_LocalUserId, Var_TargetUserId);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_UserInfo_QueryUserInfo::OnQueryUserInfoCallback(const EOS_UserInfo_QueryUserInfoCallbackInfo* Data)
{
	if(UEIK_UserInfo_QueryUserInfo* Node = static_cast<UEIK_UserInfo_QueryUserInfo*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Node->Var_LocalUserId, Node->Var_TargetUserId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}