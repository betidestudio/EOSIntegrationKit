// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_UserInfo_QueryUserInfoByExternalAccount.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_UserInfo_QueryUserInfoByExternalAccount* UEIK_UserInfo_QueryUserInfoByExternalAccount::
EIK_UserInfo_QueryUserInfoByExternalAccount(const FEIK_EpicAccountId& LocalUserId,
	const TEnumAsByte<EEIK_EExternalAccountType>& AccountType, const FString& ExternalAccountId)
{
	UEIK_UserInfo_QueryUserInfoByExternalAccount* Node = NewObject<UEIK_UserInfo_QueryUserInfoByExternalAccount>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_AccountType = AccountType;
	Node->Var_ExternalAccountId = ExternalAccountId;
	return Node;
}

void UEIK_UserInfo_QueryUserInfoByExternalAccount::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UserInfo_QueryUserInfoByExternalAccountOptions Options;
			Options.ApiVersion = EOS_USERINFO_QUERYUSERINFOBYEXTERNALACCOUNT_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.AccountType = static_cast<EOS_EExternalAccountType>(Var_AccountType.GetValue());
			Options.ExternalAccountId = TCHAR_TO_ANSI(*Var_ExternalAccountId);
			EOS_UserInfo_QueryUserInfoByExternalAccount(EOSRef->UserInfoHandle, &Options, this,&UEIK_UserInfo_QueryUserInfoByExternalAccount::OnQueryUserInfoByExternalAccountCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UserInfo_QueryUserInfoByExternalAccount::Activate: Unable to get EOS subsystem."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, Var_LocalUserId, FEIK_EpicAccountId(), Var_AccountType, Var_ExternalAccountId);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_UserInfo_QueryUserInfoByExternalAccount::OnQueryUserInfoByExternalAccountCallback(
	const EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo* Data)
{
	if (UEIK_UserInfo_QueryUserInfoByExternalAccount* Node = static_cast<UEIK_UserInfo_QueryUserInfoByExternalAccount*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->TargetUserId, static_cast<EEIK_EExternalAccountType>(Data->AccountType), Data->ExternalAccountId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
