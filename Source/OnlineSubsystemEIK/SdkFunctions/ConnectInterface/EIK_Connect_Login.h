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
		EOS_Connect_UserLoginInfo Result = {};
		Result.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
		if(DisplayName.Len() > 0)
		{
			FTCHARToUTF8 DisplayNameConverter(*DisplayName);
			CachedDisplayNameUtf8.SetNumUninitialized(DisplayNameConverter.Length() + 1);
			FMemory::Memcpy(CachedDisplayNameUtf8.GetData(), DisplayNameConverter.Get(), DisplayNameConverter.Length() + 1);
			Result.DisplayName = CachedDisplayNameUtf8.GetData();
		}
		else
		{
			CachedDisplayNameUtf8.Reset();
			Result.DisplayName = nullptr;
		}
		if(NsaTokenId.Len() > 0)
		{
			FTCHARToUTF8 NsaTokenConverter(*NsaTokenId);
			CachedNsaTokenUtf8.SetNumUninitialized(NsaTokenConverter.Length() + 1);
			FMemory::Memcpy(CachedNsaTokenUtf8.GetData(), NsaTokenConverter.Get(), NsaTokenConverter.Length() + 1);
			Result.NsaIdToken = CachedNsaTokenUtf8.GetData();
		}
		else
		{
			CachedNsaTokenUtf8.Reset();
			Result.NsaIdToken = nullptr;
		}
		return Result;
	};

private:
	TArray<ANSICHAR> CachedDisplayNameUtf8;
	TArray<ANSICHAR> CachedNsaTokenUtf8;
	
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
		EOS_Connect_Credentials Result = {};
		Result.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		if(Token.Len() > 0)
		{
			FTCHARToUTF8 TokenConverter(*Token);
			CachedTokenUtf8.SetNumUninitialized(TokenConverter.Length() + 1);
			FMemory::Memcpy(CachedTokenUtf8.GetData(), TokenConverter.Get(), TokenConverter.Length() + 1);
			Result.Token = CachedTokenUtf8.GetData();
		}
		else
		{
			CachedTokenUtf8.Reset();
			Result.Token = nullptr;
		}
		Result.Type = static_cast<EOS_EExternalCredentialType>(Type.GetValue());
		return Result;
	
	};

private:
	TArray<ANSICHAR> CachedTokenUtf8;
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
