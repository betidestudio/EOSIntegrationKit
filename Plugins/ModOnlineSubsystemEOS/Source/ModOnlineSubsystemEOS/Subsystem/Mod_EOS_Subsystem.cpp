// Betide Studio - 2023 


#include "Mod_EOS_Subsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"


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

void UMod_EOS_Subsystem::CreateEOSSession(const FBP_CreateSession_Callback& Result,
	TMap<FString, FString> Custom_Settings, FString SessionName, bool bIsDedicatedServer, bool bIsLan,
	int32 NumberOfPublicConnections, ERegionInfo Region)
{
	CreateSession_CallbackBP = Result;
	if(IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
			SessionCreationInfo.bAllowInvites = true;
			SessionCreationInfo.bIsLANMatch = bIsLan;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = false;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = false;
			SessionCreationInfo.bUsesPresence =true;
			SessionCreationInfo.bAllowJoinViaPresence = true;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = false;
			SessionCreationInfo.bShouldAdvertise = true;
			SessionCreationInfo.bAllowJoinInProgress = true;
			
			SessionCreationInfo.Settings.Add( FName(TEXT("REGIONINFO")), FOnlineSessionSetting(UEnum::GetValueAsString(Region), EOnlineDataAdvertisementType::ViaOnlineService));
			if(bIsDedicatedServer)
			{
				SessionCreationInfo.Settings.Add( FName(TEXT("PortInfo")), FOnlineSessionSetting(GetWorld()->URL.Port, EOnlineDataAdvertisementType::ViaOnlineService));
			}
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FString(SessionName), EOnlineDataAdvertisementType::ViaOnlineService);
			for (auto& Settings_SingleValue : Custom_Settings)
			{
				if (Settings_SingleValue.Key.Len() == 0)
				{
					continue;
				}

				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Data.SetValue(Settings_SingleValue.Value);
				SessionCreationInfo.Set(FName(*Settings_SingleValue.Key), Setting);
			}
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UMod_EOS_Subsystem::OnCreateSessionCompleted);
			SessionPtrRef->CreateSession(0,*SessionName,SessionCreationInfo);
		}
	}
}

void UMod_EOS_Subsystem::CreateEOSLobby(const FBP_CreateLobby_Callback& Result, TMap<FString, FString> Custom_Settings,
	FString SessionName, bool bUseVoiceChat, bool bUsePresence, bool bAllowInvites, bool bAdvertise,
	bool bAllowJoinInProgress, bool bIsLan, int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections)
{
	CreateLobby_CallbackBP = Result;
	if(IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = false;
			SessionCreationInfo.bAllowInvites = bAllowInvites;
			SessionCreationInfo.bIsLANMatch = bIsLan;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.NumPrivateConnections = NumberOfPrivateConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = true;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = bUseVoiceChat;
			SessionCreationInfo.bUsesPresence =bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresence = bUsePresence;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = bUsePresence;
			SessionCreationInfo.bShouldAdvertise = bAdvertise;
			SessionCreationInfo.bAllowJoinInProgress = bAllowJoinInProgress;
			
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FString(SessionName), EOnlineDataAdvertisementType::ViaOnlineService);
			for (auto& Settings_SingleValue : Custom_Settings)
			{
				if (Settings_SingleValue.Key.Len() == 0)
				{
					continue;
				}

				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Data.SetValue(Settings_SingleValue.Value);
				SessionCreationInfo.Set(FName(*Settings_SingleValue.Key), Setting);
			}
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UMod_EOS_Subsystem::OnCreateLobbyCompleted);
			SessionPtrRef->CreateSession(0,*SessionName,SessionCreationInfo);
		}
	}
}

void UMod_EOS_Subsystem::FindEOSSession(const FBP_FindSession_Callback& Result, TMap<FString, FString> Search_Settings,
	EMatchType MatchType, ERegionInfo RegionToSearch)
{
	FindSession_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->QuerySettings.SearchParams.Empty();
			SessionSearch->bIsLanQuery = false;
			if(MatchType == EMatchType::MT_MatchMakingSession)
			{
				if(RegionToSearch!=ERegionInfo::RE_NoSelection)
				{
					SessionSearch->QuerySettings.Set(FName(TEXT("RegionInfo")),UEnum::GetValueAsString(RegionToSearch), EOnlineComparisonOp::Equals);
				}
			}
			else
			{
				SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			}
			for (auto& Settings_SingleValue : Search_Settings)
			{
				if (Settings_SingleValue.Key.Len() == 0)
				{
					continue;
				}
				FOnlineSessionSetting Setting;
				Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				Setting.Data.SetValue(Settings_SingleValue.Value);
				SessionSearch->QuerySettings.Set(FName(*Settings_SingleValue.Key), Settings_SingleValue.Value, EOnlineComparisonOp::Equals);
			}
			SessionSearch->MaxSearchResults = 1000;
 
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UMod_EOS_Subsystem::OnFindSessionCompleted);
			SessionPtrRef->FindSessions(0,SessionSearch.ToSharedRef());
		}
	}	
}

void UMod_EOS_Subsystem::DestroyEosSession(const FBP_DestroySession_Callback& Result, FName SessionName)
{
	DestroySession_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this,&UMod_EOS_Subsystem::OnDestroySessionCompleted);
			SessionPtrRef->DestroySession(SessionName);
		}
	}
}

