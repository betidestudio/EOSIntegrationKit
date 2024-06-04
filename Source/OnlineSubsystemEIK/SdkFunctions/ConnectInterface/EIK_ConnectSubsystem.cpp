// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_ConnectSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

//LogEIK
DEFINE_LOG_CATEGORY(LogEIK);

void UEIK_ConnectSubsystem::EIK_Connect_AddNotifyAuthExpiration(const FOnAuthExpirationCallback& Callback)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_AddNotifyAuthExpirationOptions AddNotifyAuthExpirationOptions = { };
			AddNotifyAuthExpirationOptions.ApiVersion = EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST;
			EOS_Connect_AddNotifyAuthExpiration(EOSRef->ConnectHandle, &AddNotifyAuthExpirationOptions, this, [] (const EOS_Connect_AuthExpirationCallbackInfo* Data)
			{
				UEIK_ConnectSubsystem* ConnectSubsystem = static_cast<UEIK_ConnectSubsystem*>(Data->ClientData);
				if(ConnectSubsystem)
				{
					ConnectSubsystem->OnAuthExpiration.ExecuteIfBound(Data->LocalUserId);
				}
			});
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to add notify auth expiration either OnlineSubsystem is not valid or EOSRef is not valid."));
}

void UEIK_ConnectSubsystem::EIK_Connect_AddNotifyLoginStatusChanged(const FOnLoginStatusChangedCallback& Callback)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_AddNotifyLoginStatusChangedOptions AddNotifyLoginStatusChangedOptions = { };
			AddNotifyLoginStatusChangedOptions.ApiVersion = EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST;
			EOS_Connect_AddNotifyLoginStatusChanged(EOSRef->ConnectHandle, &AddNotifyLoginStatusChangedOptions, this, [] (const EOS_Connect_LoginStatusChangedCallbackInfo* Data)
			{
				UEIK_ConnectSubsystem* ConnectSubsystem = static_cast<UEIK_ConnectSubsystem*>(Data->ClientData);
				if(ConnectSubsystem)
				{
					ConnectSubsystem->OnLoginStatusChanged.ExecuteIfBound(Data->LocalUserId, static_cast<EIK_ELoginStatus>(Data->CurrentStatus));
				}
			});
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to add notify login status changed either OnlineSubsystem is not valid or EOSRef is not valid."));
}

void UEIK_ConnectSubsystem::EIK_Connect_CopyIdToken(FEIK_ProductUserId LocalUserId, TEnumAsByte<EEIK_Result>& Result,
	FString& OutIdToken)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_IdToken* IdToken = nullptr;
			EOS_Connect_CopyIdTokenOptions CopyIdTokenOptions = { };
			CopyIdTokenOptions.ApiVersion = EOS_CONNECT_COPYIDTOKEN_API_LATEST;
			CopyIdTokenOptions.LocalUserId = LocalUserId.ProductUserId_FromString();
			auto ResultVal = EOS_Connect_CopyIdToken(EOSRef->ConnectHandle, &CopyIdTokenOptions, &IdToken);
			Result = static_cast<EEIK_Result>(ResultVal);
			if(IdToken)
			{
				OutIdToken = IdToken->JsonWebToken;
			}
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to copy id token either OnlineSubsystem is not valid or EOSRef is not valid."));
}