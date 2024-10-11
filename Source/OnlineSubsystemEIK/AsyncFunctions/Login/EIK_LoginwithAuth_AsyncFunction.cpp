// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_LoginwithAuth_AsyncFunction.h"

#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_LoginwithAuth_AsyncFunction* UEIK_LoginwithAuth_AsyncFunction::LoginWithAuth(TEnumAsByte<EEIK_ELoginCredentialType> CredentialType, TEnumAsByte<EEIK_EExternalCredentialType> ExternalCredentialType, FString Id, FString Token)
{
	UEIK_LoginwithAuth_AsyncFunction* UEIK_LoginObject = NewObject<UEIK_LoginwithAuth_AsyncFunction>();
	UEIK_LoginObject->Var_CredentialType = CredentialType;
	UEIK_LoginObject->Var_Id = Id;
	UEIK_LoginObject->Var_Token = Token;
	UEIK_LoginObject->Var_ExternalCredentialType = ExternalCredentialType;
	return UEIK_LoginObject;
}

void UEIK_LoginwithAuth_AsyncFunction::Activate()
{
	Super::Activate();
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			UE_LOG(LogEIK, Verbose, TEXT("LoginUsingAuthInterface: Subsystem and Identity Interface is valid and proceeding with login."));
			FString EGS_Token;
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = Var_Id;
			AccountDetails.Token = Var_Token;
			AccountDetails.Type = "eas_+_" + UEnum::GetValueAsString(Var_CredentialType) + "_+_" + UEnum::GetValueAsString(Var_ExternalCredentialType);
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UEIK_LoginwithAuth_AsyncFunction::LoginCallback);
			IdentityPointerRef->Login(0,AccountDetails);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("LoginUsingAuthInterface: Subsystem or Identity Interface is not valid"));
	OnFailure.Broadcast(FEIK_EpicAccountId(),FEIK_ProductUserId(), "Subsystem or Identity Interface is not valid");
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_LoginwithAuth_AsyncFunction::LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId,
	const FString& Error)
{
	if(bWasSuccess)
	{
		if(UserId.IsValid())
		{
			UE_LOG(LogEIK, Verbose, TEXT("LoginUsingAuthInterface: Login was successful. UserID: %s"), *UserId.ToString());
			FString EpicId, ProductId;
			if(UserId.ToString().Split(TEXT("|"), &EpicId, &ProductId))
			{
				UE_LOG(LogEIK, Verbose, TEXT("LoginUsingAuthInterface: Login was successful. EpicID: %s, ProductID: %s"), *EpicId, *ProductId);
				OnSuccess.Broadcast(EOS_EpicAccountId_FromString(TCHAR_TO_ANSI(*EpicId)), EOS_ProductUserId_FromString(TCHAR_TO_ANSI(*ProductId)), "");
			}
			else
			{
				UE_LOG(LogEIK, Error, TEXT("LoginUsingAuthInterface: Login was successful but UserID is not valid"));
				OnFailure.Broadcast(FEIK_EpicAccountId(), FEIK_ProductUserId(), "UserID is not valid");
			}
		}
		else
		{
			UE_LOG(LogEIK, Error, TEXT("LoginUsingAuthInterface: Login was successful but UserID is not valid"));
			OnFailure.Broadcast(FEIK_EpicAccountId(), FEIK_ProductUserId(), "UserID is not valid");
		}
	}
	else
	{
		UE_LOG(LogEIK, Error, TEXT("LoginUsingAuthInterface: Login failed. Error: %s"), *Error);
		OnFailure.Broadcast(FEIK_EpicAccountId(), FEIK_ProductUserId(), Error);
	}
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
