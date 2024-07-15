// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_UiSubsystem.h"

#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

TEnumAsByte<EEIK_Result> UEIK_UiSubsystem::EIK_UI_AcknowledgeEventId(const FEIK_UI_EventId& UiEventId, const TEnumAsByte<EEIK_Result>& Result)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_AcknowledgeEventIdOptions Options;
			Options.ApiVersion = EOS_UI_ACKNOWLEDGEEVENTID_API_LATEST;
			Options.Result = static_cast<EOS_EResult>(Result.GetValue());
			Options.UiEventId = UiEventId.Ref;
			auto AcknowledgeResult = EOS_UI_AcknowledgeEventId(EOSRef->UIHandle, &Options);
			return static_cast<EEIK_Result>(AcknowledgeResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_AcknowledgeEventId: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

FEIK_NotificationId UEIK_UiSubsystem::EIK_UI_AddNotifyDisplaySettingsUpdated(FEIK_OnDisplaySettingsUpdated Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_AddNotifyDisplaySettingsUpdatedOptions Options;
			Options.ApiVersion = EOS_UI_ADDNOTIFYDISPLAYSETTINGSUPDATED_API_LATEST;
			auto NotificationId = EOS_UI_AddNotifyDisplaySettingsUpdated(EOSRef->UIHandle, &Options,this,[](const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo* Data)
			{
				UEIK_UiSubsystem* UiSubsystem = static_cast<UEIK_UiSubsystem*>(Data->ClientData);
				if (UiSubsystem)
				{
					bool bIsVisible = Data->bIsVisible == EOS_TRUE;
					bool bIsExclusiveInput = Data->bIsExclusiveInput == EOS_TRUE;
					UiSubsystem->OnDisplaySettingsUpdated.ExecuteIfBound(bIsVisible, bIsExclusiveInput);
				}
			});
			return NotificationId;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_AddNotifyDisplaySettingsUpdated: Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_UiSubsystem::EIK_UI_AddNotifyMemoryMonitor(FEIK_OnMemoryMonitor Callback)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_AddNotifyMemoryMonitorOptions Options;
			Options.ApiVersion = EOS_UI_ADDNOTIFYMEMORYMONITOR_API_LATEST;
			auto NotificationId = EOS_UI_AddNotifyMemoryMonitor(EOSRef->UIHandle, &Options, this, [](const EOS_UI_MemoryMonitorCallbackInfo* Data)
			{
				UEIK_UiSubsystem* UiSubsystem = static_cast<UEIK_UiSubsystem*>(Data->ClientData);
				if (UiSubsystem)
				{
					UiSubsystem->OnMemoryMonitor.ExecuteIfBound();
				}
			});
			return NotificationId;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_AddNotifyMemoryMonitor: Failed to get EOS subsystem"));
	return FEIK_NotificationId();
}

bool UEIK_UiSubsystem::EIK_UI_GetFriendsExclusiveInput(FEIK_EpicAccountId LocalUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_GetFriendsExclusiveInputOptions Options;
			Options.ApiVersion = EOS_UI_GETFRIENDSEXCLUSIVEINPUT_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			auto ExclusiveInput = EOS_UI_GetFriendsExclusiveInput(EOSRef->UIHandle, &Options);
			return ExclusiveInput == EOS_TRUE;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_GetFriendsExclusiveInput: Failed to get EOS subsystem"));
	return false;
}

bool UEIK_UiSubsystem::EIK_UI_GetFriendsVisible(FEIK_EpicAccountId LocalUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_GetFriendsVisibleOptions Options;
			Options.ApiVersion = EOS_UI_GETFRIENDSVISIBLE_API_LATEST;
			Options.LocalUserId = LocalUserId.GetValueAsEosType();
			auto FriendsVisible = EOS_UI_GetFriendsVisible(EOSRef->UIHandle, &Options);
			return FriendsVisible == EOS_TRUE;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_GetFriendsVisible: Failed to get EOS subsystem"));
	return false;
}

TEnumAsByte<EEIK_UI_ENotificationLocation> UEIK_UiSubsystem::EIK_UI_GetNotificationLocationPreference()
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			auto NotificationLocation = EOS_UI_GetNotificationLocationPreference(EOSRef->UIHandle);
			return static_cast<EEIK_UI_ENotificationLocation>(NotificationLocation);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_GetNotificationLocationPreference: Failed to get EOS subsystem"));
	return EEIK_UI_ENotificationLocation::EIK_UNL_TopLeft;
}

TEnumAsByte<EEIK_Result> UEIK_UiSubsystem::EIK_UI_PauseSocialOverlay(bool bIsPaused)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_PauseSocialOverlayOptions Options;
			Options.ApiVersion = EOS_UI_PAUSESOCIALOVERLAY_API_LATEST;
			Options.bIsPaused = bIsPaused;
			auto PauseResult = EOS_UI_PauseSocialOverlay(EOSRef->UIHandle, &Options);
			return static_cast<EEIK_Result>(PauseResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_PauseSocialOverlay: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

void UEIK_UiSubsystem::EIK_UI_RemoveNotifyDisplaySettingsUpdated(FEIK_NotificationId NotificationId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_RemoveNotifyDisplaySettingsUpdated(EOSRef->UIHandle, NotificationId.GetValueAsEosType());
		}
	}
}

void UEIK_UiSubsystem::EIK_UI_RemoveNotifyMemoryMonitor(FEIK_NotificationId NotificationId)
{
	{
		if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
		{
			if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
			{
				EOS_UI_RemoveNotifyMemoryMonitor(EOSRef->UIHandle, NotificationId.GetValueAsEosType());
			}
		}
	}
}

TEnumAsByte<EEIK_Result> UEIK_UiSubsystem::EIK_UI_SetDisplayPreference(
	const EEIK_UI_ENotificationLocation& DisplayPreference)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_UI_SetDisplayPreferenceOptions Options;
			Options.ApiVersion = EOS_UI_SETDISPLAYPREFERENCE_API_LATEST;
			Options.NotificationLocation = static_cast<EOS_UI_ENotificationLocation>(DisplayPreference);
			auto SetDisplayResult = EOS_UI_SetDisplayPreference(EOSRef->UIHandle, &Options);
			return static_cast<EEIK_Result>(SetDisplayResult);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_UiSubsystem::EIK_UI_SetDisplayPreference: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}
