// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_ConnectSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEIK, Log, All);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthExpirationCallback, FEIK_ProductUserId, LocalUserId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginStatusChangedCallback, FEIK_ProductUserId, LocalUserId, TEnumAsByte<EIK_ELoginStatus>, LoginStatus);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_ConnectSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	FOnAuthExpirationCallback OnAuthExpiration;
	FOnLoginStatusChangedCallback OnLoginStatusChanged;

	//Register to receive upcoming authentication expiration notifications. Notification is approximately 10 minutes prior to expiration. Call EOS_Connect_Login again with valid third party credentials to refresh access.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_AddNotifyAuthExpiration")
	FEIK_NotificationId EIK_Connect_AddNotifyAuthExpiration(const FOnAuthExpirationCallback& Callback);

	//Register to receive user login status updates.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_AddNotifyLoginStatusChanged")
	FEIK_NotificationId EIK_Connect_AddNotifyLoginStatusChanged(const FOnLoginStatusChangedCallback& Callback);

	//Fetches an ID token for a Product User ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_CopyIdToken")
	TEnumAsByte<EEIK_Result> EIK_Connect_CopyIdToken(FEIK_ProductUserId LocalUserId, FEIK_Connect_IdToken& OutIdToken);


};
