// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_base.h"
#include "eos_connect_types.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_CreateDeviceId_AsyncFunction.generated.h"


/** The result of a create device id request
 * @param Result The result of the create device id request
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateDeviceIdComplete, const TEnumAsByte<EEIK_Result>&, Result);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CreateDeviceId_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/*
	Create a new unique pseudo-account that can be used to identify the current user profile on the local device.

	This function is intended to be used by mobile games and PC games that wish to allow a new user to start playing without requiring to login to the game using any user identity.

	In addition to this, the Device ID feature is used to automatically login the local user also when they have linked at least one external user account(s) with the local Device ID.

	It is possible to link many devices with the same user's account keyring using the Device ID feature.

	Linking a device later or immediately with a real user account will ensure that the player will not lose their progress if they switch devices or lose the device at some point, as they will be always able to login with one of their linked real accounts and also link another new device with the user account associations keychain.

	Otherwise, without having at least one permanent user account linked to the Device ID, the player would lose all of their game data and progression permanently should something happen to their device or the local user profile on the device.

	After a successful one-time CreateDeviceId operation, the game can login the local user automatically on subsequent game starts with EOS_Connect_Login using the EOS_ECT_DEVICEID_ACCESS_TOKEN credentials type.

	If a Device ID already exists for the local user on the device then EOS_DuplicateNotAllowed error result is returned and the caller should proceed to calling EOS_Connect_Login directly.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_CreateDeviceId")
	static UEIK_CreateDeviceId_AsyncFunction* CreateDeviceId(FString DeviceModel);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FOnCreateDeviceIdComplete OnCallback;
	
private:
	virtual void Activate() override;
	FString Var_DeviceModel;
	static void EOS_CALL OnCreateDeviceIdCallback(const EOS_Connect_CreateDeviceIdCallbackInfo* Data);
		
};
