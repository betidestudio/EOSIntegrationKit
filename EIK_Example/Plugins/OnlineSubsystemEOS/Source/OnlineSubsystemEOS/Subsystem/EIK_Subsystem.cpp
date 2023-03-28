// Fill out your copyright notice in the Description page of Project Settings.


#include "EIK_Subsystem.h"

void UEIK_Subsystem::LoginWithDeviceID(int32 LocalUserNum, FString DisplayName, FString DeviceName,
	const FBP_Login_Callback& Result)
{
	LoginWithDeviceID_CallBackBP = Result;
	const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if(IdentityPointerRef)
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = DisplayName;
			AccountDetails.Token = DeviceName;
			AccountDetails.Type = "DeviceID";
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UEIK_Subsystem::LoginWithDeviceIDCallback);
			IdentityPointerRef->Login(LocalUserNum,AccountDetails);
		}
		else
		{
			Result.Execute(false,FBPUniqueNetId(),"Failed to get Identity Pointer");
		}
	}
	else
	{
		Result.Execute(false,FBPUniqueNetId(),"Failed to get Subsystem");
	}
}

void UEIK_Subsystem::LoginWithDeviceIDCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId,
	const FString& Error) const
{
	// Convert the FUniqueNetId object to a string
	FBPUniqueNetId UniqueNetId_BP;
	UniqueNetId_BP.SetUniqueNetId(UserId.AsShared());
	
	LoginWithDeviceID_CallBackBP.Execute(bWasSuccess,UniqueNetId_BP,Error);
}
