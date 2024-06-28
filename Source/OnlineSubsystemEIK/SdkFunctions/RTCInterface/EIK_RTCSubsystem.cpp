// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_RTCSubsystem.h"

#include "OnlineSubsystemEOS.h"

FEIK_NotificationId UEIK_RTCSubsystem::EIK_RTC_AddNotifyDisconnected(const FEIK_ProductUserId& LocalUserId,
                                                                     const FString& RoomName, const FEIK_RTC_OnDisconnectedCallback& Callback)
{
	OnDisconnectedCallback = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_RTC_AddNotifyDisconnectedOptions Options = { };
			Options.ApiVersion = EOS_RTC_ADDNOTIFYDISCONNECTED_API_LATEST;
			Options.LocalUserId = LocalUserId.ProductUserIdBasic;
			Options.RoomName = TCHAR_TO_ANSI(*RoomName);
			return EOS_RTC_AddNotifyDisconnected(EOS_Platform_GetRTCInterface(EOSRef->EOSPlatformHandle->PlatformHandle), &Options, this, [](const EOS_RTC_DisconnectedCallbackInfo* Data)
			{
				UEIK_RTCSubsystem* Subsystem = static_cast<UEIK_RTCSubsystem*>(Data->ClientData);
				if (Subsystem)
				{
					Subsystem->OnDisconnectedCallback.ExecuteIfBound(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, UTF8_TO_TCHAR(Data->RoomName));
				}
			});
		}
	}
	return FEIK_NotificationId();
}

FEIK_NotificationId UEIK_RTCSubsystem::EIK_RTC_AddNotifyParticipantStatusChanged(const FEIK_ProductUserId& LocalUserId,
	const FString& RoomName, const FEIK_RTC_OnParticipantStatusChangedCallback& Callback)
{
	OnParticipantStatusChangedCallback = Callback;
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_RTC_AddNotifyParticipantStatusChangedOptions Options = { };
			Options.ApiVersion = EOS_RTC_ADDNOTIFYPARTICIPANTSTATUSCHANGED_API_LATEST;
			Options.LocalUserId = LocalUserId.ProductUserIdBasic;
			Options.RoomName = TCHAR_TO_ANSI(*RoomName);
			return EOS_RTC_AddNotifyParticipantStatusChanged(EOS_Platform_GetRTCInterface(EOSRef->EOSPlatformHandle->PlatformHandle), &Options, this, [](const EOS_RTC_ParticipantStatusChangedCallbackInfo* Data)
			{
				UEIK_RTCSubsystem* Subsystem = static_cast<UEIK_RTCSubsystem*>(Data->ClientData);
				if (Subsystem)
				{
					FEIK_RTC_ParticipantStatusChangedCallbackInfo CallbackInfo(Data);
					Subsystem->OnParticipantStatusChangedCallback.ExecuteIfBound(CallbackInfo);
				}
			});
		}
	}
	return FEIK_NotificationId();
}
