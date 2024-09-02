// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_LobbySubsystem.h"

#include "EIKSettings.h"
#include "eos_lobby.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyJoinLobbyAccepted(FEIK_Lobby_OnJoinLobbyAcceptedCallback Callback)
{
	OnJoinLobbyAccepted = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYJOINLOBBYACCEPTED_API_LATEST;
			return EOS_Lobby_AddNotifyJoinLobbyAccepted(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_JoinLobbyAcceptedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
						CallbackObj->OnJoinLobbyAccepted.ExecuteIfBound(Data->LocalUserId, Data->UiEventId);
				}
			});
		}
	}
	return FEIK_NotificationId();
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyJoinLobbyAccepted(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyJoinLobbyAccepted(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLeaveLobbyRequested(
	FEIK_Lobby_OnLeaveLobbyRequestedCallback Callback)
{
	OnLeaveLobbyRequested = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyLeaveLobbyRequestedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYLEAVELOBBYREQUESTED_API_LATEST;
			return EOS_Lobby_AddNotifyLeaveLobbyRequested(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_LeaveLobbyRequestedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
						CallbackObj->OnLeaveLobbyRequested.ExecuteIfBound(Data->LocalUserId, Data->LobbyId);
				}
			});
		}
	}
	return FEIK_NotificationId();
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLeaveLobbyRequested(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLeaveLobbyRequested(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLobbyInviteAccepted(
	FEIK_Lobby_OnLobbyInviteAcceptedCallback Callback)
{
	OnLobbyInviteAccepted = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITEACCEPTED_API_LATEST;
			return EOS_Lobby_AddNotifyLobbyInviteAccepted(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_LobbyInviteAcceptedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					CallbackObj->OnLobbyInviteAccepted.ExecuteIfBound(Data->LocalUserId, Data->TargetUserId, Data->LobbyId, Data->InviteId);
				}
			});
		}
	}
	return FEIK_NotificationId();
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyInviteAccepted(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyInviteAccepted(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLobbyInviteReceived(FEIK_Lobby_OnLobbyInviteReceivedCallback Callback)
{
	OnLobbyInviteReceived = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyLobbyInviteReceivedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITERECEIVED_API_LATEST;
			return EOS_Lobby_AddNotifyLobbyInviteReceived(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_LobbyInviteReceivedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					CallbackObj->OnLobbyInviteReceived.ExecuteIfBound(Data->LocalUserId, Data->TargetUserId, StringCast<TCHAR>(Data->InviteId).Get());
				}
			});
		}
	}
	return FEIK_NotificationId();
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyInviteReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyInviteReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLobbyInviteRejected(
	FEIK_Lobby_OnLobbyInviteRejectedCallback Callback)
{
	OnLobbyInviteRejected = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyLobbyInviteRejectedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITEREJECTED_API_LATEST;
			return EOS_Lobby_AddNotifyLobbyInviteRejected(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_LobbyInviteRejectedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnLobbyInviteRejected.ExecuteIfBound(Data->LocalUserId, Data->TargetUserId, Data->LobbyId, Data->InviteId);
					});
				}
			});
		}
	}
	return FEIK_NotificationId();
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyInviteRejected(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyInviteRejected(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyMemberStatusReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyMemberStatusReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyMemberUpdateReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyMemberUpdateReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyUpdateReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyUpdateReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyRTCRoomConnectionChanged(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifySendLobbyNativeInviteRequested(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifySendLobbyNativeInviteRequested(EOSRef->SessionInterfacePtr->LobbyHandle, InId.GetValueAsEosType());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_Attribute_Release(const FEIK_Lobby_Attribute& Attribute)
{
	EOS_Lobby_Attribute Attr = Attribute.Ref;
	EOS_Lobby_Attribute_Release(&Attr);
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_CopyLobbyDetailsHandle(FEIK_LobbyId LobbyId,
	FEIK_ProductUserId LocalUserId, FEIK_HLobbyDetails& OutLobbyDetailsHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_CopyLobbyDetailsHandleOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLE_API_LATEST;
			Options.LobbyId = LobbyId.Ref;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			EOS_HLobbyDetails Handle = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_CopyLobbyDetailsHandle(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, &Handle));
			OutLobbyDetailsHandle = Handle;
			return Result;
		}
	}
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_CopyLobbyDetailsHandleByInviteId(FString InviteId,
	FEIK_HLobbyDetails& OutLobbyDetailsHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYINVITEID_API_LATEST;
			Options.InviteId = TCHAR_TO_ANSI(*InviteId);
			EOS_HLobbyDetails Handle = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_CopyLobbyDetailsHandleByInviteId(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, &Handle));
			OutLobbyDetailsHandle = Handle;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_CopyLobbyDetailsHandleByInviteId: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_CopyLobbyDetailsHandleByUiEventId(
	const FEIK_UI_EventId& UiEventId, FEIK_HLobbyDetails& OutLobbyDetailsHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYUIEVENTID_API_LATEST;
			Options.UiEventId = UiEventId.Ref;
			EOS_HLobbyDetails Handle = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_CopyLobbyDetailsHandleByUiEventId(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, &Handle));
			OutLobbyDetailsHandle = Handle;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_CopyLobbyDetailsHandleByUiEventId: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_CreateLobbySearch(int32 MaxResults,
	FEIK_HLobbySearch& OutLobbySearchHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_CreateLobbySearchOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_CREATELOBBYSEARCH_API_LATEST;
			Options.MaxResults = MaxResults;
			EOS_HLobbySearch Handle = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_CreateLobbySearch(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, &Handle));
			if(Result != EEIK_Result::EOS_Success)
			{
				return Result;
			}
			OutLobbySearchHandle = Handle;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_CreateLobbySearch: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_GetConnectString(FEIK_ProductUserId LocalUserId,
	FEIK_LobbyId LobbyId, FString& OutConnectString)
{
	char Buffer[EOS_LOBBY_GETCONNECTSTRING_BUFFER_SIZE];
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_GetConnectStringOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_GETCONNECTSTRING_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.LobbyId = LobbyId.Ref;
			uint32_t* InOutBufferLength= nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_GetConnectString(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, Buffer, InOutBufferLength));
			OutConnectString = Buffer;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_GetConnectString: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

int32 UEIK_LobbySubsystem::EIK_Lobby_GetInviteCount(FEIK_ProductUserId LocalUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_GetInviteCountOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_GETINVITECOUNT_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			return EOS_Lobby_GetInviteCount(EOSRef->SessionInterfacePtr->LobbyHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_GetInviteCount: OnlineSubsystemEOS is not valid"));
	return 0;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_GetInviteIdByIndex(FEIK_ProductUserId LocalUserId, int32 Index,
	FString& OutInviteId)
{
	char Buffer[256];
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_GetInviteIdByIndexOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_GETINVITEIDBYINDEX_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.Index = Index;
			int32_t* InOutBufferLength = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_GetInviteIdByIndex(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, Buffer, InOutBufferLength));
			OutInviteId = Buffer;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_GetInviteIdByIndex: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_GetRTCRoomName(FEIK_ProductUserId LocalUserId,
	FEIK_LobbyId LobbyId, FString& OutRTCRoomName)
{
	char Buffer[256];
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_GetRTCRoomNameOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_GETRTCROOMNAME_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.LobbyId = LobbyId.Ref;
			uint32_t* InOutBufferLength = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_GetRTCRoomName(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, Buffer, InOutBufferLength));
			OutRTCRoomName = Buffer;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_GetRTCRoomName: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_IsRTCRoomConnected(FEIK_ProductUserId LocalUserId,
	FEIK_LobbyId LobbyId, bool& bOutIsConnected)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_IsRTCRoomConnectedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ISRTCROOMCONNECTED_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.LobbyId = LobbyId.Ref;
			EOS_Bool VarTemp = EOS_FALSE;
			EOS_EResult Result = EOS_Lobby_IsRTCRoomConnected(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, &VarTemp);
			bOutIsConnected = VarTemp == EOS_TRUE;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_IsRTCRoomConnected: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_ParseLobbyIdFromConnectString(FString ConnectString,
	FEIK_LobbyId& OutLobbyId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_Lobby_ParseConnectStringOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_PARSECONNECTSTRING_API_LATEST;
			Options.ConnectString = TCHAR_TO_ANSI(*ConnectString);
			char* OutBuffer = nullptr;
			uint32_t* InOutBufferLength = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_ParseConnectString(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, OutBuffer, InOutBufferLength));
			OutLobbyId = OutBuffer;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_ParseLobbyIdFromConnectString: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_Lobby_UpdateLobbyModification(FEIK_ProductUserId LocalUserId,
	FEIK_LobbyId LobbyId, FEIK_HLobbyModification& OutLobbyModificationHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_Lobby_UpdateLobbyModificationOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			Options.LobbyId = LobbyId.Ref;
			EOS_HLobbyModification Handle = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_UpdateLobbyModification(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, &Handle));
			OutLobbyModificationHandle = &Handle;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_UpdateLobbyModification: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyDetails_CopyAttributeByIndex(
	FEIK_HLobbyDetails LobbyDetailsHandle, int32 AttrIndex, FEIK_Lobby_Attribute& OutAttribute)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_CopyAttributeByIndexOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST;
			Options.AttrIndex = AttrIndex;
			EOS_Lobby_Attribute* Attribute = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_LobbyDetails_CopyAttributeByIndex(LobbyDetailsHandle.Ref, &Options, &Attribute));
			if(Result != EEIK_Result::EOS_Success)
			{
				return Result;
			}
			OutAttribute = *Attribute;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyAttributeByIndex: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyDetails_CopyAttributeByKey(FEIK_HLobbyDetails LobbyDetailsHandle,
	const FString& AttrKey, FEIK_Lobby_Attribute& OutAttribute)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_CopyAttributeByKeyOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_COPYATTRIBUTEBYKEY_API_LATEST;
			Options.AttrKey = TCHAR_TO_ANSI(*AttrKey);
			EOS_Lobby_Attribute* Attribute = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_LobbyDetails_CopyAttributeByKey(LobbyDetailsHandle.Ref, &Options, &Attribute));
			if(Result != EEIK_Result::EOS_Success)
			{
				return Result;
			}
			OutAttribute = *Attribute;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyAttributeByKey: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyDetails_CopyInfo(FEIK_HLobbyDetails LobbyDetailsHandle,
	FEIK_LobbyDetailsInfo& OutLobbyDetailsInfo)
{
	if(!LobbyDetailsHandle.Ref)
	{
		UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyInfo: LobbyDetailsHandle is not valid"));
		return EEIK_Result::EOS_NotFound;
	}
	FEIK_LobbyDetailsInfo Info = {};
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_CopyInfoOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_COPYINFO_API_LATEST;
			EOS_LobbyDetails_Info* Info1 = NULL;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_LobbyDetails_CopyInfo(LobbyDetailsHandle.Ref, &Options, &Info1));
			if(Result != EEIK_Result::EOS_Success)
			{
				UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyInfo: Failed to copy info"));
				return Result;
			}
			OutLobbyDetailsInfo = *Info1;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyInfo: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyDetails_CopyMemberAttributeByIndex(
	FEIK_HLobbyDetails LobbyDetailsHandle, FEIK_ProductUserId TargetUserId, int32 AttrIndex,
	FEIK_Lobby_Attribute& OutAttribute)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_CopyMemberAttributeByIndexOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYINDEX_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			Options.AttrIndex = AttrIndex;
			EOS_Lobby_Attribute* Attribute = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_LobbyDetails_CopyMemberAttributeByIndex(LobbyDetailsHandle.Ref, &Options, &Attribute));
			if(Result != EEIK_Result::EOS_Success)
			{
				return Result;
			}
			OutAttribute = *Attribute;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyMemberAttributeByIndex: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyDetails_CopyMemberAttributeByKey(
	FEIK_HLobbyDetails LobbyDetailsHandle, FEIK_ProductUserId TargetUserId, const FString& AttrKey,
	FEIK_Lobby_Attribute& OutAttribute)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_CopyMemberAttributeByKeyOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYKEY_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			Options.AttrKey = TCHAR_TO_ANSI(*AttrKey);
			EOS_Lobby_Attribute* Attribute = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_LobbyDetails_CopyMemberAttributeByKey(LobbyDetailsHandle.Ref, &Options, &Attribute));
			if(Result != EEIK_Result::EOS_Success)
			{
				return Result;
			}
			OutAttribute = *Attribute;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyMemberAttributeByKey: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyDetails_CopyMemberInfo(FEIK_HLobbyDetails LobbyDetailsHandle,
	FEIK_ProductUserId TargetUserId, FEIK_LobbyDetails_MemberInfo& OutMemberInfo)
{
	FEIK_LobbyDetails_MemberInfo Info = {};
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_CopyMemberInfoOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_COPYMEMBERINFO_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			EOS_LobbyDetails_MemberInfo* Info1 = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_LobbyDetails_CopyMemberInfo(LobbyDetailsHandle.Ref, &Options, &Info1));
			Info = *Info1;
			OutMemberInfo = Info;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_CopyMemberInfo: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

int32 UEIK_LobbySubsystem::EIK_LobbyDetails_GetAttributeCount(FEIK_HLobbyDetails LobbyDetailsHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_GetAttributeCountOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_GETATTRIBUTECOUNT_API_LATEST;
			return EOS_LobbyDetails_GetAttributeCount(LobbyDetailsHandle.Ref, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_GetAttributeCount: OnlineSubsystemEOS is not valid"));
	return 0;
}

FEIK_ProductUserId UEIK_LobbySubsystem::EIK_LobbyDetails_GetLobbyOwner(FEIK_HLobbyDetails LobbyDetailsHandle)
{
	EOS_ProductUserId ProductUserId = nullptr;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_GetLobbyOwnerOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_GETLOBBYOWNER_API_LATEST;
			ProductUserId = EOS_LobbyDetails_GetLobbyOwner(LobbyDetailsHandle.Ref, &Options);
		}
	}
	return ProductUserId;
}

