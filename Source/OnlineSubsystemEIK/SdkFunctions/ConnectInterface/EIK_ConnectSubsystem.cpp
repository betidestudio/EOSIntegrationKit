// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_ConnectSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOS.h"

//LogEIK

FEIK_NotificationId UEIK_ConnectSubsystem::EIK_Connect_AddNotifyAuthExpiration(const FOnAuthExpirationCallback& Callback)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_AddNotifyAuthExpirationOptions AddNotifyAuthExpirationOptions = { };
			AddNotifyAuthExpirationOptions.ApiVersion = EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST;
			auto Return = EOS_Connect_AddNotifyAuthExpiration(EOSRef->ConnectHandle, &AddNotifyAuthExpirationOptions, this, [] (const EOS_Connect_AuthExpirationCallbackInfo* Data)
			{
				UEIK_ConnectSubsystem* ConnectSubsystem = static_cast<UEIK_ConnectSubsystem*>(Data->ClientData);
				if(ConnectSubsystem)
				{
					ConnectSubsystem->OnAuthExpiration.ExecuteIfBound(Data->LocalUserId);
				}
			});
			return Return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to add notify auth expiration either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_ConnectSubsystem::EIK_Connect_AddNotifyLoginStatusChanged(const FOnLoginStatusChangedCallback& Callback)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_AddNotifyLoginStatusChangedOptions AddNotifyLoginStatusChangedOptions = { };
			AddNotifyLoginStatusChangedOptions.ApiVersion = EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST;
			auto Return = EOS_Connect_AddNotifyLoginStatusChanged(EOSRef->ConnectHandle, &AddNotifyLoginStatusChangedOptions, this, [] (const EOS_Connect_LoginStatusChangedCallbackInfo* Data)
			{
				UEIK_ConnectSubsystem* ConnectSubsystem = static_cast<UEIK_ConnectSubsystem*>(Data->ClientData);
				if(ConnectSubsystem)
				{
					ConnectSubsystem->OnLoginStatusChanged.ExecuteIfBound(Data->LocalUserId, static_cast<EIK_ELoginStatus>(Data->CurrentStatus));
				}
			});
			return Return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to add notify login status changed either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_NotificationId();
}

