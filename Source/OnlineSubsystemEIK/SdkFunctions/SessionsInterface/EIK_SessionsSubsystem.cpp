// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_SessionsSubsystem.h"

#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_ActiveSession_CopyInfo(FEIK_HActiveSession Handle,
                                                                            FEIK_ActiveSession_Info& OutActiveSessionInfo)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_ActiveSession_CopyInfoOptions Options;
			Options.ApiVersion = EOS_ACTIVESESSION_COPYINFO_API_LATEST;
			EOS_ActiveSession_Info* Result = nullptr;
			auto ReturnResult = EOS_ActiveSession_CopyInfo(Handle.Ref, &Options, &Result);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutActiveSessionInfo = *Result;
				EOS_ActiveSession_Info_Release(Result);
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_ActiveSession_CopyInfo: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

FEIK_ProductUserId UEIK_SessionsSubsystem::EIK_ActiveSession_GetRegisteredPlayerByIndex(FEIK_HActiveSession Handle,
	int32 PlayerIndex)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return FEIK_ProductUserId{ nullptr };
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_ActiveSession_GetRegisteredPlayerByIndexOptions Options;
			Options.ApiVersion = EOS_ACTIVESESSION_GETREGISTEREDPLAYERBYINDEX_API_LATEST;
			Options.PlayerIndex = PlayerIndex;
			return  EOS_ActiveSession_GetRegisteredPlayerByIndex(Handle.Ref, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_ActiveSession_GetRegisteredPlayerByIndex: Unable to get EOS subsystem."));
	return FEIK_ProductUserId{ nullptr };
}

void UEIK_SessionsSubsystem::EIK_ActiveSession_Release(FEIK_HActiveSession Handle)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return;
	}
	EOS_ActiveSession_Release(Handle.Ref);
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionDetails_CopyInfo(FEIK_HSessionDetails Handle,
	FEIK_SessionDetails_Info& OutSessionInfo)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionDetails_CopyInfoOptions Options;
			Options.ApiVersion = EOS_SESSIONDETAILS_COPYINFO_API_LATEST;
			EOS_SessionDetails_Info* Result = nullptr;
			auto ReturnResult = EOS_SessionDetails_CopyInfo(Handle.Ref, &Options, &Result);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionInfo = *Result;
				EOS_SessionDetails_Info_Release(Result);
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionDetails_CopyInfo: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionDetails_CopySessionAttributeByIndex(
	FEIK_HSessionDetails Handle, int32 AttrIndex, FEIK_SessionDetails_Attribute& OutSessionAttribute)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionDetails_CopySessionAttributeByIndexOptions Options;
			Options.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYINDEX_API_LATEST;
			Options.AttrIndex = AttrIndex;
			EOS_SessionDetails_Attribute* Result = nullptr;
			auto ReturnResult = EOS_SessionDetails_CopySessionAttributeByIndex(Handle.Ref, &Options, &Result);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionAttribute = *Result;
				EOS_SessionDetails_Attribute_Release(Result);
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionDetails_CopySessionAttributeByIndex: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionDetails_CopySessionAttributeByKey(
	FEIK_HSessionDetails Handle, const FString& AttrKey, FEIK_SessionDetails_Attribute& OutSessionAttribute)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionDetails_CopySessionAttributeByKeyOptions Options;
			Options.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYKEY_API_LATEST;
			Options.AttrKey = TCHAR_TO_ANSI(*AttrKey);
			EOS_SessionDetails_Attribute* Result = nullptr;
			auto ReturnResult = EOS_SessionDetails_CopySessionAttributeByKey(Handle.Ref, &Options, &Result);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionAttribute = *Result;
				EOS_SessionDetails_Attribute_Release(Result);
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionDetails_CopySessionAttributeByKey: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