int32 UEIK_LobbySubsystem::EIK_LobbyDetails_GetMemberAttributeCount(FEIK_HLobbyDetails LobbyDetailsHandle,
	FEIK_ProductUserId TargetUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_GetMemberAttributeCountOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERATTRIBUTECOUNT_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			return EOS_LobbyDetails_GetMemberAttributeCount(LobbyDetailsHandle.Ref, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_GetMemberAttributeCount: OnlineSubsystemEOS is not valid"));
	return 0;
}

FEIK_ProductUserId UEIK_LobbySubsystem::EIK_LobbyDetails_GetMemberByIndex(FEIK_HLobbyDetails LobbyDetailsHandle,
	int32 MemberIndex)
{
	EOS_ProductUserId ProductUserId = nullptr;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_GetMemberByIndexOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERBYINDEX_API_LATEST;
			Options.MemberIndex = MemberIndex;
			ProductUserId = EOS_LobbyDetails_GetMemberByIndex(LobbyDetailsHandle.Ref, &Options);
		}
	}
	return ProductUserId;
}

int32 UEIK_LobbySubsystem::EIK_LobbyDetails_GetMemberCount(FEIK_HLobbyDetails LobbyDetailsHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_GetMemberCountOptions Options = {};
			Options.ApiVersion = EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST;
			return EOS_LobbyDetails_GetMemberCount(LobbyDetailsHandle.Ref, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyDetails_GetMemberCount: OnlineSubsystemEOS is not valid"));
	return 0;
}

void UEIK_LobbySubsystem::EIK_LobbyDetails_Info_Release(FEIK_LobbyDetailsInfo& LobbyDetailsInfo)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_Info_Release(&LobbyDetailsInfo.Ref);
		}
	}
}

