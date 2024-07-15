// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_ui.h"
#include "eos_ui_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_UiSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIK_OnDisplaySettingsUpdated, bool, bIsVisible, bool, bIsExclusiveInput);
DECLARE_DYNAMIC_DELEGATE(FEIK_OnMemoryMonitor);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_UiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//Lets the SDK know that the given UI event ID has been acknowledged and should be released. EOS_Success is returned if the UI event ID has been acknowledged. EOS_NotFound is returned if the UI event ID does not exist.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_AcknowledgeEventId")
	static TEnumAsByte<EEIK_Result> EIK_UI_AcknowledgeEventId(const FEIK_UI_EventId& UiEventId, const TEnumAsByte<EEIK_Result>& Result);

	FEIK_OnDisplaySettingsUpdated OnDisplaySettingsUpdated;
	//Register to receive notifications when the overlay display settings are updated. Newly registered handlers will always be called the next tick with the current state.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_AddNotifyDisplaySettingsUpdated")
	FEIK_NotificationId EIK_UI_AddNotifyDisplaySettingsUpdated(FEIK_OnDisplaySettingsUpdated Callback);

	FEIK_OnMemoryMonitor OnMemoryMonitor;
	//Register to receive notifications from the memory monitor. Newly registered handlers will always be called the next tick with the current state.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_AddNotifyMemoryMonitor")
	FEIK_NotificationId EIK_UI_AddNotifyMemoryMonitor(FEIK_OnMemoryMonitor Callback);

	//Gets the friends overlay exclusive input state.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_GetFriendsExclusiveInput")
	static bool EIK_UI_GetFriendsExclusiveInput(FEIK_EpicAccountId LocalUserId);

	//Gets the friends overlay visibility.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_GetFriendsVisible")
	static bool EIK_UI_GetFriendsVisible(FEIK_EpicAccountId LocalUserId);
	
	//Returns the current notification location display preference.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_GetNotificationLocationPreference")
	static TEnumAsByte<EEIK_UI_ENotificationLocation> EIK_UI_GetNotificationLocationPreference();

	//Sets the bIsPaused state of the overlay. While true then all notifications will be delayed until after the bIsPaused is false again. While true then the key and button events will not toggle the overlay. If the Overlay was visible before being paused then it will be hidden. If it is known that the Overlay should now be visible after being paused then it will be shown.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_PauseSocialOverlay")
	TEnumAsByte<EEIK_Result> EIK_UI_PauseSocialOverlay(bool bIsPaused);

	//Unregister from receiving notifications when the overlay display settings are updated.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_RemoveNotifyDisplaySettingsUpdated")
	void EIK_UI_RemoveNotifyDisplaySettingsUpdated(FEIK_NotificationId NotificationId);

	//Unregister from receiving notifications when the memory monitor posts a notification.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_RemoveNotifyMemoryMonitor")
	void EIK_UI_RemoveNotifyMemoryMonitor(FEIK_NotificationId NotificationId);

	
	//Define any preferences for any display settings.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | UI Interface", DisplayName="EOS_UI_SetDisplayPreference")
	TEnumAsByte<EEIK_Result> EIK_UI_SetDisplayPreference(const EEIK_UI_ENotificationLocation& DisplayPreference);
};
