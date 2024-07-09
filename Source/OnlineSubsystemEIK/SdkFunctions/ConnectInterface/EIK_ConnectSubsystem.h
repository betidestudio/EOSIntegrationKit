// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/AsyncFunctions/Extra/EIK_BlueprintFunctions.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_ConnectSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthExpirationCallback, FEIK_ProductUserId, LocalUserId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginStatusChangedCallback, FEIK_ProductUserId, LocalUserId, const TEnumAsByte<EIK_ELoginStatus>&, LoginStatus);

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

	//Fetch information about an external account linked to a Product User ID. On a successful call, the caller must release the returned structure using the EOS_Connect_ExternalAccountInfo_Release API.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_CopyProductUserExternalAccountByAccountId")
	TEnumAsByte<EEIK_Result> EIK_Connect_CopyProductUserExternalAccountByAccountId(FEIK_ProductUserId LocalUserId, FString AccountId, FEIK_Connect_ExternalAccountInfo& OutExternalAccountInfo);

	//Fetch information about an external account of a specific type linked to a Product User ID. On a successful call, the caller must release the returned structure using the EOS_Connect_ExternalAccountInfo_Release API.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_CopyProductUserExternalAccountByAccountType")
	TEnumAsByte<EEIK_Result> EIK_Connect_CopyProductUserExternalAccountByAccountType(FEIK_ProductUserId LocalUserId, TEnumAsByte<EEIK_EExternalAccountType> AccountType, FEIK_Connect_ExternalAccountInfo& OutExternalAccountInfo);

	//Fetch information about an external account linked to a Product User ID. On a successful call, the caller must release the returned structure using the EOS_Connect_ExternalAccountInfo_Release API.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_CopyProductUserExternalAccountByIndex")
	TEnumAsByte<EEIK_Result> EIK_Connect_CopyProductUserExternalAccountByIndex(FEIK_ProductUserId LocalUserId, int32 Index, FEIK_Connect_ExternalAccountInfo& OutExternalAccountInfo);

	//Fetch information about a Product User, using the external account that they most recently logged in with as the reference. On a successful call, the caller must release the returned structure using the EOS_Connect_ExternalAccountInfo_Release API.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_CopyProductUserInfo")
	TEnumAsByte<EEIK_Result> EIK_Connect_CopyProductUserInfo(FEIK_ProductUserId LocalUserId, FEIK_Connect_ExternalAccountInfo& OutProductUserInfo);

	//Release the memory associated with an external account info. This must be called on data retrieved from EOS_Connect_CopyProductUserExternalAccountByIndex, EOS_Connect_CopyProductUserExternalAccountByAccountType, EOS_Connect_CopyProductUserExternalAccountByAccountId or EOS_Connect_CopyProductUserInfo.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_ExternalAccountInfo_Release")
	void EIK_Connect_ExternalAccountInfo_Release(FEIK_Connect_ExternalAccountInfo ExternalAccountInfo);

	//Fetch a Product User ID that maps to an external account ID cached from a previous query.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_GetExternalAccountMapping")
	FEIK_ProductUserId EIK_Connect_GetExternalAccountMapping(FEIK_ProductUserId LocalUserId, TEnumAsByte<EEIK_EExternalAccountType> AccountIdType, FString TargetExternalUserId);

	//Fetch a Product User ID that is logged in. This Product User ID is in the Epic Online Services namespace.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_GetLoggedInUserByIndex")
	FEIK_ProductUserId EIK_Connect_GetLoggedInUserByIndex(int32 Index);

	//Fetch the number of product users that are logged in.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_GetLoggedInUsersCount")
	int32 EIK_Connect_GetLoggedInUsersCount();

	//Fetches the login status for an Product User ID. This Product User ID is considered logged in as long as the underlying access token has not expired.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_GetLoginStatus")
	TEnumAsByte<EEIK_LoginStatus> EIK_Connect_GetLoginStatus(FEIK_ProductUserId LocalUserId);

	//Fetch the number of linked external accounts for a Product User ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_GetProductUserExternalAccountCount")
	int32 EIK_Connect_GetProductUserExternalAccountCount(FEIK_ProductUserId LocalUserId);

	//Fetch an external account ID, in string form, that maps to a given Product User ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_GetProductUserIdMapping")
	TEnumAsByte<EEIK_Result> EIK_Connect_GetProductUserIdMapping(FEIK_ProductUserId LocalUserId, TEnumAsByte<EEIK_EExternalAccountType> AccountIdType, FEIK_ProductUserId TargetUserId, FString& OutBuffer);

	//Release the memory associated with an EOS_Connect_IdToken structure. This must be called on data retrieved from EOS_Connect_CopyIdToken.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_IdToken_Release")
	void EIK_Connect_IdToken_Release(FEIK_Connect_IdToken IdToken);

	//Unregister from receiving expiration notifications.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_RemoveNotifyAuthExpiration")
	void EIK_Connect_RemoveNotifyAuthExpiration(FEIK_NotificationId InId);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_RemoveNotifyLoginStatusChanged")
	void EIK_Connect_RemoveNotifyLoginStatusChanged(FEIK_NotificationId InId);

};
