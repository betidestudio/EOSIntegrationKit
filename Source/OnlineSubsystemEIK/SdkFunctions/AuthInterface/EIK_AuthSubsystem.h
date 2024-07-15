// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_auth.h"
THIRD_PARTY_INCLUDES_END
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_AuthSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_Auth_OnLoginStatusChangedCallback, FEIK_EpicAccountId, LocalUserId, const TEnumAsByte<EIK_ELoginStatus>&, PrevStatus, const TEnumAsByte<EIK_ELoginStatus>&, CurrentStatus);
UCLASS(meta=(DisplayName="Auth Interface"), Category="EOS Integration Kit", DisplayName="Auth Interface")
class ONLINESUBSYSTEMEIK_API UEIK_AuthSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FEIK_Auth_OnLoginStatusChangedCallback OnLoginStatusChanged;
	/*
	 Register to receive login status updates.
	 *
	 If the returned NotificationId is valid, you must call EOS_Auth_RemoveNotifyLoginStatusChanged when you no longer wish to have your NotificationHandler called.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_AddNotifyLoginStatusChanged")
	FEIK_NotificationId EIK_Auth_AddNotifyLoginStatusChanged(const FEIK_Auth_OnLoginStatusChangedCallback& Callback);

	//Fetch an ID token for an Epic Account ID. ID tokens are used to securely verify user identities with online services. The most common use case is using an ID token to authenticate the local user by their selected account ID, which is the account ID that should be used to access any game-scoped data for the current application. An ID token for the selected account ID of a locally authenticated user will always be readily available. To retrieve it for the selected account ID, you can use EOS_Auth_CopyIdToken directly after a successful user login.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_CopyIdToken")
	TEnumAsByte<EEIK_Result> EIK_Auth_CopyIdToken(FEIK_EpicAccountId AccountId, FEIK_Auth_IdToken& OutToken);

	//Fetch a user auth token for an Epic Account ID. A user authentication token allows any code with possession (backend/client) to perform certain actions on behalf of the user. Because of this, for the purposes of user identity verification, the EOS_Auth_CopyIdToken API should be used instead.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_CopyUserAuthToken")
	TEnumAsByte<EEIK_Result> EIK_Auth_CopyUserAuthToken(FEIK_EpicAccountId LocalUserId, FEIK_Auth_Token& OutToken);

	//Fetch an Epic Account ID that is logged in.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_GetLoggedInAccountByIndex")
	FEIK_EpicAccountId EIK_Auth_GetLoggedInAccountByIndex(int32 Index);

	//Fetch the number of accounts that are logged in.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_GetLoggedInAccountsCount")
	int32 EIK_Auth_GetLoggedInAccountsCount();
	
	//Fetches the login status for an Epic Account ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_GetLoginStatus")
	TEnumAsByte<EIK_ELoginStatus> EIK_Auth_GetLoginStatus(FEIK_EpicAccountId LocalUserId);

	//Fetch one of the merged account IDs for a given logged in account.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_GetMergedAccountByIndex")
	FEIK_EpicAccountId EIK_Auth_GetMergedAccountByIndex(FEIK_EpicAccountId LocalUserId, int32 Index);

	//Fetch the number of merged accounts for a given logged in account.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_GetMergedAccountsCount")
	int32 EIK_Auth_GetMergedAccountsCount(FEIK_EpicAccountId LocalUserId);

	//Fetch the selected account ID to the current application for a local authenticated user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_GetSelectedAccountId")
	TEnumAsByte<EEIK_Result> EIK_Auth_GetSelectedAccountId(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId& OutSelectedAccountId);

	//Release the memory associated with an EOS_Auth_IdToken structure. This must be called on data retrieved from EOS_Auth_CopyIdToken.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_IdToken_Release")
	void EIK_Auth_IdToken_Release(FEIK_Auth_IdToken& Token);

	//Unregister from receiving login status updates.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_RemoveNotifyLoginStatusChanged")
	void EIK_Auth_RemoveNotifyLoginStatusChanged(FEIK_NotificationId NotificationId);

	//Release the memory associated with an EOS_Auth_Token structure. This must be called on data retrieved from EOS_Auth_CopyUserAuthToken.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName="EOS_Auth_Token_Release")
	void EIK_Auth_Token_Release(FEIK_Auth_Token& Token);
};