TEnumAsByte<EEIK_Result> UEIK_ConnectSubsystem::EIK_Connect_CopyIdToken(FEIK_ProductUserId LocalUserId,
	FEIK_Connect_IdToken& OutIdToken)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_IdToken* IdToken = nullptr;
			EOS_Connect_CopyIdTokenOptions CopyIdTokenOptions = { };
			CopyIdTokenOptions.ApiVersion = EOS_CONNECT_COPYIDTOKEN_API_LATEST;
			CopyIdTokenOptions.LocalUserId = LocalUserId.GetValueAsEosType();
			auto ResultVal = EOS_Connect_CopyIdToken(EOSRef->ConnectHandle, &CopyIdTokenOptions, &IdToken);
			if(IdToken)
			{
				OutIdToken = *IdToken;
			}
			return static_cast<EEIK_Result>(ResultVal);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to copy id token either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_ConnectSubsystem::EIK_Connect_CopyProductUserExternalAccountByAccountId(
	FEIK_ProductUserId LocalUserId, FString AccountId, FEIK_Connect_ExternalAccountInfo& OutExternalAccountInfo)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_ExternalAccountInfo* ExternalAccountInfo = nullptr;
			EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions CopyProductUserExternalAccountByAccountIdOptions = { };
			CopyProductUserExternalAccountByAccountIdOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTID_API_LATEST;
			CopyProductUserExternalAccountByAccountIdOptions.TargetUserId = LocalUserId.GetValueAsEosType();
			CopyProductUserExternalAccountByAccountIdOptions.AccountId = TCHAR_TO_ANSI(*AccountId);
			auto ResultVal = EOS_Connect_CopyProductUserExternalAccountByAccountId(EOSRef->ConnectHandle, &CopyProductUserExternalAccountByAccountIdOptions, &ExternalAccountInfo);
			if(ExternalAccountInfo)
			{
				OutExternalAccountInfo = *ExternalAccountInfo;
			}
			return static_cast<EEIK_Result>(ResultVal);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to copy product user external account by account id either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_ConnectSubsystem::EIK_Connect_CopyProductUserExternalAccountByAccountType(
	FEIK_ProductUserId LocalUserId, TEnumAsByte<EEIK_EExternalAccountType> AccountType,
	FEIK_Connect_ExternalAccountInfo& OutExternalAccountInfo)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_ExternalAccountInfo* ExternalAccountInfo = nullptr;
			EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions CopyProductUserExternalAccountByAccountTypeOptions = { };
			CopyProductUserExternalAccountByAccountTypeOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTTYPE_API_LATEST;
			CopyProductUserExternalAccountByAccountTypeOptions.TargetUserId = LocalUserId.GetValueAsEosType();
			CopyProductUserExternalAccountByAccountTypeOptions.AccountIdType = static_cast<EOS_EExternalAccountType>(AccountType.GetValue());
			auto ResultVal = EOS_Connect_CopyProductUserExternalAccountByAccountType(EOSRef->ConnectHandle, &CopyProductUserExternalAccountByAccountTypeOptions, &ExternalAccountInfo);
			if(ExternalAccountInfo)
			{
				OutExternalAccountInfo = *ExternalAccountInfo;
			}
			return static_cast<EEIK_Result>(ResultVal);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to copy product user external account by account type either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_ConnectSubsystem::EIK_Connect_CopyProductUserExternalAccountByIndex(
	FEIK_ProductUserId LocalUserId, int32 Index, FEIK_Connect_ExternalAccountInfo& OutExternalAccountInfo)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_ExternalAccountInfo* ExternalAccountInfo = nullptr;
			EOS_Connect_CopyProductUserExternalAccountByIndexOptions CopyProductUserExternalAccountByIndexOptions = { };
			CopyProductUserExternalAccountByIndexOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST;
			CopyProductUserExternalAccountByIndexOptions.TargetUserId = LocalUserId.GetValueAsEosType();
			CopyProductUserExternalAccountByIndexOptions.ExternalAccountInfoIndex = Index;
			auto ResultVal = EOS_Connect_CopyProductUserExternalAccountByIndex(EOSRef->ConnectHandle, &CopyProductUserExternalAccountByIndexOptions, &ExternalAccountInfo);
			if(ExternalAccountInfo)
			{
				OutExternalAccountInfo = *ExternalAccountInfo;
			}
			return static_cast<EEIK_Result>(ResultVal);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to copy product user external account by index either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_ConnectSubsystem::EIK_Connect_CopyProductUserInfo(FEIK_ProductUserId LocalUserId,
	FEIK_Connect_ExternalAccountInfo& OutProductUserInfo)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_ExternalAccountInfo* ExternalAccountInfo = nullptr;
			EOS_Connect_CopyProductUserInfoOptions CopyProductUserInfoOptions = { };
			CopyProductUserInfoOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSERINFO_API_LATEST;
			CopyProductUserInfoOptions.TargetUserId = LocalUserId.GetValueAsEosType();
			auto ResultVal = EOS_Connect_CopyProductUserInfo(EOSRef->ConnectHandle, &CopyProductUserInfoOptions, &ExternalAccountInfo);
			if(ExternalAccountInfo)
			{
				OutProductUserInfo = *ExternalAccountInfo;
			}
			return static_cast<EEIK_Result>(ResultVal);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to copy product user info either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_ConnectSubsystem::EIK_Connect_ExternalAccountInfo_Release(
	FEIK_Connect_ExternalAccountInfo ExternalAccountInfo)
{
	EOS_Connect_ExternalAccountInfo ReleaseExternalAccountInfo = ExternalAccountInfo.GetValueAsEosType();
	EOS_Connect_ExternalAccountInfo_Release(&ReleaseExternalAccountInfo);
}

FEIK_ProductUserId UEIK_ConnectSubsystem::EIK_Connect_GetExternalAccountMapping(FEIK_ProductUserId LocalUserId,	TEnumAsByte<EEIK_EExternalAccountType> AccountIdType, FString TargetExternalUserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_GetExternalAccountMappingsOptions GetExternalAccountMappingsOptions = { };
			GetExternalAccountMappingsOptions.ApiVersion = EOS_CONNECT_GETEXTERNALACCOUNTMAPPING_API_LATEST;
			GetExternalAccountMappingsOptions.LocalUserId = LocalUserId.GetValueAsEosType();
			GetExternalAccountMappingsOptions.AccountIdType = static_cast<EOS_EExternalAccountType>(AccountIdType.GetValue());
			if(!TargetExternalUserId.IsEmpty())
			{
				GetExternalAccountMappingsOptions.TargetExternalUserId = TCHAR_TO_ANSI(*TargetExternalUserId);
			}
			return EOS_Connect_GetExternalAccountMapping(EOSRef->ConnectHandle, &GetExternalAccountMappingsOptions);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get external account mapping either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_ProductUserId();
}