int32 UEIK_SessionsSubsystem::EIK_SessionDetails_GetSessionAttributeCount(FEIK_HSessionDetails Handle)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionDetails_GetSessionAttributeCountOptions Options;
			Options.ApiVersion = EOS_SESSIONDETAILS_GETSESSIONATTRIBUTECOUNT_API_LATEST;
			return EOS_SessionDetails_GetSessionAttributeCount(Handle.Ref, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionDetails_GetSessionAttributeCount: Unable to get EOS subsystem."));
	return -1;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_AddAttribute(FEIK_HSessionModification Handle,
	FEIK_Sessions_AttributeData AttrData, TEnumAsByte<EIK_ESessionAttributeAdvertisementType> AdvertisementType)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_AddAttributeOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_ADDATTRIBUTE_API_LATEST;
			Options.AdvertisementType = static_cast<EOS_ESessionAttributeAdvertisementType>(AdvertisementType.GetValue());
			EOS_Sessions_AttributeData LocalTemp = AttrData.GetValueAsEosType();
			Options.SessionAttribute = &LocalTemp;
			return static_cast<EEIK_Result>(EOS_SessionModification_AddAttribute(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_AddAttribute: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_SessionsSubsystem::EIK_SessionModification_Release(FEIK_HSessionModification Handle)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return;
	}
	EOS_SessionModification_Release(Handle.Ref);
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_RemoveAttribute(
	FEIK_HSessionModification Handle, const FString& Key)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_RemoveAttributeOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_REMOVEATTRIBUTE_API_LATEST;
			Options.Key = TCHAR_TO_ANSI(*Key);
			return static_cast<EEIK_Result>(EOS_SessionModification_RemoveAttribute(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_RemoveAttribute: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_SetAllowedPlatformIds(
	FEIK_HSessionModification Handle, const TArray<int32>& PlatformIds)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_SetAllowedPlatformIdsOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_SETALLOWEDPLATFORMIDS_API_LATEST;
			Options.AllowedPlatformIdsCount = PlatformIds.Num();
			uint32_t* PlatformIdsData = new uint32_t[PlatformIds.Num()];
			for (int i = 0; i < PlatformIds.Num(); i++)
			{
				PlatformIdsData[i] = PlatformIds[i];
			}
			Options.AllowedPlatformIds = PlatformIdsData;
			return static_cast<EEIK_Result>(EOS_SessionModification_SetAllowedPlatformIds(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_SetAllowedPlatformIds: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_SetBucketId(FEIK_HSessionModification Handle,
	const FString& BucketId)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_SetBucketIdOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_SETBUCKETID_API_LATEST;
			Options.BucketId = TCHAR_TO_ANSI(*BucketId);
			return static_cast<EEIK_Result>(EOS_SessionModification_SetBucketId(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_SetBucketId: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_SetHostAddress(
	FEIK_HSessionModification Handle, const FString& HostAddress)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_SetHostAddressOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_SETHOSTADDRESS_API_LATEST;
			Options.HostAddress = TCHAR_TO_ANSI(*HostAddress);
			return static_cast<EEIK_Result>(EOS_SessionModification_SetHostAddress(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_SetHostAddress: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_SetInvitesAllowed(
	FEIK_HSessionModification Handle, bool bInvitesAllowed)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_SetInvitesAllowedOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_SETINVITESALLOWED_API_LATEST;
			Options.bInvitesAllowed = bInvitesAllowed ? EOS_TRUE : EOS_FALSE;
			return static_cast<EEIK_Result>(EOS_SessionModification_SetInvitesAllowed(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_SetInvitesAllowed: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_SetJoinInProgressAllowed(
	FEIK_HSessionModification Handle, bool bAllowJoinInProgress)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_SetJoinInProgressAllowedOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_SETJOININPROGRESSALLOWED_API_LATEST;
			Options.bAllowJoinInProgress = bAllowJoinInProgress ? EOS_TRUE : EOS_FALSE;
			return static_cast<EEIK_Result>(EOS_SessionModification_SetJoinInProgressAllowed(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_SetJoinInProgressAllowed: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_SetMaxPlayers(FEIK_HSessionModification Handle,
	int32 MaxPlayers)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_SetMaxPlayersOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_SETMAXPLAYERS_API_LATEST;
			Options.MaxPlayers = MaxPlayers;
			return static_cast<EEIK_Result>(EOS_SessionModification_SetMaxPlayers(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_SetMaxPlayers: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionModification_SetPermissionLevel(
	FEIK_HSessionModification Handle, TEnumAsByte<EEIK_EOnlineSessionPermissionLevel> PermissionLevel)
{
	if(Handle.Ref == nullptr)
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem: Invalid Handle."));
		return EEIK_Result::EOS_NotFound;
	}
	EOS_EOnlineSessionPermissionLevel LocalPermissionLevel = static_cast<EOS_EOnlineSessionPermissionLevel>(PermissionLevel.GetValue());
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_SessionModification_SetPermissionLevelOptions Options;
			Options.ApiVersion = EOS_SESSIONMODIFICATION_SETPERMISSIONLEVEL_API_LATEST;
			Options.PermissionLevel = LocalPermissionLevel;
			return static_cast<EEIK_Result>(EOS_SessionModification_SetPermissionLevel(Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionModification_SetPermissionLevel: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

FEIK_NotificationId UEIK_SessionsSubsystem::EIK_Sessions_AddNotifyJoinSessionAccepted(
	const FEIK_Sessions_OnJoinSessionAcceptedCallback& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_AddNotifyJoinSessionAcceptedOptions Options;
			Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYJOINSESSIONACCEPTED_API_LATEST;
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifyJoinSessionAccepted: Invalid Sessions Handle."));
				return FEIK_NotificationId();
			}
			return EOS_Sessions_AddNotifyJoinSessionAccepted(EOSRef->SessionsHandle, &Options, this, [](const EOS_Sessions_JoinSessionAcceptedCallbackInfo* Data)
			{
				if(UEIK_SessionsSubsystem* Subsystem = static_cast<UEIK_SessionsSubsystem*>(Data->ClientData))
				{
					Subsystem->OnJoinSessionAcceptedCallback.ExecuteIfBound(Data->LocalUserId, Data->UiEventId);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifyJoinSessionAccepted: Invalid Callback."));
	return FEIK_NotificationId();
}
FEIK_NotificationId UEIK_SessionsSubsystem::EIK_Sessions_AddNotifyLeaveSessionRequested(
	const FEIK_Sessions_OnLeaveSessionRequestedCallback& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_AddNotifyLeaveSessionRequestedOptions Options;
			Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYLEAVESESSIONREQUESTED_API_LATEST;
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifyLeaveSessionRequested: Invalid Sessions Handle."));
				return FEIK_NotificationId();
			}
			return EOS_Sessions_AddNotifyLeaveSessionRequested(EOSRef->SessionsHandle, &Options, this, [](const EOS_Sessions_LeaveSessionRequestedCallbackInfo* Data)
			{
				if(UEIK_SessionsSubsystem* Subsystem = static_cast<UEIK_SessionsSubsystem*>(Data->ClientData))
				{
					Subsystem->OnLeaveSessionRequestedCallback.ExecuteIfBound(Data->LocalUserId, Data->SessionName);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifyLeaveSessionRequested: Invalid Callback."));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySendSessionNativeInviteRequested(
	const FEIK_Sessions_OnSendSessionInviteCallback& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_AddNotifySendSessionNativeInviteRequestedOptions Options;
			Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYSENDSESSIONNATIVEINVITEREQUESTED_API_LATEST;
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySendSessionNativeInviteRequested: Invalid Sessions Handle."));
				return FEIK_NotificationId();
			}
			return EOS_Sessions_AddNotifySendSessionNativeInviteRequested(EOSRef->SessionsHandle, &Options, this, [](const EOS_Sessions_SendSessionNativeInviteRequestedCallbackInfo* Data)
			{
				if(UEIK_SessionsSubsystem* Subsystem = static_cast<UEIK_SessionsSubsystem*>(Data->ClientData))
				{
					Subsystem->OnSendSessionInviteCallback.ExecuteIfBound(Data->LocalUserId, Data->UiEventId, Data->TargetNativeAccountType, Data->TargetUserNativeAccountId, Data->SessionId);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySendSessionNativeInviteRequested: Invalid Callback."));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteAccepted(
	const FEIK_Sessions_OnSessionInviteAcceptedCallback& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_AddNotifySessionInviteAcceptedOptions Options;
			Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITEACCEPTED_API_LATEST;
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteAccepted: Invalid Sessions Handle."));
				return FEIK_NotificationId();
			}
			return EOS_Sessions_AddNotifySessionInviteAccepted(EOSRef->SessionsHandle, &Options, this, [](const EOS_Sessions_SessionInviteAcceptedCallbackInfo* Data)
			{
				if(UEIK_SessionsSubsystem* Subsystem = static_cast<UEIK_SessionsSubsystem*>(Data->ClientData))
				{
					Subsystem->OnSessionInviteAcceptedCallback.ExecuteIfBound(Data->LocalUserId, Data->SessionId,Data->TargetUserId, Data->InviteId);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteAccepted: Invalid Callback."));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteReceived(
	const FEIK_Sessions_OnSessionInviteReceivedCallback& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_AddNotifySessionInviteReceivedOptions Options;
			Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITERECEIVED_API_LATEST;
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteReceived: Invalid Sessions Handle."));
				return FEIK_NotificationId();
			}
			return EOS_Sessions_AddNotifySessionInviteReceived(EOSRef->SessionsHandle, &Options, this, [](const EOS_Sessions_SessionInviteReceivedCallbackInfo* Data)
			{
				if(UEIK_SessionsSubsystem* Subsystem = static_cast<UEIK_SessionsSubsystem*>(Data->ClientData))
				{
					Subsystem->OnSessionInviteReceivedCallback.ExecuteIfBound(Data->LocalUserId, Data->InviteId, Data->TargetUserId);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteReceived: Invalid Callback."));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteRejected(
	const FEIK_Sessions_OnSessionInviteRejectedCallback& Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_AddNotifySessionInviteRejectedOptions Options;
			Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITEREJECTED_API_LATEST;
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteRejected: Invalid Sessions Handle."));
				return FEIK_NotificationId();
			}
			return EOS_Sessions_AddNotifySessionInviteRejected(EOSRef->SessionsHandle, &Options, this, [](const EOS_Sessions_SessionInviteRejectedCallbackInfo* Data)
			{
				if(UEIK_SessionsSubsystem* Subsystem = static_cast<UEIK_SessionsSubsystem*>(Data->ClientData))
				{
					Subsystem->OnSessionInviteRejectedCallback.ExecuteIfBound(Data->LocalUserId, Data->SessionId, Data->TargetUserId, Data->InviteId);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_AddNotifySessionInviteRejected: Invalid Callback."));
	return FEIK_NotificationId();
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_CopyActiveSessionHandle(FString SessionName,
	FEIK_HActiveSession& OutActiveSessionHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopyActiveSessionHandle: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_CopyActiveSessionHandleOptions Options;
			Options.ApiVersion = EOS_SESSIONS_COPYACTIVESESSIONHANDLE_API_LATEST;
			Options.SessionName = TCHAR_TO_ANSI(*SessionName);
			EOS_HActiveSession LocalHandle = nullptr;
			auto ReturnResult = EOS_Sessions_CopyActiveSessionHandle(EOSRef->SessionsHandle, &Options, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutActiveSessionHandle = LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopyActiveSessionHandle: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleByInviteId(FString InviteId,
	FEIK_HSessionDetails& OutSessionHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleByInviteId: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_CopySessionHandleByInviteIdOptions Options;
			Options.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEBYINVITEID_API_LATEST;
			Options.InviteId = TCHAR_TO_ANSI(*InviteId);
			EOS_HSessionDetails LocalHandle = nullptr;
			auto ReturnResult = EOS_Sessions_CopySessionHandleByInviteId(EOSRef->SessionsHandle, &Options, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionHandle = LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleByInviteId: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleByUiEventId(FEIK_UI_EventId UiEventId,
	FEIK_HSessionDetails& OutSessionHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleByUiEventId: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_CopySessionHandleByUiEventIdOptions Options;
			Options.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEBYUIEVENTID_API_LATEST;
			Options.UiEventId = UiEventId.Ref;
			EOS_HSessionDetails LocalHandle = nullptr;
			auto ReturnResult = EOS_Sessions_CopySessionHandleByUiEventId(EOSRef->SessionsHandle, &Options, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionHandle = LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleByUiEventId: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleForPresence(
	FEIK_ProductUserId LocalUserId, FEIK_HSessionDetails& OutSessionHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleForPresence: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_CopySessionHandleForPresenceOptions Options;
			Options.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEFORPRESENCE_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			EOS_HSessionDetails LocalHandle = nullptr;
			auto ReturnResult = EOS_Sessions_CopySessionHandleForPresence(EOSRef->SessionsHandle, &Options, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionHandle = LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CopySessionHandleForPresence: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_CreateSessionModification(
	FEIK_Sessions_CreateSessionModificationOptions Options, FEIK_HSessionModification& OutSessionModificationHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CreateSessionModification: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_CreateSessionModificationOptions LocalOptions = Options.ToEosStruct();
			EOS_HSessionModification LocalHandle = nullptr;
			auto ReturnResult = EOS_Sessions_CreateSessionModification(EOSRef->SessionsHandle, &LocalOptions, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionModificationHandle = LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CreateSessionModification: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_CreateSessionSearch(int32 MaxSearchResults,
	FEIK_HSessionSearch& OutSessionSearchHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CreateSessionSearch: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_CreateSessionSearchOptions Options;
			Options.ApiVersion = EOS_SESSIONS_CREATESESSIONSEARCH_API_LATEST;
			Options.MaxSearchResults = MaxSearchResults;
			EOS_HSessionSearch LocalHandle = nullptr;
			auto ReturnResult = EOS_Sessions_CreateSessionSearch(EOSRef->SessionsHandle, &Options, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionSearchHandle = &LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_CreateSessionSearch: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_SessionsSubsystem::EIK_Sessions_DumpSessionState(FString SessionName)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_DumpSessionState: Invalid Sessions Handle."));
				return;
			}
			EOS_Sessions_DumpSessionStateOptions Options;
			Options.ApiVersion = EOS_SESSIONS_DUMPSESSIONSTATE_API_LATEST;
			Options.SessionName = TCHAR_TO_ANSI(*SessionName);
			EOS_Sessions_DumpSessionState(EOSRef->SessionsHandle, &Options);
			return;
		}
	}
}

int32 UEIK_SessionsSubsystem::EIK_Sessions_GetInviteCount(FEIK_ProductUserId LocalUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_GetInviteCount: Invalid Sessions Handle."));
				return -1;
			}
			EOS_Sessions_GetInviteCountOptions Options;
			Options.ApiVersion = EOS_SESSIONS_GETINVITECOUNT_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			return EOS_Sessions_GetInviteCount(EOSRef->SessionsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_GetInviteCount: Unable to get EOS subsystem."));
	return -1;
}

FString UEIK_SessionsSubsystem::EIK_Sessions_GetInviteIdByIndex(FEIK_ProductUserId LocalUserId, int32 Index)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_GetInviteIdByIndex: Invalid Sessions Handle."));
				return "";
			}
			EOS_Sessions_GetInviteIdByIndexOptions Options;
			Options.ApiVersion = EOS_SESSIONS_GETINVITEIDBYINDEX_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.Index = Index;
			char Buffer[EOS_SESSIONS_INVITEID_MAX_LENGTH];
			int32_t* InOutBufferLength = new int32_t(EOS_SESSIONS_INVITEID_MAX_LENGTH);
			EOS_Sessions_GetInviteIdByIndex(EOSRef->SessionsHandle, &Options, Buffer, InOutBufferLength);
			return Buffer;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_GetInviteIdByIndex: Unable to get EOS subsystem."));
	return "";
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_IsUserInSession(FEIK_ProductUserId TargetUserId,
	FString SessionName)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_IsUserInSession: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_IsUserInSessionOptions Options;
			Options.ApiVersion = EOS_SESSIONS_ISUSERINSESSION_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			Options.SessionName = TCHAR_TO_ANSI(*SessionName);
			return static_cast<EEIK_Result>(EOS_Sessions_IsUserInSession(EOSRef->SessionsHandle, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_IsUserInSession: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_SessionsSubsystem::EIK_Sessions_RemoveNotifyJoinSessionAccepted(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_RemoveNotifyJoinSessionAccepted(EOSRef->SessionsHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_SessionsSubsystem::EIK_Sessions_RemoveNotifyLeaveSessionRequested(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_RemoveNotifyLeaveSessionRequested(EOSRef->SessionsHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_SessionsSubsystem::EIK_Sessions_RemoveNotifySendSessionNativeInviteRequested(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_RemoveNotifySendSessionNativeInviteRequested(EOSRef->SessionsHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_SessionsSubsystem::EIK_Sessions_RemoveNotifySessionInviteAccepted(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_RemoveNotifySessionInviteAccepted(EOSRef->SessionsHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_SessionsSubsystem::EIK_Sessions_RemoveNotifySessionInviteReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_RemoveNotifySessionInviteReceived(EOSRef->SessionsHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_SessionsSubsystem::EIK_Sessions_RemoveNotifySessionInviteRejected(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_RemoveNotifySessionInviteRejected(EOSRef->SessionsHandle, InId.GetValueAsEosType());
		}
	}
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_Sessions_UpdateSessionModification(FString SessionName,
	FEIK_HSessionModification& OutSessionModificationHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->SessionsHandle == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_UpdateSessionModification: Invalid Sessions Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_Sessions_UpdateSessionModificationOptions Options;
			Options.ApiVersion = EOS_SESSIONS_UPDATESESSIONMODIFICATION_API_LATEST;
			Options.SessionName = TCHAR_TO_ANSI(*SessionName);
			EOS_HSessionModification LocalHandle = nullptr;
			auto ReturnResult = EOS_Sessions_UpdateSessionModification(EOSRef->SessionsHandle, &Options, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionModificationHandle = LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_Sessions_UpdateSessionModification: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionSearch_CopySearchResultByIndex(FEIK_HSessionSearch Handle,
	int32 SessionIndex, FEIK_HSessionDetails& OutSessionHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_CopySearchResultByIndex: Invalid Session Search Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_SessionSearch_CopySearchResultByIndexOptions Options;
			Options.ApiVersion = EOS_SESSIONSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
			Options.SessionIndex = SessionIndex;
			EOS_HSessionDetails LocalHandle = nullptr;
			auto ReturnResult = EOS_SessionSearch_CopySearchResultByIndex(*Handle.Ref, &Options, &LocalHandle);
			if (ReturnResult == EOS_EResult::EOS_Success)
			{
				OutSessionHandle = LocalHandle;
			}
			return static_cast<EEIK_Result>(ReturnResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_CopySearchResultByIndex: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

int32 UEIK_SessionsSubsystem::EIK_SessionSearch_GetSearchResultCount(FEIK_HSessionSearch Handle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_GetSearchResultCount: Invalid Session Search Handle."));
				return -1;
			}
			EOS_SessionSearch_GetSearchResultCountOptions Options;
			Options.ApiVersion = EOS_SESSIONSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;
			return EOS_SessionSearch_GetSearchResultCount(*Handle.Ref, &Options);
		}
	}
	return -1;
}

void UEIK_SessionsSubsystem::EIK_SessionSearch_Release(FEIK_HSessionSearch Handle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_Release: Invalid Session Search Handle."));
				return;
			}
			EOS_SessionSearch_Release(*Handle.Ref);
		}
	}
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionSearch_RemoveParameter(FEIK_HSessionSearch Handle,
	const FString& Key, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_RemoveParameter: Invalid Session Search Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_SessionSearch_RemoveParameterOptions Options;
			Options.ApiVersion = EOS_SESSIONSEARCH_REMOVEPARAMETER_API_LATEST;
			Options.ComparisonOp = static_cast<EOS_EComparisonOp>(ComparisonOp.GetValue());
			Options.Key = TCHAR_TO_ANSI(*Key);
			return static_cast<EEIK_Result>(EOS_SessionSearch_RemoveParameter(*Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_RemoveParameter: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionSearch_SetMaxResults(FEIK_HSessionSearch Handle,
	int32 MaxSearchResults)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetMaxResults: Invalid Session Search Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_SessionSearch_SetMaxResultsOptions Options;
			Options.ApiVersion = EOS_SESSIONSEARCH_SETMAXSEARCHRESULTS_API_LATEST;
			Options.MaxSearchResults = MaxSearchResults;
			return static_cast<EEIK_Result>(EOS_SessionSearch_SetMaxResults(*Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetMaxResults: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionSearch_SetParameter(FEIK_HSessionSearch Handle,
	FEIK_Sessions_AttributeData Parameter, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetParameter: Invalid Session Search Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_SessionSearch_SetParameterOptions Options;
			Options.ApiVersion = EOS_SESSIONSEARCH_SETPARAMETER_API_LATEST;
			Options.ComparisonOp = static_cast<EOS_EComparisonOp>(ComparisonOp.GetValue());
			EOS_Sessions_AttributeData LocalData = Parameter.GetValueAsEosType();
			Options.Parameter = &LocalData;
			return static_cast<EEIK_Result>(EOS_SessionSearch_SetParameter(*Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetParameter: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionSearch_SetSessionId(FEIK_HSessionSearch Handle,
	const FString& SessionId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetSessionId: Invalid Session Search Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_SessionSearch_SetSessionIdOptions Options;
			Options.ApiVersion = EOS_SESSIONSEARCH_SETSESSIONID_API_LATEST;
			Options.SessionId = TCHAR_TO_ANSI(*SessionId);
			return static_cast<EEIK_Result>(EOS_SessionSearch_SetSessionId(*Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetSessionId: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_SessionsSubsystem::EIK_SessionSearch_SetTargetUserId(FEIK_HSessionSearch Handle,
	FEIK_ProductUserId TargetUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(Handle.Ref == nullptr)
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetTargetUserId: Invalid Session Search Handle."));
				return EEIK_Result::EOS_NotFound;
			}
			EOS_SessionSearch_SetTargetUserIdOptions Options;
			Options.ApiVersion = EOS_SESSIONSEARCH_SETTARGETUSERID_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			return static_cast<EEIK_Result>(EOS_SessionSearch_SetTargetUserId(*Handle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_SessionsSubsystem::EIK_SessionSearch_SetTargetUserId: Unable to get EOS subsystem."));
	return EEIK_Result::EOS_NotFound;
}
