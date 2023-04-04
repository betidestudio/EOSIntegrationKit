// Betide Studio - 2023 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Mod_EOS_Subsystem.generated.h"


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

//Delegates for callbacks in BP
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FBP_Login_Callback, bool, bWasSuccess, const FBPUniqueNetId&, UniqueNetId ,const FString&,Error);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_Logout_Callback, bool, bWasSuccess);


UCLASS()
class MODONLINESUBSYSTEMEOS_API UMod_EOS_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//This is a C++ method definition for logging in a user to an online subsystem using provided credentials, and it sets up a callback function to handle the login response.
	UFUNCTION(BlueprintCallable, Category="OSIK Identity")
	void LoginWithDeviceID(int32 LocalUserNum, FString DisplayName, FString DeviceName, const FBP_Login_Callback& Result);
	
	//This is a C++ method definition for logging out a user from an online subsystem, and it sets up a callback function to handle the logout response.
	UFUNCTION(BlueprintCallable, Category="OSIK Identity")
	void Logout(int32 LocalUserNum, const FBP_Logout_Callback& Result);

	//This is a C++ method definition for getting the nickname of a player from an online subsystem.
	UFUNCTION(BlueprintPure, Category="OSIK Identity")
	FString GetPlayerNickname(const int32 LocalUserNum) const;

	//This is a C++ method definition for getting the login status of a player from an online subsystem.
	UFUNCTION(BlueprintPure, Category="OSIK Identity")
	FString GetLoginStatus(const int32 LocalUserNum) const;
	

	
	//Return Functions
	void LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error) const;
	void LogoutCallback(int32 LocalUserNum,bool bWasSuccess) const;


	//Variables
	FBP_Login_Callback LoginCallBackBP;
	FBP_Logout_Callback LogoutCallbackBP;
	
};
