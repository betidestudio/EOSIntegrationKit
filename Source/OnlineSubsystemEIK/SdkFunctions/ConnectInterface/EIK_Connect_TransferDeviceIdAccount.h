// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Connect_TransferDeviceIdAccount.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Connect_TransferDeviceIdAccount_Delegate, const FEIK_ProductUserId&, LocalUserId, TEnumAsByte<EEIK_Result>, Result);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Connect_TransferDeviceIdAccount : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	// Transfer a Device ID pseudo-account and the product user associated with it into another keychain linked with real user accounts (such as Epic Games, PlayStation(TM)Network, Xbox Live, and other). This function allows transferring a product user, i.e. the local user's game progression backend data from a Device ID owned keychain into a keychain with real user accounts linked to it. The transfer of Device ID owned product user into a keychain of real user accounts allows persisting the user's game data on the backend in the event that they would lose access to the local device or otherwise switch to another device or platform. This function is only applicable in the situation of where the local user first plays the game using the anonymous Device ID login, then later logs in using a real user account that they have also already used to play the same game or another game under the same organization within Epic Online Services. In such situation, while normally the login attempt with a real user account would return EOS_InvalidUser and an EOS_ContinuanceToken and allow calling the EOS_Connect_LinkAccount API to link it with the Device ID's keychain, instead the login operation succeeds and finds an existing user because the association already exists. Because the user cannot have two product users simultaneously to play with, the game should prompt the user to choose which profile to keep and which one to discard permanently. Based on the user choice, the game may then proceed to transfer the Device ID login into the keychain that is persistent and backed by real user accounts, and if the user chooses so, move the product user as well into the destination keychain and overwrite the existing previous product user with it. To clarify, moving the product user with the Device ID login in this way into a persisted keychain allows to preserve the so far only locally persisted game progression and thus protect the user against a case where they lose access to the device. On success, the completion callback will return the preserved EOS_ProductUserId that remains logged in while the discarded EOS_ProductUserId has been invalidated and deleted permanently. Consecutive logins using the existing Device ID login type or the external account will connect the user to the same backend data belonging to the preserved EOS_ProductUserId. Example walkthrough: Cross-platform mobile game using the anonymous Device ID login. For onboarding new users, the game will attempt to always automatically login the local user by calling EOS_Connect_Login using the EOS_ECT_DEVICEID_ACCESS_TOKEN login type. If the local Device ID credentials are not found, and the game wants a frictionless entry for the first time user experience, the game will automatically call EOS_Connect_CreateDeviceId to create new Device ID pseudo-account and then login the local user into it. Consecutive game starts will thus automatically login the user to their locally persisted Device ID account. The user starts playing anonymously using the Device ID login type and makes significant game progress. Later, they login using an external account that they have already used previously for the same game perhaps on another platform, or another game owned by the same organization. In such case, EOS_Connect_Login will automatically login the user to their existing account linking keychain and create automatically a new empty product user for this product. In order for the user to use their existing previously created keychain and have the locally created Device ID login reference to that keychain instead, the user's current product user needs to be moved to be under that keychain so that their existing game progression will be preserved. To do so, the game can call EOS_Connect_TransferDeviceIdAccount to transfer the Device ID login and the product user associated with it into the other keychain that has real external user account(s) linked to it. Note that it is important that the game either automatically checks that the other product user does not have any meaningful progression data, or otherwise will prompt the user to make the choice on which game progression to preserve and which can be discarded permanently. The other product user will be discarded permanently and cannot be recovered, so it is very important that the user is guided to make the right choice to avoid accidental loss of all game progression.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_TransferDeviceIdAccount")
	static UEIK_Connect_TransferDeviceIdAccount* EIK_Connect_TransferDeviceIdAccount(FEIK_ProductUserId PrimaryLocalUserId, FEIK_ProductUserId LocalUserId, FEIK_ProductUserId ProductUserIdToPreserve);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_Connect_TransferDeviceIdAccount_Delegate OnCallback;
private:
	static void OnTransferDeviceIdAccountCallback(const EOS_Connect_TransferDeviceIdAccountCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_ProductUserId Var_PrimaryLocalUserId;
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_ProductUserId Var_ProductUserIdToPreserve;
};