void UMod_EOS_Subsystem::JoinEosSession(const FBP_JoinSession_Callback& Result, FName SessionName,
	bool bIsDedicatedServerSession, FBlueprintSessionResult SessionResult)
{
	Local_bIsDedicatedServerSession = bIsDedicatedServerSession;
	JoinSession_CallbackBP = Result;
	const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SessionResult.OnlineResult.IsSessionInfoValid())
	{
		if(SubsystemRef)
		{
			const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if(SessionPtrRef)
			{
				if(SessionResult.OnlineResult.Session.SessionSettings.Settings.Num() > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Session settings:"));
					for (auto& Elem : SessionResult.OnlineResult.Session.SessionSettings.Settings)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *Elem.Key.ToString(), *Elem.Value.Data.ToString());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Session settings not found"));
				}
				SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UMod_EOS_Subsystem::OnJoinSessionCompleted);
				SessionPtrRef->JoinSession(0, SessionName,SessionResult.OnlineResult);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session ref not valid"));
	}
}

void UMod_EOS_Subsystem::OnCreateLobbyCompleted(FName SessionName, bool bWasSuccessful) const
{
	if(bWasSuccessful)
	{
		FDelegateHandle SessionJoinHandle;
		if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
		{
			if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
			{
				if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
				{
			
				SessionPtrRef->RegisterPlayer(SessionName, *IdentityPointerRef->GetUniquePlayerId(0),false);
				CreateLobby_CallbackBP.Execute(bWasSuccessful, SessionName);
			}
			}
			else
			{
				CreateLobby_CallbackBP.Execute(false, SessionName);
			}
		}
		else
		{
			CreateLobby_CallbackBP.Execute(false, SessionName);
		}
	}
	else
	{
		CreateLobby_CallbackBP.Execute(false, SessionName);
	}
}

void UMod_EOS_Subsystem::OnFindSessionCompleted(bool bWasSuccess) const
{
	if (const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		TArray<FSessionFindStruct> SessionResult_Array;
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			if (SessionSearch->SearchResults.Num() > 0)
			{
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					FBlueprintSessionResult SessionResult;
					SessionResult.OnlineResult = SessionSearch->SearchResults[SearchIdx];
					FOnlineSessionSettings SessionSettings  = SessionResult.OnlineResult.Session.SessionSettings;
					TMap<FName, FString> AllSettingsWithData;
					TMap<FName, FOnlineSessionSetting>::TIterator It(SessionSettings.Settings);

					TMap<FString, FString> LocalArraySettings;
					while (It)
					{
						const FName& SettingName = It.Key();
						const FOnlineSessionSetting& Setting = It.Value();
						FString SettingValueString = Setting.Data.ToString();

						UE_LOG(LogTemp, Log, TEXT("%s: %s"), *SettingName.ToString(), *SettingValueString);
						LocalArraySettings.Add(*SettingName.ToString(), *SettingValueString);
						++It;
					}
					FSessionFindStruct LocalStruct;
					LocalStruct.SessionName = LocalArraySettings.FindRef("SEARCHKEYWORDS");
					LocalStruct.CurrentNumberOfPlayers = SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections - SessionResult.OnlineResult.Session.NumOpenPublicConnections;
					LocalStruct.MaxNumberOfPlayers = SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections;
					LocalStruct.SessionResult= SessionResult;
					LocalStruct.SessionSettings = LocalArraySettings;
					SessionResult_Array.Add(LocalStruct);
				}
			}
		}

		FindSession_CallbackBP.Execute(bWasSuccess, SessionResult_Array);
	}
}


void UMod_EOS_Subsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result==EOnJoinSessionCompleteResult::Success)
	{
		if(APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(),0))
		{
			if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
			{
				if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
				{
					FString JoinAddress;
					SessionPtrRef->GetResolvedConnectString(SessionName,JoinAddress);
					if(Local_bIsDedicatedServerSession)
					{
						TArray<FString> IpPortArray;
						JoinAddress.ParseIntoArray(IpPortArray, TEXT(":"), true);
						const FString IpAddress = IpPortArray[0];
						if(LocalPortInfo.IsEmpty())
						{
							LocalPortInfo = "7777";
						}
						const FString NewCustomIP = IpAddress + ":" + LocalPortInfo;
						JoinAddress = NewCustomIP;
					}
					UE_LOG(LogTemp,Warning,TEXT("Join Address is %s"), *JoinAddress);
					if(!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress,ETravelType::TRAVEL_Absolute);
						JoinSession_CallbackBP.ExecuteIfBound(true);
						return;
					}
					else
					{
						JoinSession_CallbackBP.Execute(false);
						return;
					}
				}
				else
				{
					JoinSession_CallbackBP.Execute(false);
					return;
				}
			}
			else
			{
				JoinSession_CallbackBP.Execute(false);
				return;
			}
		}
		else
		{
			JoinSession_CallbackBP.Execute(false);
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Join Session Error with Reason of %d"), Result);
		JoinSession_CallbackBP.Execute(false);
		return;
	}
	JoinSession_CallbackBP.Execute(false);
}
void UMod_EOS_Subsystem::OnDestroySessionCompleted(FName SessionName, bool bWasSuccess) const
{
	DestroySession_CallbackBP.Execute(bWasSuccess);
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

void UMod_EOS_Subsystem::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful) const
{
	if(bWasSuccessful)
	{
		FDelegateHandle SessionJoinHandle;
		if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld()))
		{
			if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
			{
					if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
					{
						SessionPtrRef->RegisterPlayer(SessionName, *IdentityPointerRef->GetUniquePlayerId(0),false);
						CreateSession_CallbackBP.Execute(bWasSuccessful, SessionName);
					}
			}
			else
			{
				CreateSession_CallbackBP.Execute(false, SessionName);
			}
		}
		else
		{
			CreateSession_CallbackBP.Execute(false, SessionName);
		}
	}
	else
	{
		CreateSession_CallbackBP.Execute(false, SessionName);
	}
}
