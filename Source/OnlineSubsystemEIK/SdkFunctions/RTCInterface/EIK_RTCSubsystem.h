// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
THIRD_PARTY_INCLUDES_START
#include "eos_rtc.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_RTCSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FEIK_RTC_ParticipantStatusChangedCallbackInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	FEIK_ProductUserId LocalUserId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	FString RoomName;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	FEIK_ProductUserId ParticipantId;

	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	TEnumAsByte<EEIK_ERTCParticipantStatus> ParticipantStatus;

	//The participant metadata items. This is only set for the first notification where ParticipantStatus is EOS_RTCPS_Joined. Subsequent notifications such as when bParticipantInBlocklist changes will not contain any metadata.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	TArray<FEIK_RTC_ParticipantMetadata> ParticipantMetadata;

	//The participant's block list status, if ParticipantStatus is EOS_RTCPS_Joined. This is set to true if the participant is in any of the local user's applicable block lists, such Epic block list or any of the current platform's block lists. It can be used to detect when an internal automatic RTC block is applied because of trust and safety restrictions.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | RTC Interface")
	bool bParticipantInBlocklist;

	FEIK_RTC_ParticipantStatusChangedCallbackInfo()
	{
		ParticipantStatus = EEIK_ERTCParticipantStatus::EIK_RTCPS_Left;
		bParticipantInBlocklist = false;
	}
	FEIK_RTC_ParticipantStatusChangedCallbackInfo(const EOS_RTC_ParticipantStatusChangedCallbackInfo* Data)
	{
		LocalUserId = Data->LocalUserId;
		RoomName = UTF8_TO_TCHAR(Data->RoomName);
		ParticipantId = Data->ParticipantId;
		ParticipantStatus = static_cast<EEIK_ERTCParticipantStatus>(Data->ParticipantStatus);
		bParticipantInBlocklist = Data->bParticipantInBlocklist ? true : false;
		for (uint32_t i = 0; i < Data->ParticipantMetadataCount; i++)
		{
			ParticipantMetadata.Add(Data->ParticipantMetadata[i]);
		}
	}
	
};

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_RTC_OnDisconnectedCallback, const TEnumAsByte<EEIK_Result>, Result, const FEIK_ProductUserId&, LocalUserId, const FString&, RoomName);
DECLARE_DYNAMIC_DELEGATE_OneParam(FEIK_RTC_OnParticipantStatusChangedCallback, const FEIK_RTC_ParticipantStatusChangedCallbackInfo&, Data);
UCLASS(DisplayName = "RTC Interface", meta = (DisplayName = "RTC Interface"))
class ONLINESUBSYSTEMEIK_API UEIK_RTCSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FEIK_RTC_OnDisconnectedCallback OnDisconnectedCallback;
	//Register to receive notifications when disconnected from the room. If the returned NotificationId is valid, you must call EOS_RTC_RemoveNotifyDisconnected when you no longer wish to have your CompletionDelegate called. This function will always return EOS_INVALID_NOTIFICATIONID when used with lobby RTC room. To be notified of the connection status of a Lobby-managed RTC room, use the EOS_Lobby_AddNotifyRTCRoomConnectionChanged function instead.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | RTC Interface", DisplayName="EOS_RTC_AddNotifyDisconnected")
	FEIK_NotificationId EIK_RTC_AddNotifyDisconnected(const FEIK_ProductUserId& LocalUserId, const FString& RoomName, const FEIK_RTC_OnDisconnectedCallback& Callback);

	FEIK_RTC_OnParticipantStatusChangedCallback OnParticipantStatusChangedCallback;
	//Register to receive notifications when a participant's status changes (e.g: join or leave the room), or when the participant is added or removed from an applicable block list (e.g: Epic block list and/or current platform's block list). If the returned NotificationId is valid, you must call EOS_RTC_RemoveNotifyParticipantStatusChanged when you no longer wish to have your CompletionDelegate called. If you register to this notification before joining a room, you will receive a notification for every member already in the room when you join said room. This allows you to know who is already in the room when you join. To be used effectively with a Lobby-managed RTC room, this should be registered during the EOS_Lobby_CreateLobby or EOS_Lobby_JoinLobby completion callbacks when the ResultCode is EOS_Success. If this notification is registered after that point, it is possible to miss notifications for already-existing room participants. You can use this notification to detect internal automatic RTC blocks due to block lists. When a participant joins a room and while the system resolves the block list status of said participant, the participant is set to blocked and you'll receive a notification with ParticipantStatus set to EOS_RTCPS_Joined and bParticipantInBlocklist set to true. Once the block list status is resolved, if the player is not in any applicable block list(s), it is then unblocked and a new notification is sent with ParticipantStatus set to EOS_RTCPS_Joined and bParticipantInBlocklist set to false.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | RTC Interface", DisplayName="EOS_RTC_AddNotifyParticipantStatusChanged")
	FEIK_NotificationId EIK_RTC_AddNotifyParticipantStatusChanged(const FEIK_ProductUserId& LocalUserId, const FString& RoomName, const FEIK_RTC_OnParticipantStatusChangedCallback& Callback);
	
};
