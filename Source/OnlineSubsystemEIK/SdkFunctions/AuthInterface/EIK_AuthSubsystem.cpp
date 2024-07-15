// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_AuthSubsystem.h"

#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

FEIK_NotificationId UEIK_AuthSubsystem::EIK_Auth_AddNotifyLoginStatusChanged(
	const FEIK_Auth_OnLoginStatusChangedCallback& Callback)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_AddNotifyLoginStatusChangedOptions Options = {};
			Options.ApiVersion = EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST;
			auto Return = EOS_Auth_AddNotifyLoginStatusChanged(EOSRef->AuthHandle, &Options, this, [](const EOS_Auth_LoginStatusChangedCallbackInfo* Data)
			{
				UEIK_AuthSubsystem* Subsystem = static_cast<UEIK_AuthSubsystem*>(Data->ClientData);
				if (Subsystem)
				{
					Subsystem->OnLoginStatusChanged.ExecuteIfBound(Data->LocalUserId, static_cast<EIK_ELoginStatus>(Data->PrevStatus), static_cast<EIK_ELoginStatus>(Data->CurrentStatus));
				}
			});
			return Return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

TEnumAsByte<EEIK_Result> UEIK_AuthSubsystem::EIK_Auth_CopyIdToken(FEIK_EpicAccountId AccountId,
	FEIK_Auth_IdToken& OutToken)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_CopyIdTokenOptions Options = {};
			Options.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
			Options.AccountId = AccountId.GetValueAsEosType();
			EOS_Auth_IdToken* Token = nullptr;
			auto Return = EOS_Auth_CopyIdToken(EOSRef->AuthHandle, &Options, &Token);
			if (Return == EOS_EResult::EOS_Success)
			{
				OutToken = *Token;
			}
			return static_cast<EEIK_Result>(Return);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_AuthSubsystem::EIK_Auth_CopyUserAuthToken(FEIK_EpicAccountId LocalUserId,
	FEIK_Auth_Token& OutToken)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_CopyUserAuthTokenOptions Options = {};
			Options.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;
			EOS_Auth_Token* Token = nullptr;
			auto Return = EOS_Auth_CopyUserAuthToken(EOSRef->AuthHandle, &Options, LocalUserId.GetValueAsEosType(), &Token);
			if (Return == EOS_EResult::EOS_Success)
			{
				OutToken = *Token;
			}
			return static_cast<EEIK_Result>(Return);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

FEIK_EpicAccountId UEIK_AuthSubsystem::EIK_Auth_GetLoggedInAccountByIndex(int32 Index)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return EOS_Auth_GetLoggedInAccountByIndex(EOSRef->AuthHandle, Index);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return FEIK_EpicAccountId();
}

int32 UEIK_AuthSubsystem::EIK_Auth_GetLoggedInAccountsCount()
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return EOS_Auth_GetLoggedInAccountsCount(EOSRef->AuthHandle);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return 0;
}

TEnumAsByte<EIK_ELoginStatus> UEIK_AuthSubsystem::EIK_Auth_GetLoginStatus(FEIK_EpicAccountId LocalUserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return static_cast<EIK_ELoginStatus>(EOS_Auth_GetLoginStatus(EOSRef->AuthHandle, LocalUserId.GetValueAsEosType()));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EIK_ELoginStatus::EIK_LS_NotLoggedIn;
}

FEIK_EpicAccountId UEIK_AuthSubsystem::EIK_Auth_GetMergedAccountByIndex(FEIK_EpicAccountId LocalUserId,
	int32 Index)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return EOS_Auth_GetMergedAccountByIndex(EOSRef->AuthHandle, LocalUserId.GetValueAsEosType(), Index);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return FEIK_EpicAccountId();
}

int32 UEIK_AuthSubsystem::EIK_Auth_GetMergedAccountsCount(FEIK_EpicAccountId LocalUserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return EOS_Auth_GetMergedAccountsCount(EOSRef->AuthHandle, LocalUserId.GetValueAsEosType());
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return 0;
}

TEnumAsByte<EEIK_Result> UEIK_AuthSubsystem::EIK_Auth_GetSelectedAccountId(FEIK_EpicAccountId LocalUserId,
	FEIK_EpicAccountId& OutSelectedAccountId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_EpicAccountId OutSelectedAccountIdPtr = nullptr;
			auto Result = static_cast<EEIK_Result>(EOS_Auth_GetSelectedAccountId(EOSRef->AuthHandle, LocalUserId.GetValueAsEosType(), &OutSelectedAccountIdPtr));
			if (Result == EEIK_Result::EOS_Success)
			{
				OutSelectedAccountId = OutSelectedAccountIdPtr;
			}
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_AuthSubsystem::EIK_Auth_IdToken_Release(FEIK_Auth_IdToken& Token)
{
	EOS_Auth_IdToken ReleaseToken = Token.GetValueAsEosType();
	EOS_Auth_IdToken_Release(&ReleaseToken);
}

void UEIK_AuthSubsystem::EIK_Auth_RemoveNotifyLoginStatusChanged(FEIK_NotificationId NotificationId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_RemoveNotifyLoginStatusChanged(EOSRef->AuthHandle, NotificationId.GetValueAsEosType());
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
}

void UEIK_AuthSubsystem::EIK_Auth_Token_Release(FEIK_Auth_Token& Token)
{
	EOS_Auth_Token ReleaseToken = Token.GetValueAsEosType();
	EOS_Auth_Token_Release(&ReleaseToken);
}