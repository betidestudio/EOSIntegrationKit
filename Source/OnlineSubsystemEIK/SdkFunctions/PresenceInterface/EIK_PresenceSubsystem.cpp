// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_PresenceSubsystem.h"

#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
THIRD_PARTY_INCLUDES_START
#include "eos_presence.h"
THIRD_PARTY_INCLUDES_END
#include "OnlineSubsystemEOS.h"

FEIK_NotificationId UEIK_PresenceSubsystem::EIK_Presence_AddNotifyJoinGameAccepted(const FEIK_Presence_JoinGameAcceptedCallbackInfo& Callback)
{
	JoinGameAcceptedCallbackInfo = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_AddNotifyJoinGameAcceptedOptions Options = {};
			Options.ApiVersion = EOS_PRESENCE_ADDNOTIFYJOINGAMEACCEPTED_API_LATEST;
			return EOS_Presence_AddNotifyJoinGameAccepted(EOSRef->PresenceHandle, &Options, this, [](const EOS_Presence_JoinGameAcceptedCallbackInfo* Data)
			{
				if (UEIK_PresenceSubsystem* PresenceSubsystem = static_cast<UEIK_PresenceSubsystem*>(Data->ClientData))
				{
					PresenceSubsystem->JoinGameAcceptedCallbackInfo.ExecuteIfBound(Data->JoinInfo, Data->LocalUserId, Data->TargetUserId, Data->UiEventId);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Presence_AddNotifyJoinGameAccepted: OnlineSubsystemEOS is not valid"));
	return FEIK_NotificationId();
	
}

FEIK_NotificationId UEIK_PresenceSubsystem::EIK_Presence_AddNotifyOnPresenceChanged(
	const FEIK_Presence_OnPresenceChangedCallbackInfo& Callback)
{
	PresenceChangedCallbackInfo = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_AddNotifyOnPresenceChangedOptions Options = {};
			Options.ApiVersion = EOS_PRESENCE_ADDNOTIFYONPRESENCECHANGED_API_LATEST;
			return EOS_Presence_AddNotifyOnPresenceChanged(EOSRef->PresenceHandle, &Options, this, [](const EOS_Presence_PresenceChangedCallbackInfo* Data)
			{
				if (UEIK_PresenceSubsystem* PresenceSubsystem = static_cast<UEIK_PresenceSubsystem*>(Data->ClientData))
				{
					PresenceSubsystem->PresenceChangedCallbackInfo.ExecuteIfBound(Data->LocalUserId, Data->PresenceUserId);
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Presence_AddNotifyOnPresenceChanged: OnlineSubsystemEOS is not valid"));
	return FEIK_NotificationId();
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_Presence_CopyPresence(FEIK_EpicAccountId LocalUserId,
	FEIK_EpicAccountId TargetUserId, FEIK_Presence_Info& OutPresence)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_CopyPresenceOptions Options = {};
			Options.ApiVersion = EOS_PRESENCE_COPYPRESENCE_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			EOS_Presence_Info* Presence = nullptr;
			auto Result = EOS_Presence_CopyPresence(EOSRef->PresenceHandle, &Options, &Presence);
			if (Presence)
			{
				OutPresence = *Presence;
			}
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Presence_CopyPresence: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_Presence_CreatePresenceModification(FEIK_EpicAccountId LocalUserId,
	FEIK_HPresenceModification& OutPresenceModificationHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_CreatePresenceModificationOptions Options = {};
			Options.ApiVersion = EOS_PRESENCE_CREATEPRESENCEMODIFICATION_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			EOS_HPresenceModification PresenceModificationHandle = nullptr;
			auto Result = EOS_Presence_CreatePresenceModification(EOSRef->PresenceHandle, &Options, &PresenceModificationHandle);
			if(Result == EOS_EResult::EOS_Success)
			{
				OutPresenceModificationHandle = PresenceModificationHandle;
			}
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Presence_CreatePresenceModification: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_Presence_GetJoinInfo(FEIK_EpicAccountId LocalUserId,
	FEIK_EpicAccountId TargetUserId, FString& OutBuffer)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_GetJoinInfoOptions Options = {};
			Options.ApiVersion = EOS_PRESENCE_GETJOININFO_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			char Buffer[EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH];
			int32_t* OutStringLength = nullptr;
			auto Result = EOS_Presence_GetJoinInfo(EOSRef->PresenceHandle, &Options, Buffer, OutStringLength);
			OutBuffer = Buffer;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Presence_GetJoinInfo: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

bool UEIK_PresenceSubsystem::EIK_Presence_HasPresence(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_HasPresenceOptions Options = {};
			Options.ApiVersion = EOS_PRESENCE_HASPRESENCE_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			auto Result = EOS_Presence_HasPresence(EOSRef->PresenceHandle, &Options);
			if(Result == EOS_TRUE)
			{
				return true;
			}
			return false;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Presence_HasPresence: OnlineSubsystemEOS is not valid"));
	return false;
}

void UEIK_PresenceSubsystem::EIK_Presence_Info_Release(const FEIK_Presence_Info& PresenceInfo)
{
	EOS_Presence_Info ReleasePresenceInfo = PresenceInfo.Ref;
	EOS_Presence_Info_Release(&ReleasePresenceInfo);
}

void UEIK_PresenceSubsystem::EIK_Presence_RemoveNotifyJoinGameAccepted(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_RemoveNotifyJoinGameAccepted(EOSRef->PresenceHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_PresenceSubsystem::EIK_Presence_RemoveNotifyOnPresenceChanged(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Presence_RemoveNotifyOnPresenceChanged(EOSRef->PresenceHandle, InId.GetValueAsEosType());
		}
	}
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_PresenceModification_DeleteData(FEIK_HPresenceModification PresenceModificationHandle, const FEIK_PresenceModification_DataRecordId& Data, int32 Count)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PresenceModification_DeleteDataOptions Options = {};
			Options.ApiVersion = EOS_PRESENCEMODIFICATION_DELETEDATA_API_LATEST;
			Options.RecordsCount = Count;
			Options.Records = &Data.Ref;
			auto Result = EOS_PresenceModification_DeleteData(PresenceModificationHandle.Ref, &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_DeleteData: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_PresenceSubsystem::EIK_PresenceModification_Release(FEIK_HPresenceModification PresenceModificationHandle)
{
	EOS_PresenceModification_Release(PresenceModificationHandle.Ref);
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_PresenceModification_SetData(
	FEIK_HPresenceModification PresenceModificationHandle, const FEIK_Presence_DataRecord& Data, int32 Count)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PresenceModification_SetDataOptions Options = {};
			Options.ApiVersion = EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST;
			Options.RecordsCount = Count;
			Options.Records = &Data.Ref;
			auto Result = EOS_PresenceModification_SetData(PresenceModificationHandle.Ref, &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_SetData: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_PresenceModification_SetJoinInfo(
	FEIK_HPresenceModification PresenceModificationHandle, const FString& JoinInfo)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PresenceModification_SetJoinInfoOptions Options = {};
			Options.ApiVersion = EOS_PRESENCEMODIFICATION_SETJOININFO_API_LATEST;
			Options.JoinInfo = TCHAR_TO_ANSI(*JoinInfo);
			auto Result = EOS_PresenceModification_SetJoinInfo(PresenceModificationHandle.Ref, &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_SetJoinInfo: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_PresenceModification_SetRawRichText(
	FEIK_HPresenceModification PresenceModificationHandle, const FString& RichText)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PresenceModification_SetRawRichTextOptions Options = {};
			Options.ApiVersion = EOS_PRESENCEMODIFICATION_SETRAWRICHTEXT_API_LATEST;
			Options.RichText = TCHAR_TO_ANSI(*RichText);
			auto Result = EOS_PresenceModification_SetRawRichText(PresenceModificationHandle.Ref, &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_SetRawRichText: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_PresenceSubsystem::EIK_PresenceModification_SetStatus(
	FEIK_HPresenceModification PresenceModificationHandle, TEnumAsByte<EEIK_Presence_EStatus> Status)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_PresenceModification_SetStatusOptions Options = {};
			Options.ApiVersion = EOS_PRESENCEMODIFICATION_SETSTATUS_API_LATEST;
			Options.Status = static_cast<EOS_Presence_EStatus>(Status.GetValue());
			auto Result = EOS_PresenceModification_SetStatus(PresenceModificationHandle.Ref, &Options);
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_PresenceModification_SetStatus: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}
