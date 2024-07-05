// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_connect_types.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Connect_Login.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Connect_UserLoginInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FString NsaTokenId;

	FEIK_Connect_UserLoginInfo()
	{
		DisplayName = "";
	}

	FEIK_Connect_UserLoginInfo(EOS_Connect_UserLoginInfo UserLoginInfo)
	{
		if(UserLoginInfo.DisplayName != nullptr)
		{
			DisplayName = UserLoginInfo.DisplayName;
		}
		if(UserLoginInfo.NsaIdToken != nullptr)
		{
			NsaTokenId = UserLoginInfo.NsaIdToken;
		}
	}
	EOS_Connect_UserLoginInfo ToEOSConnectUserLoginInfo()
	{
		EOS_Connect_UserLoginInfo Result;
		Result.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
		if(DisplayName.Len() > 0)
		{
			Result.DisplayName = TCHAR_TO_ANSI(*DisplayName);
		}
		else
		{
			Result.DisplayName = nullptr;
		}
		if(NsaTokenId.Len() > 0)
		{
			Result.NsaIdToken = TCHAR_TO_ANSI(*NsaTokenId);
		}
		else
		{
			Result.NsaIdToken = nullptr;
		}
		return Result;
	};
	
};

USTRUCT(BlueprintType)
struct FEIK_Connect_Credentials
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	FString Token;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Connect Interface")
	TEnumAsByte<EEIK_EExternalCredentialType> Type;

	FEIK_Connect_Credentials()
	{
		Token = "";
		Type = EEIK_EExternalCredentialType::EIK_ECT_EPIC;
	}

	FEIK_Connect_Credentials(EOS_Connect_Credentials Credentials)
	{
		Token = Credentials.Token;
		Type = static_cast<EEIK_EExternalCredentialType>(Credentials.Type);
	}

	EOS_Connect_Credentials ToEOSConnectCredentials()
	{
		EOS_Connect_Credentials Result;
		Result.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		if(Token.Len() > 0)
		{
			Result.Token = TCHAR_TO_ANSI(*Token);
		}
		else
		{
			Result.Token = nullptr;
		}
		Result.Type = static_cast<EOS_EExternalCredentialType>(Type.GetValue());
		return Result;
	
	};
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLoginCallback, TEnumAsByte<EEIK_Result>, ResultCode, FEIK_ProductUserId, LocalUserId, FEIK_ContinuanceToken, ContinuanceToken);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Connect_Login : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	
	/*
	  Login/Authenticate given a valid set of external auth credentials.
	  @param Credentials - Credentials specified for a given login method
	  @param UserLoginInfo - Additional information about the local user. This field is required to be set and used when authenticating the user using Amazon, Apple, Google, Nintendo Account, Nintendo Service Account, Oculus or the Device ID feature login. It is also required for using the Lobbies and Sessions interfaces when running on the Nintendo Switch device, and using any other credential type than EOS_ECT_NINTENDO_NSA_ID_TOKEN. In all other cases, set this field to NULL.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Connect Interface", DisplayName="EOS_Connect_Login")
	static UEIK_Connect_Login* EIK_Connect_Login(FEIK_Connect_Credentials Credentials, FEIK_Connect_UserLoginInfo UserLoginInfo);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FOnLoginCallback OnCallback;
private:
	static void OnLoginCallback(const EOS_Connect_LoginCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_Connect_Credentials Local_Credentials;
	FEIK_Connect_UserLoginInfo Local_UserLoginInfo;
};