void UEIK_LobbySubsystem::EIK_LobbyDetails_MemberInfo_Release(FEIK_LobbyDetails_MemberInfo& MemberInfo)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyDetails_MemberInfo_Release(&MemberInfo.Ref);
		}
	}
}
void UEIK_LobbySubsystem::EIK_LobbyDetails_Release(FEIK_HLobbyDetails LobbyDetailsHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbyDetails_Release(LobbyDetailsHandle.Ref);
		}
	}
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_AddAttribute(
	FEIK_HLobbyModification LobbyModificationHandle, const FEIK_Lobby_AttributeData& Attribute,
	const TEnumAsByte<EEIK_ELobbyAttributeVisibility>& Visibility)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbyModification_AddAttributeOptions Options = {};
			Options.ApiVersion = EOS_LOBBYMODIFICATION_ADDATTRIBUTE_API_LATEST;
			Options.Attribute = &Attribute.Ref;
			Options.Visibility = static_cast<EOS_ELobbyAttributeVisibility>(Visibility.GetValue());
			return static_cast<EEIK_Result>(EOS_LobbyModification_AddAttribute(*LobbyModificationHandle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_AddAttribute: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_AddMemberAttribute(
	FEIK_HLobbyModification LobbyModificationHandle, const FEIK_Lobby_AttributeData& Attribute,
	const TEnumAsByte<EEIK_ELobbyAttributeVisibility>& Visibility)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbyModification_AddMemberAttributeOptions Options = {};
			Options.ApiVersion = EOS_LOBBYMODIFICATION_ADDMEMBERATTRIBUTE_API_LATEST;
			Options.Attribute = &Attribute.Ref;
			Options.Visibility = static_cast<EOS_ELobbyAttributeVisibility>(Visibility.GetValue());
			return static_cast<EEIK_Result>(EOS_LobbyModification_AddMemberAttribute(*LobbyModificationHandle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_AddMemberAttribute: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_LobbySubsystem::EIK_LobbyModification_Release(FEIK_HLobbyModification LobbyModificationHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbyModification_Release(*LobbyModificationHandle.Ref);
		}
	}
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_RemoveAttribute(
	FEIK_HLobbyModification LobbyModificationHandle, const FString& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbyModification_RemoveAttributeOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYMODIFICATION_REMOVEATTRIBUTE_API_LATEST;
			LocalOptions.Key = TCHAR_TO_ANSI(*Options);
			return static_cast<EEIK_Result>(EOS_LobbyModification_RemoveAttribute(*LobbyModificationHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_RemoveAttribute: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_RemoveMemberAttribute(
	FEIK_HLobbyModification LobbyModificationHandle, const FString& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbyModification_RemoveMemberAttributeOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYMODIFICATION_REMOVEMEMBERATTRIBUTE_API_LATEST;
			LocalOptions.Key = TCHAR_TO_ANSI(*Options);
			return static_cast<EEIK_Result>(EOS_LobbyModification_RemoveMemberAttribute(*LobbyModificationHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_RemoveMemberAttribute: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_SetAllowedPlatformIds(
	FEIK_HLobbyModification LobbyModificationHandle, const TArray<int32>& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			TArray<int32> PlatformIds;
			for (int32 Id : Options)
			{
				PlatformIds.Add(static_cast<int32>(Id));
			}
			EOS_LobbyModification_SetAllowedPlatformIdsOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYMODIFICATION_SETALLOWEDPLATFORMIDS_API_LATEST;
			LocalOptions.AllowedPlatformIdsCount = PlatformIds.Num();
			uint32_t LocalArray[256];
			for (int32 i = 0; i < PlatformIds.Num(); i++)
			{
				LocalArray[i] = PlatformIds[i];
			}
			LocalOptions.AllowedPlatformIds = LocalArray;
			return static_cast<EEIK_Result>(EOS_LobbyModification_SetAllowedPlatformIds(*LobbyModificationHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_SetAllowedPlatformIds: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_SetBucketId(
	FEIK_HLobbyModification LobbyModificationHandle, const FString& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyModification_SetBucketIdOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYMODIFICATION_SETBUCKETID_API_LATEST;
			LocalOptions.BucketId = TCHAR_TO_ANSI(*Options);
			return static_cast<EEIK_Result>(EOS_LobbyModification_SetBucketId(*LobbyModificationHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_SetBucketId: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_SetInvitesAllowed(
	FEIK_HLobbyModification LobbyModificationHandle, const bool& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyModification_SetInvitesAllowedOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYMODIFICATION_SETINVITESALLOWED_API_LATEST;
			LocalOptions.bInvitesAllowed = Options ? EOS_TRUE : EOS_FALSE;
			return static_cast<EEIK_Result>(EOS_LobbyModification_SetInvitesAllowed(*LobbyModificationHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_SetInvitesAllowed: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_SetMaxMembers(
	FEIK_HLobbyModification LobbyModificationHandle, const int32& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyModification_SetMaxMembersOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYMODIFICATION_SETMAXMEMBERS_API_LATEST;
			LocalOptions.MaxMembers = Options;
			return static_cast<EEIK_Result>(EOS_LobbyModification_SetMaxMembers(*LobbyModificationHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_SetMaxMembers: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbyModification_SetPermissionLevel(
	FEIK_HLobbyModification LobbyModificationHandle, const TEnumAsByte<EEIK_ELobbyPermissionLevel>& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub)) {
			EOS_LobbyModification_SetPermissionLevelOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYMODIFICATION_SETPERMISSIONLEVEL_API_LATEST;
			LocalOptions.PermissionLevel = static_cast<EOS_ELobbyPermissionLevel>(Options.GetValue());
			return static_cast<EEIK_Result>(EOS_LobbyModification_SetPermissionLevel(*LobbyModificationHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbyModification_SetPermissionLevel: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbySearch_CopySearchResultByIndex(
	FEIK_HLobbySearch LobbySearchHandle, int32 LobbyIndex, FEIK_HLobbyDetails& OutLobbyDetailsHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_HLobbyDetails Handle = nullptr;
			EOS_LobbySearch_CopySearchResultByIndexOptions Options = {};
			Options.ApiVersion = EOS_LOBBYSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
			Options.LobbyIndex = LobbyIndex;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_LobbySearch_CopySearchResultByIndex(LobbySearchHandle.Ref, &Options, &Handle));
			OutLobbyDetailsHandle = Handle;
			return Result;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbySearch_CopySearchResultByIndex: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

int32 UEIK_LobbySubsystem::EIK_LobbySearch_GetSearchResultCount(FEIK_HLobbySearch LobbySearchHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_GetSearchResultCountOptions Options = {};
			Options.ApiVersion = EOS_LOBBYSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;
			return EOS_LobbySearch_GetSearchResultCount(LobbySearchHandle.Ref, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbySearch_GetSearchResultCount: OnlineSubsystemEOS is not valid"));
	return 0;
}

void UEIK_LobbySubsystem::EIK_LobbySearch_Release(FEIK_HLobbySearch LobbySearchHandle)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_Release(LobbySearchHandle.Ref);
		}
	}
	
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbySearch_RemoveParameter(FEIK_HLobbySearch LobbySearchHandle,
	const FString& Key, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_RemoveParameterOptions Options = {};
			Options.ApiVersion = EOS_LOBBYSEARCH_REMOVEPARAMETER_API_LATEST;
			Options.ComparisonOp = static_cast<EOS_EComparisonOp>(ComparisonOp.GetValue());
			Options.Key = TCHAR_TO_ANSI(*Key);
			return static_cast<EEIK_Result>(EOS_LobbySearch_RemoveParameter(LobbySearchHandle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbySearch_RemoveParameter: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbySearch_SetLobbyId(FEIK_HLobbySearch LobbySearchHandle,
	const FEIK_LobbyId& Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_SetLobbyIdOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYSEARCH_SETLOBBYID_API_LATEST;
			LocalOptions.LobbyId = Options.Ref;
			return static_cast<EEIK_Result>(EOS_LobbySearch_SetLobbyId(LobbySearchHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbySearch_SetLobbyId: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbySearch_SetMaxResults(FEIK_HLobbySearch LobbySearchHandle,
	const int32 MaxResults)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_SetMaxResultsOptions Options = {};
			Options.ApiVersion = EOS_LOBBYSEARCH_SETMAXRESULTS_API_LATEST;
			Options.MaxResults = MaxResults;
			return static_cast<EEIK_Result>(EOS_LobbySearch_SetMaxResults(LobbySearchHandle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbySearch_SetMaxResults: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbySearch_SetParameter(FEIK_HLobbySearch LobbySearchHandle,
	const FEIK_Lobby_AttributeData& Parameter, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_SetParameterOptions Options = {};
			Options.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
			Options.ComparisonOp = static_cast<EOS_EComparisonOp>(ComparisonOp.GetValue());
			Options.Parameter = &Parameter.Ref;
			return static_cast<EEIK_Result>(EOS_LobbySearch_SetParameter(LobbySearchHandle.Ref, &Options));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbySearch_SetParameter: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_LobbySubsystem::EIK_LobbySearch_SetTargetUserId(FEIK_HLobbySearch LobbySearchHandle,
FEIK_ProductUserId Options)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_LobbySearch_SetTargetUserIdOptions LocalOptions = {};
			LocalOptions.ApiVersion = EOS_LOBBYSEARCH_SETTARGETUSERID_API_LATEST;
			LocalOptions.TargetUserId = Options.GetValueAsEosType();
			return static_cast<EEIK_Result>(EOS_LobbySearch_SetTargetUserId(LobbySearchHandle.Ref, &LocalOptions));
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_LobbySearch_SetTargetUserId: OnlineSubsystemEOS is not valid"));
	return EEIK_Result::EOS_NotFound;
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLobbyMemberStatusReceived(
	FEIK_Lobby_OnLobbyMemberStatusReceivedCallback Callback)
{
	OnLobbyMemberStatusReceived = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYMEMBERSTATUSRECEIVED_API_LATEST;
			return EOS_Lobby_AddNotifyLobbyMemberStatusReceived(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnLobbyMemberStatusReceived.ExecuteIfBound(Data->TargetUserId, Data->LobbyId, static_cast<EEIK_ELobbyMemberStatus>(Data->CurrentStatus));
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_AddNotifyLobbyMemberStatusReceived: OnlineSubsystemEOS is not valid"));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLobbyMemberUpdateReceived(
	FEIK_Lobby_OnLobbyMemberUpdateReceivedCallback Callback)
{
	OnLobbyMemberUpdateReceived = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYMEMBERUPDATERECEIVED_API_LATEST;
			return EOS_Lobby_AddNotifyLobbyMemberUpdateReceived(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnLobbyMemberUpdateReceived.ExecuteIfBound(Data->TargetUserId, Data->LobbyId);
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_AddNotifyLobbyMemberUpdateReceived: OnlineSubsystemEOS is not valid"));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLobbyUpdateReceived(
	FEIK_Lobby_OnLobbyUpdateReceivedCallback Callback)
{
	OnLobbyUpdateReceived = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYUPDATERECEIVED_API_LATEST;
			return EOS_Lobby_AddNotifyLobbyUpdateReceived(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_LobbyUpdateReceivedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnLobbyUpdateReceived.ExecuteIfBound(Data->LobbyId);
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_AddNotifyLobbyUpdateReceived: OnlineSubsystemEOS is not valid"));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyRTCRoomConnectionChanged(FEIK_Lobby_OnRTCRoomConnectionChangedCallback Callback)
{
	OnRTCRoomConnectionChanged = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifyRTCRoomConnectionChangedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYRTCROOMCONNECTIONCHANGED_API_LATEST;
			return EOS_Lobby_AddNotifyRTCRoomConnectionChanged(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_RTCRoomConnectionChangedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						bool bResult = true;
						CallbackObj->OnRTCRoomConnectionChanged.ExecuteIfBound(Data->LobbyId, Data->LocalUserId, bResult, static_cast<EEIK_Result>(Data->DisconnectReason));
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_AddNotifyRTCRoomConnectionChanged: OnlineSubsystemEOS is not valid"));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifySendLobbyNativeInviteRequested(
	FEIK_Lobby_OnSendLobbyNativeInviteCallback Callback)
{
	OnSendLobbyNativeInvite = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_AddNotifySendLobbyNativeInviteRequestedOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_ADDNOTIFYSENDLOBBYNATIVEINVITEREQUESTED_API_LATEST;
			return EOS_Lobby_AddNotifySendLobbyNativeInviteRequested(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, [](const EOS_Lobby_SendLobbyNativeInviteRequestedCallbackInfo* Data)
			{
				if (UEIK_LobbySubsystem* CallbackObj = static_cast<UEIK_LobbySubsystem*>(Data->ClientData))
				{
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnSendLobbyNativeInvite.ExecuteIfBound(Data->UiEventId, Data->LocalUserId, Data->TargetUserNativeAccountId, Data->TargetNativeAccountType, Data->LobbyId);
					});
				}
			});
		}
	}
	UE_LOG(LogEIK, Error, TEXT("EIK_Lobby_AddNotifySendLobbyNativeInviteRequested: OnlineSubsystemEOS is not valid"));
	return FEIK_NotificationId();
}
