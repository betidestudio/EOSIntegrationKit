// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_PresenceSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_Presence_JoinGameAcceptedCallbackInfo, const FString&, JoinInfo, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId, const FEIK_UI_EventId&, UiEventId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIK_Presence_OnPresenceChangedCallbackInfo, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, PresenceUserId);

UCLASS(DisplayName="Presense Interface", DisplayName="Presense Interface")
class ONLINESUBSYSTEMEIK_API UEIK_PresenceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	FEIK_Presence_JoinGameAcceptedCallbackInfo JoinGameAcceptedCallbackInfo;
	//Register to receive notifications when a user accepts a join game option via the social overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_AddNotifyJoinGameAccepted")
	FEIK_NotificationId EIK_Presence_AddNotifyJoinGameAccepted(const FEIK_Presence_JoinGameAcceptedCallbackInfo& Callback);


	FEIK_Presence_OnPresenceChangedCallbackInfo PresenceChangedCallbackInfo;
	//Register to receive notifications when presence changes.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_AddNotifyOnPresenceChanged")
	FEIK_NotificationId EIK_Presence_AddNotifyOnPresenceChanged(const FEIK_Presence_OnPresenceChangedCallbackInfo& Callback);

	//Get a user's cached presence object. If successful, this data must be released by calling EOS_Presence_Info_Release
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_CopyPresence")
	TEnumAsByte<EEIK_Result> EIK_Presence_CopyPresence(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId, FEIK_Presence_Info& OutPresence);


	//Creates a presence modification handle. This handle can used to add multiple changes to your presence that can be applied with EOS_Presence_SetPresence. The resulting handle must be released by calling EOS_PresenceModification_Release once it has been passed to EOS_Presence_SetPresence.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_CreatePresenceModification")
	TEnumAsByte<EEIK_Result> EIK_Presence_CreatePresenceModification(FEIK_EpicAccountId LocalUserId, FEIK_HPresenceModification& OutPresenceModificationHandle);

	//Gets a join info custom game-data string for a specific user. This is a helper function for reading the presence data related to how a user can be joined. Its meaning is entirely application dependent. This value will be valid only after a QueryPresence call has successfully completed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_GetJoinInfo")
	TEnumAsByte<EEIK_Result> EIK_Presence_GetJoinInfo(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId, FString& OutBuffer);

	//Check if we already have presence for a user
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_HasPresence")
	bool EIK_Presence_HasPresence(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId);

	//Release the memory associated with an EOS_Presence_Info structure and its sub-objects. This must be called on data retrieved from EOS_Presence_CopyPresence. This can be safely called on a NULL presence info object.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_Info_Release")
	void EIK_Presence_Info_Release(const FEIK_Presence_Info& PresenceInfo);

	//Unregister from receiving notifications when a user accepts a join game option via the social overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_RemoveNotifyJoinGameAccepted")
	void EIK_Presence_RemoveNotifyJoinGameAccepted(FEIK_NotificationId InId);

	//Unregister a previously bound notification handler from receiving presence update notifications
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_RemoveNotifyOnPresenceChanged")
	void EIK_Presence_RemoveNotifyOnPresenceChanged(FEIK_NotificationId InId);

	//Removes one or more rows of user-defined presence data for a local user. At least one DeleteDataInfo object must be specified.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_DeleteData")
	TEnumAsByte<EEIK_Result> EIK_PresenceModification_DeleteData(FEIK_HPresenceModification PresenceModificationHandle, const FEIK_PresenceModification_DataRecordId& Data, int32 Count);

	//Release the memory associated with an EOS_HPresenceModification handle. This must be called on Handles retrieved from EOS_Presence_CreatePresenceModification. This can be safely called on a NULL presence modification handle. This also may be safely called while a call to SetPresence is still pending.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_Release")
	void EIK_PresenceModification_Release(FEIK_HPresenceModification PresenceModificationHandle);

	//Modifies one or more rows of user-defined presence data for a local user. At least one InfoData object must be specified.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_SetData")
	TEnumAsByte<EEIK_Result> EIK_PresenceModification_SetData(FEIK_HPresenceModification PresenceModificationHandle, const FEIK_Presence_DataRecord& Data, int32 Count);

	//Sets your new join info custom game-data string. This is a helper function for reading the presence data related to how a user can be joined. Its meaning is entirely application dependent.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_SetJoinInfo")
	TEnumAsByte<EEIK_Result> EIK_PresenceModification_SetJoinInfo(FEIK_HPresenceModification PresenceModificationHandle, const FString& JoinInfo);

	//Modifies a user's Rich Presence string to a new state. This is the exact value other users will see when they query the local user's presence.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_SetRawRichText")
	TEnumAsByte<EEIK_Result> EIK_PresenceModification_SetRawRichText(FEIK_HPresenceModification PresenceModificationHandle, const FString& RichText);

	//Modifies a user's online status to be the new state.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_PresenceModification_SetStatus")
	TEnumAsByte<EEIK_Result> EIK_PresenceModification_SetStatus(FEIK_HPresenceModification PresenceModificationHandle, TEnumAsByte<EEIK_Presence_EStatus> Status);
};
