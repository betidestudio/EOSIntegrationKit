// Betide Studio - 2023 


#include "Mod_EOS_Subsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"


void UMod_EOS_Subsystem::Login(int32 LocalUserNum, FString ID, FString Token , FString Type, const FBP_Login_Callback& Result)
{
	LoginCallBackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = Type;
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UMod_EOS_Subsystem::LoginCallback);
			IdentityPointerRef->Login(LocalUserNum,AccountDetails);
		}
		else
		{
			Result.Execute(false, FBPUniqueNetId(),"Failed to get Identity Pointer");
		}
	}
	else
	{
		Result.Execute(false,FBPUniqueNetId(),"Failed to get Subsystem");
	}
}


/*
* This is a C++ function definition for a method called "Login" in the "UMod_EOS_Subsystem" class. It takes in one parameters: an integer "LocalUserNum" It also takes in a callback function "Result" of type "FBP_Login_Callback".

The method first sets the "LoginCallBackBP" property of the class to the "Result" parameter. Then it retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, it creates an instance of "FOnlineAccountCredentials" with the values provided in the parameters and adds a login callback delegate to the "OnLoginCompleteDelegates" list of the identity interface. Finally, it calls the "Login" method of the identity interface with the "LocalUserNum" and "AccountDetails" parameters.

If the online subsystem or the identity interface cannot be retrieved, the method executes the "Result" callback function with a false value and an error message. The actual login response is handled by the "LoginCallback" function, which is defined elsewhere in the class and added to the "OnLoginCompleteDelegates" list as a delegate.
 */
void UMod_EOS_Subsystem::LoginWithDeviceID(int32 LocalUserNum, FString DisplayName, FString DeviceName, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,DisplayName, DeviceName, "deviceid", Result);
}

void UMod_EOS_Subsystem::LoginWithAccountPortal(int32 LocalUserNum, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,"", "", "accountportal", Result);
}

void UMod_EOS_Subsystem::LoginWithPersistantAuth(int32 LocalUserNum, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,"", "", "persistentauth", Result);
}

void UMod_EOS_Subsystem::LoginWithDeveloperTool(int32 LocalUserNum, FString LocalIP, FString Credential, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,LocalIP, Credential, "developer", Result);
}

void UMod_EOS_Subsystem::LoginWithEpicLauncher(int32 LocalUserNum, const FBP_Login_Callback& Result)
{
}

/*
* This is a method definition for the "Logout" method in the "UMod_EOS_Subsystem" class, written in C++. The method takes in an integer "LocalUserNum" and a callback function "Result" of type "FBP_Logout_Callback". The method sets the "LogoutCallbackBP" property of the class to the "Result" parameter, then retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, the method adds a logout callback delegate to the "OnLogoutCompleteDelegates" list of the identity interface, and then calls the "Logout" method of the identity interface with the "LocalUserNum" parameter.

If the online subsystem or the identity interface cannot be retrieved, the method executes the "Result" callback function with a false value. The actual logout response is handled by the "LogoutCallback" function, which is defined elsewhere in the class and added to the "OnLogoutCompleteDelegates" list as a delegate.
 */
void UMod_EOS_Subsystem::Logout(int32 LocalUserNum, const FBP_Logout_Callback& Result)
{
	LogoutCallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			IdentityPointerRef->OnLogoutCompleteDelegates->AddUObject(this,&UMod_EOS_Subsystem::LogoutCallback);
			IdentityPointerRef->Logout(LocalUserNum);
		}
		else
		{
			Result.Execute(false);
		}
	}
	else
	{
		Result.Execute(false);
		
	}
}

/*
* This is a method definition for the "GetPlayerNickname" method in the "UMod_EOS_Subsystem" class, written in C++. The method takes in an integer "LocalUserNum" as a parameter. The method retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, the method calls the "GetPlayerNickname" method of the identity interface with the "LocalUserNum" parameter and returns the result.

If the online subsystem or the identity interface cannot be retrieved, the method returns an empty string. This method is useful for retrieving the display name or nickname of a player in a multiplayer game, which can be used to identify the player in the game world or UI.
 */
FString UMod_EOS_Subsystem::GetPlayerNickname(const int32 LocalUserNum) const
{
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			return IdentityPointerRef->GetPlayerNickname(LocalUserNum);
		}
		else
		{
			return FString();
		}
	}
	else
	{
		return FString();
	}
}

/*
* This is a method definition for the "GetLoginStatus" method in the "UMod_EOS_Subsystem" class, written in C++. The method takes in an integer "LocalUserNum" as a parameter. The method retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, the method calls the "GetLoginStatus" method of the identity interface with the "LocalUserNum" parameter and converts the result to a string using the "ToString" method.

If the online subsystem or the identity interface cannot be retrieved, the method returns an empty string. This method is useful for checking the login status of a player in a multiplayer game, which can be used to determine if the player needs to be logged in before accessing certain features or gameplay modes.
 */
FString UMod_EOS_Subsystem::GetLoginStatus(const int32 LocalUserNum) const
{
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			return ToString(IdentityPointerRef->GetLoginStatus(LocalUserNum));
		}
		else
		{
			return FString();
		}
	}
	else
	{
		return FString();
	}
}



//Callback Functions
void UMod_EOS_Subsystem::LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId,
                                       const FString& Error) const
{
	FBPUniqueNetId LocalUserID;
	if(bWasSuccess)
	{
		LocalUserID.SetUniqueNetId(&UserId);
	}
	LoginCallBackBP.Execute(bWasSuccess,LocalUserID,Error);
}

void UMod_EOS_Subsystem::LogoutCallback(int32 LocalUserNum, bool bWasSuccess) const
{
	LogoutCallbackBP.Execute(bWasSuccess);
}
