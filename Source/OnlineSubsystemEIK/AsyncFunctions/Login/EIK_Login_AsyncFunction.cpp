//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Login_AsyncFunction.h"

#include "EIKSettings.h"
#include "Online.h"
#include "Misc/CommandLine.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"


UEIK_Login_AsyncFunction* UEIK_Login_AsyncFunction::LoginUsingEIK(ELoginTypes LoginMethod, FString Input1,
                                                                  FString Input2)
{
	UEIK_Login_AsyncFunction* UEIK_LoginObject= NewObject<UEIK_Login_AsyncFunction>();
	UEIK_LoginObject->Input1 = Input1;
	UEIK_LoginObject->Input2 = Input2;
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
			bDelegateCalled = true;
		}
		else
		{
			OnFail.Broadcast("", "UserID is not valid");
			SetReadyToDestroy();
			bDelegateCalled = true;
		}
	}
	else
	{
		OnFail.Broadcast("", Error);
		SetReadyToDestroy();
		bDelegateCalled = true;
	}
}