FEIK_ProductUserId UEIK_ConnectSubsystem::EIK_Connect_GetLoggedInUserByIndex(int32 Index)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return EOS_Connect_GetLoggedInUserByIndex(EOSRef->ConnectHandle, Index);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get logged in user by index either OnlineSubsystem is not valid or EOSRef is not valid."));
	return FEIK_ProductUserId();
}

int32 UEIK_ConnectSubsystem::EIK_Connect_GetLoggedInUsersCount()
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return EOS_Connect_GetLoggedInUsersCount(EOSRef->ConnectHandle);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get logged in users count either OnlineSubsystem is not valid or EOSRef is not valid."));
	return 0;
}

TEnumAsByte<EEIK_LoginStatus> UEIK_ConnectSubsystem::EIK_Connect_GetLoginStatus(FEIK_ProductUserId LocalUserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			return static_cast<EEIK_LoginStatus>(EOS_Connect_GetLoginStatus(EOSRef->ConnectHandle, LocalUserId.GetValueAsEosType()));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get login status either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_LoginStatus::NotLoggedIn;
}

int32 UEIK_ConnectSubsystem::EIK_Connect_GetProductUserExternalAccountCount(FEIK_ProductUserId LocalUserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_GetProductUserExternalAccountCountOptions GetProductUserExternalAccountCountOptions = { };
			GetProductUserExternalAccountCountOptions.ApiVersion = EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST;
			GetProductUserExternalAccountCountOptions.TargetUserId = LocalUserId.GetValueAsEosType();
			return EOS_Connect_GetProductUserExternalAccountCount(EOSRef->ConnectHandle, &GetProductUserExternalAccountCountOptions);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get product user external account count either OnlineSubsystem is not valid or EOSRef is not valid."));
	return -1;
}

TEnumAsByte<EEIK_Result> UEIK_ConnectSubsystem::EIK_Connect_GetProductUserIdMapping(FEIK_ProductUserId LocalUserId,
	TEnumAsByte<EEIK_EExternalAccountType> AccountIdType, FEIK_ProductUserId TargetUserId, FString& OutBuffer)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_GetProductUserIdMappingOptions GetProductUserIdMappingOptions = { };
			GetProductUserIdMappingOptions.ApiVersion = EOS_CONNECT_GETPRODUCTUSERIDMAPPING_API_LATEST;
			GetProductUserIdMappingOptions.LocalUserId = LocalUserId.GetValueAsEosType();
			GetProductUserIdMappingOptions.AccountIdType = static_cast<EOS_EExternalAccountType>(AccountIdType.GetValue());
			GetProductUserIdMappingOptions.TargetProductUserId = TargetUserId.GetValueAsEosType();
			char Buffer[EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH + 1];
			int32 BufferSize = EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH + 1;
			auto ResultVal = EOS_Connect_GetProductUserIdMapping(EOSRef->ConnectHandle, &GetProductUserIdMappingOptions, Buffer, &BufferSize);
			OutBuffer = Buffer;
			return static_cast<EEIK_Result>(ResultVal);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get product user id mapping either OnlineSubsystem is not valid or EOSRef is not valid."));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_ConnectSubsystem::EIK_Connect_IdToken_Release(FEIK_Connect_IdToken IdToken)
{
	EOS_Connect_IdToken ReleaseIdToken = IdToken.GetValueAsEosType();
	EOS_Connect_IdToken_Release(&ReleaseIdToken);
}

void UEIK_ConnectSubsystem::EIK_Connect_RemoveNotifyAuthExpiration(FEIK_NotificationId InId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_RemoveNotifyAuthExpiration(EOSRef->ConnectHandle, InId.NotificationId);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to remove notify auth expiration either OnlineSubsystem is not valid or EOSRef is not valid."));
}

void UEIK_ConnectSubsystem::EIK_Connect_RemoveNotifyLoginStatusChanged(FEIK_NotificationId InId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_RemoveNotifyLoginStatusChanged(EOSRef->ConnectHandle, InId.NotificationId);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to remove notify login status changed either OnlineSubsystem is not valid or EOSRef is not valid."));
}
