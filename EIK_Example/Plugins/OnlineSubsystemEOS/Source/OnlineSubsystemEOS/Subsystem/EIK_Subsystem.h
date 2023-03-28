// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "FindSessionsCallbackProxy.h"
#include "VoiceChat.h"
#include "OnlineSubsystem.h"
#include "EIK_Subsystem.generated.h"



//Love you VaRest for this :D

USTRUCT(BlueprintType)
struct FBPUniqueNetId
{
	GENERATED_USTRUCT_BODY()

private:
	bool bUseDirectPointer;


public:
	TSharedPtr<const FUniqueNetId> UniqueNetId;
	const FUniqueNetId * UniqueNetIdPtr;

	void SetUniqueNetId(const TSharedPtr<const FUniqueNetId> &ID)
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
		UniqueNetId = ID;
	}

	void SetUniqueNetId(const FUniqueNetId *ID)
	{
		bUseDirectPointer = true;
		UniqueNetIdPtr = ID;
	}

	bool IsValid() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr && UniqueNetIdPtr->IsValid())
		{
			return true;
		}
		else if (UniqueNetId.IsValid())
		{
			return true;
		}
		else
			return false;

	}

	const FUniqueNetId* GetUniqueNetId() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr)
		{
			// No longer converting to non const as all functions now pass const UniqueNetIds
			return /*const_cast<FUniqueNetId*>*/(UniqueNetIdPtr);
		}
		else if (UniqueNetId.IsValid())
		{
			return UniqueNetId.Get();
		}
		else
			return nullptr;
	}

	// Adding in a compare operator so that std functions will work with this struct
	FORCEINLINE bool operator==(const FBPUniqueNetId& Other) const
	{
		return (IsValid() && Other.IsValid() && (*GetUniqueNetId() == *Other.GetUniqueNetId()));
	}

	FORCEINLINE bool operator!=(const FBPUniqueNetId& Other) const
	{
		return !(IsValid() && Other.IsValid() && (*GetUniqueNetId() == *Other.GetUniqueNetId()));
	}

	FBPUniqueNetId()
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
	}
};

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FBP_Login_Callback, bool, bWasSuccess, const FBPUniqueNetId&, UniqueNetId, const FString&,Error);
UCLASS()
class ONLINESUBSYSTEMEOS_API UEIK_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	//This is a C++ method definition for logging in a user to an online subsystem using provided credentials, and it sets up a callback function to handle the login response.
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Identity")
	void LoginWithDeviceID(int32 LocalUserNum, FString DisplayName, FString DeviceName, const FBP_Login_Callback& Result);



private:
	//Callback Reference Variables with all values of delegates stored as variable
	FBP_Login_Callback LoginWithDeviceID_CallBackBP;


	//Login With Device ID Callback which is returned if the user login is success.
	void LoginWithDeviceIDCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error) const;

};
