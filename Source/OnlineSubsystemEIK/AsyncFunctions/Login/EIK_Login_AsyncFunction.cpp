// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_Login_AsyncFunction.h"

#include "EIKSettings.h"
#include "Online.h"
#include "Misc/CommandLine.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"


UEIK_Login_AsyncFunction* UEIK_Login_AsyncFunction::LoginUsingEIK(ELoginTypes LoginMethod, FString DisplayName, FString Token)
{
	UEIK_Login_AsyncFunction* UEIK_LoginObject= NewObject<UEIK_Login_AsyncFunction>();
	UEIK_LoginObject->Input1 = DisplayName;
	UEIK_LoginObject->Input2 = Token;
	UEIK_LoginObject->LoginMethod = LoginMethod;
	return UEIK_LoginObject;
}

void UEIK_Login_AsyncFunction::Activate()
{
	Login();
	Super::Activate();
}

void UEIK_Login_AsyncFunction::Login()
{
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			FString EGS_Token;
			FOnlineAccountCredentials AccountDetails;
			switch (LoginMethod)
			{
			case ELoginTypes::AccountPortal:
				AccountDetails.Id = "";
				AccountDetails.Token = "";
				AccountDetails.Type = "accountportal";
				break;
			case ELoginTypes::PersistentAuth:
				AccountDetails.Id = "";
				AccountDetails.Token = "";
				AccountDetails.Type = "persistentauth";
				break;
			case ELoginTypes::EpicLauncher:
				FParse::Value(FCommandLine::Get(), TEXT("AUTH_PASSWORD="), EGS_Token);
				AccountDetails.Id = "";
				AccountDetails.Token = EGS_Token;
				AccountDetails.Type = "exchangecode";
				break;
			case ELoginTypes::Steam:
				AccountDetails.Id = Input1;
				AccountDetails.Token = Input2;
				AccountDetails.Type = "steam";
				break;
			case ELoginTypes::DeviceID:
				if(UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
				{
					if(EIKSettings->bShowAdvancedLogs)
					{
						if(EIKSettings->bEnableOverlay || EIKSettings->bEnableEditorOverlay || EIKSettings->bEnableSocialOverlay)
						{
							UE_LOG(LogTemp, Error, TEXT("EIK: Device ID Login is not supported with Overlay Enabled. Please disable Overlay to use this login type."));
						}
					}
				}
				AccountDetails.Id = Input1;
				AccountDetails.Token = Input2;
				AccountDetails.Type = "deviceid";
				break;
			case ELoginTypes::Developer:
				AccountDetails.Id = Input1;
				AccountDetails.Token = Input2;
				AccountDetails.Type = "developer";
				break;
			case ELoginTypes::OpenID:
				AccountDetails.Id = Input1;
				AccountDetails.Token = Input2;
				AccountDetails.Type = "openid";
				break;
			case ELoginTypes::Apple:
				LoginWithApple();
				return;
			case ELoginTypes::Google:
				AccountDetails.Id = Input1;
				AccountDetails.Token = Input2;
				AccountDetails.Type = "google";
				break;
			default:
				if(UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
				{
					if(EIKSettings->bShowAdvancedLogs)
					{
							UE_LOG(LogTemp, Error, TEXT("EIK: Invalid Login Type. Please use a valid Login Type."));
					}
				}
			}
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UEIK_Login_AsyncFunction::LoginCallback);
			IdentityPointerRef->Login(0,AccountDetails);
		}
		else
		{
			if(bDelegateCalled == false)
			{
				OnFail.Broadcast("", "Failed to get Identity Pointer Ref");
				SetReadyToDestroy();
MarkAsGarbage();
				bDelegateCalled = true;
			}
		}
	}
	else
	{
		if(bDelegateCalled == false)
		{
			OnFail.Broadcast("", "Failed to get Subsystem");
			SetReadyToDestroy();
MarkAsGarbage();
			bDelegateCalled = true;
		}
	}
}

void UEIK_Login_AsyncFunction::LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId,
	const FString& Error)
{
	if(bDelegateCalled == true)
	{
		return;
	}
	if(bWasSuccess)
	{
		if(UserId.IsValid())
		{
			OnSuccess.Broadcast(UserId.ToString(), "");
			SetReadyToDestroy();
			MarkAsGarbage();
			bDelegateCalled = true;
		}
		else
		{
			OnFail.Broadcast("", "UserID is not valid");
			SetReadyToDestroy();
			MarkAsGarbage();
			bDelegateCalled = true;
		}
	}
	else
	{
		OnFail.Broadcast("", Error);
		SetReadyToDestroy();
		MarkAsGarbage();
		bDelegateCalled = true;
	}
}

void UEIK_Login_AsyncFunction::LoginWithAppleCallback(TSharedPtr<const FUniqueNetId> UniqueNetId, int I,
	const FOnlineError& OnlineError)
{
	if(OnlineError.WasSuccessful())
	{
		if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				UE_LOG(LogTemp, Warning, TEXT("EIK: Apple Login Successful - %s"), *UniqueNetId->ToString());
				FOnlineAccountCredentials AccountDetails;
				AccountDetails.Id = UniqueNetId->ToString();
				AccountDetails.Type = "apple";
				AccountDetails.Token = AppleSubsystem->GetIdentityInterface()->GetAuthToken(0);
				IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UEIK_Login_AsyncFunction::LoginCallback);
				IdentityPointerRef->Login(0,AccountDetails);
			}
		}
	}
	else
	{
		OnFail.Broadcast("", OnlineError.GetErrorCode());
		SetReadyToDestroy();
MarkAsGarbage();
		bDelegateCalled = true;
	}
}

void UEIK_Login_AsyncFunction::LoginWithApple()
{
	if(const IOnlineSubsystem *AppleSubsystemRef = Online::GetSubsystem(GetWorld(),APPLE_SUBSYSTEM))
	{
		if(const IOnlineExternalUIPtr AppleExternalUIPointerRef = AppleSubsystemRef->GetExternalUIInterface())
		{
			FOnLoginUIClosedDelegate OnLoginUIClosedDelegate = FOnLoginUIClosedDelegate::CreateUObject(this,&UEIK_Login_AsyncFunction::LoginWithAppleCallback);
			AppleExternalUIPointerRef->ShowLoginUI(0, false, false, OnLoginUIClosedDelegate);
		}
		else
		{
			if(bDelegateCalled == false)
			{
				OnFail.Broadcast("", "Failed to get External UI Pointer Ref");
				SetReadyToDestroy();
MarkAsGarbage();
				bDelegateCalled = true;
			}
		}
	}
}
