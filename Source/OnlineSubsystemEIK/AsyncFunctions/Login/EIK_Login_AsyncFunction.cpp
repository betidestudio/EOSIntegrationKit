// Fill out your copyright notice in the Description page of Project Settings.


#include "EIK_Login_AsyncFunction.h"

#include "OnlineSubsystem.h"
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
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
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
			case ELoginTypes::DeviceID:
				AccountDetails.Id = Input1;
				AccountDetails.Token = Input2;
				AccountDetails.Type = "deviceid";
				break;
			case ELoginTypes::Google: break;
			case ELoginTypes::Apple: break;
			case ELoginTypes::Discord: break;
			case ELoginTypes::Oculus: break;
			case ELoginTypes::OpenID: break;
			case ELoginTypes::Developer: break;
			default: ;
			}

			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UEIK_Login_AsyncFunction::LoginCallback);
			IdentityPointerRef->Login(0,AccountDetails);
		}
		else
		{
			if(bDelegateCalled == false)
			{
				OnFail.Broadcast("", "Failed to get Identity Pointer Ref");
				bDelegateCalled = true;
			}
		}
	}
	else
	{
		if(bDelegateCalled == false)
		{
			OnFail.Broadcast("", "Failed to get Subsystem");
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
			bDelegateCalled = true;
		}
		else
		{
			OnFail.Broadcast("", "UserID is not valid");
			bDelegateCalled = true;
		}
	}
	else
	{
		OnFail.Broadcast("", Error);
		bDelegateCalled = true;
	}
}
