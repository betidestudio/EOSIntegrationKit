// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_LobbySubsystem.h"

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
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnJoinLobbyAccepted.ExecuteIfBound(Data->LocalUserId, Data->UiEventId);
					});
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
			EOS_Lobby_RemoveNotifyJoinLobbyAccepted(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
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
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnLeaveLobbyRequested.ExecuteIfBound(Data->LocalUserId, Data->LobbyId);
					});
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
			EOS_Lobby_RemoveNotifyLeaveLobbyRequested(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
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
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnLobbyInviteAccepted.ExecuteIfBound(Data->LocalUserId, Data->TargetUserId, Data->LobbyId, Data->InviteId);
					});
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
			EOS_Lobby_RemoveNotifyLobbyInviteAccepted(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
		}
	}
}

FEIK_NotificationId UEIK_LobbySubsystem::EIK_Lobby_AddNotifyLobbyInviteReceived(
	FEIK_Lobby_OnLobbyInviteReceivedCallback Callback)
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
					AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
					{
						CallbackObj->OnLobbyInviteReceived.ExecuteIfBound(Data->LocalUserId, Data->TargetUserId, Data->InviteId);
					});
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
			EOS_Lobby_RemoveNotifyLobbyInviteReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
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
			EOS_Lobby_RemoveNotifyLobbyInviteRejected(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyMemberStatusReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyMemberStatusReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyMemberUpdateReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyMemberUpdateReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyLobbyUpdateReceived(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyLobbyUpdateReceived(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifyRTCRoomConnectionChanged(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_RemoveNotifySendLobbyNativeInviteRequested(FEIK_NotificationId InId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RemoveNotifySendLobbyNativeInviteRequested(EOSRef->SessionInterfacePtr->LobbyHandle, InId.EOS_NotificationId_FromInt64());
		}
	}
}

void UEIK_LobbySubsystem::EIK_Lobby_Attribute_Release(const FEIK_Lobby_Attribute& Attribute)
{
	EOS_Lobby_Attribute_Release(Attribute.Ref);
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
			Options.LocalUserId = LocalUserId.ProductUserIdBasic;
			EOS_HLobbyDetails Handle = nullptr;
			EEIK_Result Result = static_cast<EEIK_Result>(EOS_Lobby_CopyLobbyDetailsHandle(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, &Handle));
			OutLobbyDetailsHandle = &Handle;
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
			OutLobbyDetailsHandle = &Handle;
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
			OutLobbyDetailsHandle = &Handle;
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
			OutLobbySearchHandle = &Handle;
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
			Options.LocalUserId = LocalUserId.ProductUserId_FromString();
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
			Options.LocalUserId = LocalUserId.ProductUserId_FromString();
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
			Options.LocalUserId = LocalUserId.ProductUserId_FromString();
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
			Options.LocalUserId = LocalUserId.ProductUserId_FromString();
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
			Options.LocalUserId = LocalUserId.ProductUserId_FromString();
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
