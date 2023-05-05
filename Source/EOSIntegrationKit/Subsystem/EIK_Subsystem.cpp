//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Subsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#ifdef PLAYFAB_PLUGIN_INSTALLED
#include "Core/PlayFabClientAPI.h"
#endif


void UEIK_Subsystem::Login(int32 LocalUserNum, FString ID, FString Token , FString Type, const FBP_Login_Callback& Result)
{
	LoginCallBackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = Type;
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UEIK_Subsystem::LoginCallback);
			IdentityPointerRef->Login(LocalUserNum,AccountDetails);
		}
		else
		{
			Result.ExecuteIfBound(false,"Failed to get Identity Pointer");
		}
	}
	else
	{
		Result.ExecuteIfBound(false,"Failed to get Subsystem");
	}
}

bool UEIK_Subsystem::InitializeEIK()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineFriendsPtr FriendsPtr = SubsystemRef->GetFriendsInterface())
		{
			// Bind the OnSessionUserInviteAccepted event to a delegate
			OnSessionUserInviteAcceptedDelegate.BindUObject(this, &UEIK_Subsystem::OnSessionUserInviteAccepted);

			// Add the delegate to the online subsystem
			IOnlineSessionPtr SessionInt = Online::GetSessionInterface(GetWorld());
			if (SessionInt.IsValid())
			{
				SessionInt->AddOnSessionUserInviteAcceptedDelegate_Handle(OnSessionUserInviteAcceptedDelegate);
				UE_LOG(LogTemp, Warning, TEXT("Bound success"));
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
* This is a C++ function definition for a method called "Login" in the "UEIK_Subsystem" class. It takes in one parameters: an integer "LocalUserNum" It also takes in a callback function "Result" of type "FBP_Login_Callback".

The method first sets the "LoginCallBackBP" property of the class to the "Result" parameter. Then it retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, it creates an instance of "FOnlineAccountCredentials" with the values provided in the parameters and adds a login callback delegate to the "OnLoginCompleteDelegates" list of the identity interface. Finally, it calls the "Login" method of the identity interface with the "LocalUserNum" and "AccountDetails" parameters.

If the online subsystem or the identity interface cannot be retrieved, the method executes the "Result" callback function with a false value and an error message. The actual login response is handled by the "LoginCallback" function, which is defined elsewhere in the class and added to the "OnLoginCompleteDelegates" list as a delegate.
 */
void UEIK_Subsystem::LoginWithDeviceID(int32 LocalUserNum, FString DisplayName, FString DeviceName, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,DisplayName, DeviceName, "deviceid", Result);
}

void UEIK_Subsystem::LoginWithAccountPortal(int32 LocalUserNum, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,"", "", "accountportal", Result);
}

void UEIK_Subsystem::LoginWithSteam(int32 LocalUserNum, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,"", "", "steam", Result);
}

void UEIK_Subsystem::LoginWithPersistantAuth(int32 LocalUserNum, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,"", "", "persistentauth", Result);
}

void UEIK_Subsystem::LoginWithDeveloperTool(int32 LocalUserNum, FString LocalIP, FString Credential, const FBP_Login_Callback& Result)
{
	Login(LocalUserNum,LocalIP, Credential, "developer", Result);
}

void UEIK_Subsystem::LoginWithEpicLauncher(int32 LocalUserNum, const FBP_Login_Callback& Result)
{
	FString EGS_Token;
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_PASSWORD="), EGS_Token);
	Login(LocalUserNum,"", EGS_Token, "exchangecode", Result);
}

/*
* This is a method definition for the "Logout" method in the "UEIK_Subsystem" class, written in C++. The method takes in an integer "LocalUserNum" and a callback function "Result" of type "FBP_Logout_Callback". The method sets the "LogoutCallbackBP" property of the class to the "Result" parameter, then retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, the method adds a logout callback delegate to the "OnLogoutCompleteDelegates" list of the identity interface, and then calls the "Logout" method of the identity interface with the "LocalUserNum" parameter.

If the online subsystem or the identity interface cannot be retrieved, the method executes the "Result" callback function with a false value. The actual logout response is handled by the "LogoutCallback" function, which is defined elsewhere in the class and added to the "OnLogoutCompleteDelegates" list as a delegate.
 */
void UEIK_Subsystem::Logout(int32 LocalUserNum, const FBP_Logout_Callback& Result)
{
	LogoutCallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			IdentityPointerRef->OnLogoutCompleteDelegates->AddUObject(this,&UEIK_Subsystem::LogoutCallback);
			IdentityPointerRef->Logout(LocalUserNum);
		}
		else
		{
			Result.ExecuteIfBound(false);
		}
	}
	else
	{
		Result.ExecuteIfBound(false);
		
	}
}

/*
* This is a method definition for the "GetPlayerNickname" method in the "UEIK_Subsystem" class, written in C++. The method takes in an integer "LocalUserNum" as a parameter. The method retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, the method calls the "GetPlayerNickname" method of the identity interface with the "LocalUserNum" parameter and returns the result.

If the online subsystem or the identity interface cannot be retrieved, the method returns an empty string. This method is useful for retrieving the display name or nickname of a player in a multiplayer game, which can be used to identify the player in the game world or UI.
 */
FString UEIK_Subsystem::GetPlayerNickname(const int32 LocalUserNum)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
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
* This is a method definition for the "GetLoginStatus" method in the "UEIK_Subsystem" class, written in C++. The method takes in an integer "LocalUserNum" as a parameter. The method retrieves the current online subsystem and the identity interface for that subsystem. If the identity interface is successfully retrieved, the method calls the "GetLoginStatus" method of the identity interface with the "LocalUserNum" parameter and converts the result to a string using the "ToString" method.

If the online subsystem or the identity interface cannot be retrieved, the method returns an empty string. This method is useful for checking the login status of a player in a multiplayer game, which can be used to determine if the player needs to be logged in before accessing certain features or gameplay modes.
 */
bool UEIK_Subsystem::GetLoginStatus(const int32 LocalUserNum)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(IdentityPointerRef->GetLoginStatus(LocalUserNum)==ELoginStatus::LoggedIn)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UEIK_Subsystem::CreateEOSSession(const FBP_CreateSession_Callback& Result,
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
			SessionCreationInfo.bUsesPresence =true;
			SessionCreationInfo.bAllowJoinViaPresence = true;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = false;
			SessionCreationInfo.bAllowInvites = true;
			if(bIsDedicatedServer)
			{
				SessionCreationInfo.bUsesPresence = false;
				SessionCreationInfo.bAllowJoinViaPresence = false;
				SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = false;
				SessionCreationInfo.bAllowInvites = false;
			}
			SessionCreationInfo.bIsLANMatch = bIsLan;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = false;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = false;
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
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnCreateSessionCompleted);
			SessionPtrRef->CreateSession(0,*SessionName,SessionCreationInfo);
		}
	}
}

void UEIK_Subsystem::CreateEOSLobby(const FBP_CreateLobby_Callback& Result, TMap<FString, FString> Custom_Settings,
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
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnCreateLobbyCompleted);
			SessionPtrRef->CreateSession(0,*SessionName,SessionCreationInfo);
		}
	}
}

void UEIK_Subsystem::FindEOSSession(const FBP_FindSession_Callback& Result, TMap<FString, FString> Search_Settings,int32 MaxResults,
	EMatchType MatchType, ERegionInfo RegionToSearch)
{
	FindSession_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
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
			if (!Search_Settings.IsEmpty()) {
				for (auto& Settings_SingleValue : Search_Settings) {
					if (Settings_SingleValue.Key.Len() == 0) {
						continue;
					}
					FOnlineSessionSetting Setting;
					Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
					Setting.Data.SetValue(Settings_SingleValue.Value);
					SessionSearch->QuerySettings.Set(FName(*Settings_SingleValue.Key), Settings_SingleValue.Value, EOnlineComparisonOp::Equals);
				}
			}
			SessionSearch->MaxSearchResults = MaxResults;
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnFindSessionCompleted);
			SessionPtrRef->FindSessions(0,SessionSearch.ToSharedRef());
		}
		else
		{
			Result.ExecuteIfBound(false, TArray<FSessionFindStruct>());
		}
	}
	else
	{
		Result.ExecuteIfBound(false, TArray<FSessionFindStruct>());
	}
}

void UEIK_Subsystem::DestroyEosSession(const FBP_DestroySession_Callback& Result, FName SessionName)
{
	DestroySession_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this,&UEIK_Subsystem::OnDestroySessionCompleted);
			SessionPtrRef->DestroySession(SessionName);
		}
		else
		{
			Result.ExecuteIfBound(false);
		}
	}
	else
	{
		Result.ExecuteIfBound(false);
	}
}

void UEIK_Subsystem::JoinEosSession(const FBP_JoinSession_Callback& Result, FName SessionName,
	bool bIsDedicatedServerSession, FBlueprintSessionResult SessionResult)
{
	Local_bIsDedicatedServerSession = bIsDedicatedServerSession;
	JoinSession_CallbackBP = Result;
	const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SessionResult.OnlineResult.IsSessionInfoValid())
	{
		if(SubsystemRef)
		{
			if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
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
					Result.ExecuteIfBound(false);
				}
				SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnJoinSessionCompleted);
				SessionPtrRef->JoinSession(0, SessionName,SessionResult.OnlineResult);
			}
			else
			{
				Result.ExecuteIfBound(false);
			}
		}
		else
		{
			Result.ExecuteIfBound(false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session ref not valid"));
		Result.ExecuteIfBound(false);
	}
}

FEIKUniqueNetId UEIK_Subsystem::GetUserUniqueID() const
{
	if(const TSharedPtr<const FUniqueNetId> EIK_NetID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId())
	{
		FEIKUniqueNetId LocalUNetID;
		LocalUNetID.SetUniqueNetId(EIK_NetID);
		if(LocalUNetID.IsValid())
		{
			return LocalUNetID;
		}
		return FEIKUniqueNetId();
	}
	else
	{
		return FEIKUniqueNetId();
	}
}

FString UEIK_Subsystem::GetProductUserID(const FEIKUniqueNetId& UniqueNetId)
{
	if(UniqueNetId.IsValid() && UniqueNetId.UniqueNetId.IsValid())
	{
		const FString String_UserID = UniqueNetId.UniqueNetId.Get()->ToString();
		TArray<FString> Substrings;
		String_UserID.ParseIntoArray(Substrings, TEXT("|"));
		// Check if the split was successful
		if (Substrings.Num() == 2)
		{
			return Substrings[1];
		}
		else if(Substrings.Num() == 1)
		{
			return Substrings[0];
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

FString UEIK_Subsystem::GetEpicID(const FEIKUniqueNetId& UniqueNetId)
{
	if(UniqueNetId.IsValid())
	{
		const FString String_UserID = UniqueNetId.UniqueNetId.Get()->ToString();
		TArray<FString> Substrings;
		String_UserID.ParseIntoArray(Substrings, TEXT("|"));
		// Check if the split was successful
		if (Substrings.Num() == 2)
		{
			return Substrings[0];
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

void UEIK_Subsystem::UnRegisterPlayer(FName SessionName)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				SessionPtrRef->UnregisterPlayer(SessionName, *IdentityPointerRef->GetUniquePlayerId(0));
			}
		}
	}
}

bool UEIK_Subsystem::ShowFriendUserInterface()
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(); // Get the Online Subsystem
	if (OnlineSubsystem != nullptr)
	{
		const IOnlineExternalUIPtr ExternalUI = OnlineSubsystem->GetExternalUIInterface();        
		if (ExternalUI.IsValid())
		{
			return ExternalUI->ShowFriendsUI(0);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UEIK_Subsystem::UpdateStats(const FBP_UpdateStat_Callback& Result, FString StatName, int32 Amount)
{
	UpdateStat_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineStatsPtr StatsPointerRef = SubsystemRef->GetStatsInterface())
			{
				FOnlineStatsUserUpdatedStats StatVar = FOnlineStatsUserUpdatedStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef());
				StatVar.Stats.Add(StatName, FOnlineStatUpdate(Amount,FOnlineStatUpdate::EOnlineStatModificationType::Sum));
				TArray<FOnlineStatsUserUpdatedStats> StatArray;
				StatArray.Add(StatVar);
				StatsPointerRef->UpdateStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef(),StatArray,FOnlineStatsUpdateStatsComplete::CreateUObject(this, &UEIK_Subsystem::OnUpdateStatsCompleted));
			}
		}
	}
}

void UEIK_Subsystem::GetStats(const FBP_GetStats_Callback& Result, TArray<FString> StatName)
{
	GetStats_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineStatsPtr StatsPointerRef = SubsystemRef->GetStatsInterface())
			{
				TArray<TSharedRef<const FUniqueNetId>> Usersvar;
				Usersvar.Add(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef());
				StatsPointerRef->QueryStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef(),Usersvar,StatName,FOnlineStatsQueryUsersStatsComplete::CreateUObject(this, &UEIK_Subsystem::OnGetStatsCompleted));
			}
		}
	}
}


void UEIK_Subsystem::PurchaseItem(const FBP_PurchaseOffer_Callback& Result, FString ItemID)
{
	PurchaseOffer_CallbackBP = Result;
	if (const IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if (const IOnlineStoreV2Ptr StoreV2Ptr = SubsystemRef->GetStoreV2Interface())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if (const IOnlinePurchasePtr Purchase = SubsystemRef->GetPurchaseInterface())
				{
					FPurchaseCheckoutRequest Request = {};
					Request.AddPurchaseOffer(TEXT(""), ItemID, 1);

					Purchase->Checkout(*IdentityPointerRef->GetUniquePlayerId(0).Get(),
						Request,
						FOnPurchaseCheckoutComplete::CreateLambda(
							[this](
							const FOnlineError& Result,
							const TSharedRef<FPurchaseReceipt>& Receipt)
							{
								if (Result.WasSuccessful())
								{
									PurchaseOffer_CallbackBP.ExecuteIfBound(true);
								}
								else
								{
									PurchaseOffer_CallbackBP.ExecuteIfBound(false);
								}
							})
					);
				}
				else
				{
					PurchaseOffer_CallbackBP.ExecuteIfBound(false);
				}
			}
			else
			{
				PurchaseOffer_CallbackBP.ExecuteIfBound(false);
			}
		}
		else
		{
			PurchaseOffer_CallbackBP.ExecuteIfBound(false);
		}
	}
	else
	{
		PurchaseOffer_CallbackBP.ExecuteIfBound(false);
	}
}

void UEIK_Subsystem::QueryOffers(const FBP_GetOffers_Callback& Result)
{
	GetOffers_CallbackBP = Result;
	if (const IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if (const IOnlineStoreV2Ptr StoreV2Ptr = SubsystemRef->GetStoreV2Interface())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				StoreV2Ptr->QueryOffersByFilter(*IdentityPointerRef->GetUniquePlayerId(0).Get(), FOnlineStoreFilter(),
				                                FOnQueryOnlineStoreOffersComplete::CreateLambda([
						                                StoreV2Wk = TWeakPtr<IOnlineStoreV2, ESPMode::ThreadSafe>(
							                                StoreV2Ptr), this](
					                                bool bWasSuccessful,
					                                const TArray<FUniqueOfferId>& OfferIds,
					                                const FString& Error)
					                                {
						                                if (const auto StoreV2 = StoreV2Wk.Pin())
						                                {
							                                if (bWasSuccessful && StoreV2.IsValid())
							                                {
								                                TArray<FOnlineStoreOfferRef> Offers;
								                                StoreV2->GetOffers(Offers);
								                                TArray<FOffersStruct> OfferArray;
								                                for (int32 i = 0; i < Offers.Num(); ++i)
								                                {
								                                	OfferArray[i].ItemID = Offers[i]->OfferId;
									                                OfferArray[i].ItemName = Offers[i]->Title;
									                                OfferArray[i].Description = Offers[i]->Description;
									                                OfferArray[i].ExpirationDate = Offers[i]->
										                                ExpirationDate;
									                                OfferArray[i].LongDescription = Offers[i]->
										                                LongDescription;
									                                OfferArray[i].NumericPrice = Offers[i]->
										                                NumericPrice;
									                                OfferArray[i].PriceText = Offers[i]->PriceText;
									                                OfferArray[i].RegularPrice = Offers[i]->
										                                RegularPrice;
									                                OfferArray[i].ReleaseDate = Offers[i]->ReleaseDate;
									                                OfferArray[i].RegularPriceText = Offers[i]->
										                                RegularPriceText;
								                                }
								                                GetOffers_CallbackBP.ExecuteIfBound(true, OfferArray);
							                                }
							                                else
							                                {
							                                	GetOffers_CallbackBP.ExecuteIfBound(false, 	TArray<FOffersStruct>());
							                                }
						                                }
						                                else
						                                {
						                                	GetOffers_CallbackBP.ExecuteIfBound(false, 	TArray<FOffersStruct>());
						                                }
					                                }));
			}
			else
			{
				GetOffers_CallbackBP.ExecuteIfBound(false, 	TArray<FOffersStruct>());
			}
		}
		else
		{
			GetOffers_CallbackBP.ExecuteIfBound(false, 	TArray<FOffersStruct>());
		}
	}
	else
	{
		GetOffers_CallbackBP.ExecuteIfBound(false, 	TArray<FOffersStruct>());
	}
}

void UEIK_Subsystem::GetOwnedItems(const FBP_GetOwnedItems_Callback& Result)
{
	GetOwnedItems_CallbackBP = Result;
	if (const IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld()))
	{
		if (const IOnlineStoreV2Ptr StoreV2Ptr = SubsystemRef->GetStoreV2Interface())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if (const IOnlinePurchasePtr Purchase = SubsystemRef->GetPurchaseInterface())
				{
					Purchase->QueryReceipts(*IdentityPointerRef->GetUniquePlayerId(0).Get(), false,
										   FOnQueryReceiptsComplete::CreateLambda(
											   [this, SubsystemRef, IdentityPointerRef, Purchase
											   ](const FOnlineError& Error)
											   {
												   if (Error.WasSuccessful())
												   {
													   if (Purchase)
													   {
														   TArray<FString> ItemNames;
														   TArray<FPurchaseReceipt> Receipts;
														   Purchase->GetReceipts(
															   *IdentityPointerRef->GetUniquePlayerId(0).Get(), Receipts);
														   for (int i = 0; i < Receipts.Num(); i++)
														   {
														   	ItemNames.Add(Receipts[i].ReceiptOffers[0].LineItems[0].ItemName);
														   }
														   GetOwnedItems_CallbackBP.ExecuteIfBound(false, ItemNames);
													   }
													   else
													   {
														   GetOwnedItems_CallbackBP.ExecuteIfBound(false, TArray<FString>());
													   }
												   }
											   }));
				}
				else
				{
					GetOwnedItems_CallbackBP.ExecuteIfBound(false, TArray<FString>());
				}
			}
			else
			{
				GetOwnedItems_CallbackBP.ExecuteIfBound(false, TArray<FString>());
			}
		}
		else
		{
			GetOwnedItems_CallbackBP.ExecuteIfBound(false, TArray<FString>());
		}
	}
	else
	{
		GetOwnedItems_CallbackBP.ExecuteIfBound(false, TArray<FString>());
	}
}

FString UEIK_Subsystem::GenerateSessionCode() const
{
	FString SessionCode;
	const int32 CodeLength = 9;

	for (int32 i = 0; i < CodeLength; i++)
	{
		const int32 RandomNumber = FMath::RandRange(0, 35);

		// Convert the random number into a character (0-9, A-Z)
		TCHAR RandomChar = (RandomNumber < 10) ? TCHAR('0' + RandomNumber) : TCHAR('A' + (RandomNumber - 10));

		// Append the character to the session code
		SessionCode.AppendChar(RandomChar);
	}

	return SessionCode;
}


void UEIK_Subsystem::SetPlayerData(const FBP_WriteFile_Callback& Result, FString FileName, USaveGame* SavedGame)
{
	WriteFile_CallbackBP = Result;
	if(SavedGame)
	{
		TArray<uint8> LocalArray;
		UGameplayStatics::SaveGameToMemory(SavedGame,LocalArray);
		if(!LocalArray.IsEmpty())
		{
			if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get() )
			{
				if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
				{
					if(const IOnlineUserCloudPtr CloudPointerRef = SubsystemRef->GetUserCloudInterface())
					{
						const TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
						CloudPointerRef->OnWriteUserFileCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnWriteFileComplete);
						CloudPointerRef->WriteUserFile(*UserIDRef,FileName,LocalArray);
					}
					else
					{
						WriteFile_CallbackBP.ExecuteIfBound(false);
					}
				}
				else
				{
						WriteFile_CallbackBP.ExecuteIfBound(false);
				}
			}
			else
			{
				WriteFile_CallbackBP.ExecuteIfBound(false);
			}
		}
		else
		{
			WriteFile_CallbackBP.ExecuteIfBound(false);
		}
	}
	else
	{
		WriteFile_CallbackBP.ExecuteIfBound(false);
	}
}

void UEIK_Subsystem::GetPlayerData(const FBP_GetFile_Callback& Result, FString FileName)
{
	GetFile_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineUserCloudPtr CloudPointerRef = SubsystemRef->GetUserCloudInterface())
			{
				TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
				CloudPointerRef->OnReadUserFileCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnGetFileComplete);
				CloudPointerRef->ReadUserFile(*UserIDRef, FileName);
			}
			else
			{
				GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
			}
		}
		else
		{
			GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
		}
	}
	else
	{
		GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
	}
}

void UEIK_Subsystem::PlayerDataDownload(const FBP_DownloadFile_Callback& Result, FString FileName)
{
	DownloadFile_CallbackBP = Result;
	if (const IOnlineSubsystem* SubsystemRef = IOnlineSubsystem::Get())
	{
		if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if (const IOnlineUserCloudPtr CloudPointerRef = SubsystemRef->GetUserCloudInterface())
			{
				TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
				CloudPointerRef->OnReadUserFileCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnDownloadFile);
				CloudPointerRef->ReadUserFile(*UserIDRef, FileName);
			}
			else
			{
				GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
			}
		}
		else
		{
			GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
		}
	}
	else
	{
		GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
	}
}

void UEIK_Subsystem::PlayerDataUpload(const FBP_WriteFile_Callback& Result, FString FilePath)
{
	WriteFile_CallbackBP = Result;
	TArray<uint8> LocalArray;
	if (FFileHelper::LoadFileToArray(LocalArray, *FilePath))
	{
		if (const IOnlineSubsystem* SubsystemRef = IOnlineSubsystem::Get())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if (const IOnlineUserCloudPtr CloudPointerRef = SubsystemRef->GetUserCloudInterface())
				{
					const TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
					CloudPointerRef->OnWriteUserFileCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnWriteFileComplete);
					FString FileName = FPaths::GetCleanFilename(FilePath);
					CloudPointerRef->WriteUserFile(*UserIDRef, FileName, LocalArray);
				}
				else
				{
					WriteFile_CallbackBP.ExecuteIfBound(false);
				}
			}
			else
			{
				WriteFile_CallbackBP.ExecuteIfBound(false);
			}
		}
		else
		{
			WriteFile_CallbackBP.ExecuteIfBound(false);
		}
	}
	else
	{
		WriteFile_CallbackBP.ExecuteIfBound(false);
	}
}

void UEIK_Subsystem::EnumerateTitleFiles(const FBP_TitleFileList_Callback& Result)
{
	//Coming in Update 1.12
	TitleFileList_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineTitleFilePtr TitleFilePtr = SubsystemRef->GetTitleFileInterface())
			{
				TitleFilePtr->OnEnumerateFilesCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnTitleFileListComplete);
				TitleFilePtr->EnumerateFiles();
			}
			else
			{
				TitleFileList_CallbackBP.ExecuteIfBound(false, "Failed to get Title File Interface");
			}
		}
		else
		{
			TitleFileList_CallbackBP.ExecuteIfBound(false, "Failed to get Online Identity");
		}
	}
	else
	{
		TitleFileList_CallbackBP.ExecuteIfBound(false, "Failed to get Online Subsystem");
	}
}

TArray<FFileListStruct> UEIK_Subsystem::GetTitleFileList()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineTitleFilePtr TitleFilePtr = SubsystemRef->GetTitleFileInterface())
			{
				TArray<FCloudFileHeader> Files;
				TitleFilePtr->GetFileList(Files);
				TArray<FFileListStruct> Local_FileList;
				for(int i =0; i< Files.Num(); i++)
				{
					FFileListStruct Temp;
					Temp.FileName = Files[i].FileName;
					Temp.FileSize = Files[i].FileSize;
					Temp.Hash = Files[i].Hash;
					Temp.HashType = Files[i].HashType;
					Temp.iChunkID = Files[i].ChunkID;
					Temp.DLName = Files[i].DLName;
					Temp.ExternalStorageIds = Files[i].ExternalStorageIds;
					Temp.URL = Files[i].URL;
					Local_FileList.Add(Temp);
				}
				return Local_FileList;
			}
		}
	}
	return TArray<FFileListStruct>();
}

void UEIK_Subsystem::GetTitleFile(const FBP_GetTitleFile_Callback& Result,FString FileName)
{
	GetTitleFile_CallbackBP = Result;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineTitleFilePtr TitleFilePtr = SubsystemRef->GetTitleFileInterface())
			{
				TitleFilePtr->OnReadFileCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnTitleFileComplete);
				TitleFilePtr->ReadFile(FileName);
				return;
			}
		}
	}
	GetTitleFile_CallbackBP.ExecuteIfBound(false);
}

TArray<uint8> UEIK_Subsystem::GetTitleFileContent(FString FileName)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineTitleFilePtr TitleFilePtr = SubsystemRef->GetTitleFileInterface())
			{
				TArray<uint8> TitleFileContent;
				TitleFilePtr->GetFileContents(FileName, TitleFileContent);
				return TitleFileContent;
			}
		}
	}
	return TArray<uint8>();
}

void UEIK_Subsystem::GetLeaderboard(const FBP_GetFile_Callback& Result, FName LeaderboardName, int32 Rank, int32 Range)
{
	//Coming in 1.12
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(IOnlineIdentityPtr Identity = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineLeaderboardsPtr Leaderboards = SubsystemRef->GetLeaderboardsInterface())
			{
				// FOnlineLeaderboardReadRef ReadRef = MakeShared<FOnlineLeaderboardRead, ESPMode::ThreadSafe>();
				// ReadRef->LeaderboardName = LeaderboardName;
				// Leaderboards->AddOnLeaderboardReadCompleteDelegate_Handle(FOnLeaderboardReadComplete::FDelegate::CreateUObject(this,&UMyClass::HandleLeaderboardResult, ReadRef));
				// Leaderboards->ReadLeaderboardsAroundRank(Rank, Range,ReadRef);
			}
		}
	}
}

void UEIK_Subsystem::ConnectEosAndPlayFab(const FBP_ConnectEOSAndPlayFab_Callback& Result)
{
	ConnectEosAndPlayFab_CallbackBP = Result;
	if(GetLoginStatus(0) && !GetPlayerNickname(0).IsEmpty())
	{
		#ifdef PLAYFAB_PLUGIN_INSTALLED
		PlayFabClientPtr PlayFabClientAPI = nullptr;
		PlayFabClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
		PlayFab::ClientModels::FLoginWithCustomIDRequest CustomIDRequest;
		CustomIDRequest.CreateAccount = true;
		CustomIDRequest.CustomId = GetProductUserID(GetUserUniqueID());
		PlayFabClientAPI->LoginWithCustomID(CustomIDRequest,PlayFab::UPlayFabClientAPI::FLoginWithCustomIDDelegate::CreateUObject(this, &UEIK_Subsystem::OnLoginWithEpicIDPFSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UEIK_Subsystem::OnLoginWithEpicIDPFFailure));
		#else
		Result.ExecuteIfBound(false, "Plugin not installed");
		#endif
	}
	else
	{
		Result.ExecuteIfBound(false, "Either the user is not logged in or Nickname is empty");
	}
}

void UEIK_Subsystem::OnCreateLobbyCompleted(FName SessionName, bool bWasSuccessful) const
{
	if(bWasSuccessful)
	{
		FDelegateHandle SessionJoinHandle;
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
		{
			if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
			{
				if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
				{
			
				SessionPtrRef->RegisterPlayer(SessionName, *IdentityPointerRef->GetUniquePlayerId(0),false);
				CreateLobby_CallbackBP.ExecuteIfBound(bWasSuccessful, SessionName);
			}
			}
			else
			{
				CreateLobby_CallbackBP.ExecuteIfBound(false, SessionName);
			}
		}
		else
		{
			CreateLobby_CallbackBP.ExecuteIfBound(false, SessionName);
		}
	}
	else
	{
		CreateLobby_CallbackBP.ExecuteIfBound(false, SessionName);
	}
}

void UEIK_Subsystem::OnFindSessionCompleted(bool bWasSuccess) const
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
						LocalArraySettings.Add(*SettingName.ToString(), *SettingValueString);
						++It;
					}
					FSessionFindStruct LocalStruct;
					LocalStruct.SessionName = LocalArraySettings.FindRef("SEARCHKEYWORDS");
					LocalStruct.CurrentNumberOfPlayers = (SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections + SessionResult.OnlineResult.Session.SessionSettings.NumPrivateConnections) - (SessionResult.OnlineResult.Session.NumOpenPublicConnections + SessionResult.OnlineResult.Session.NumOpenPrivateConnections);
					LocalStruct.MaxNumberOfPlayers = SessionResult.OnlineResult.Session.SessionSettings.NumPublicConnections + SessionResult.OnlineResult.Session.SessionSettings.NumPrivateConnections;
					LocalStruct.SessionResult= SessionResult;
					LocalStruct.SessionSettings = LocalArraySettings;
					LocalStruct.bIsDedicatedServer = false;
					SessionResult_Array.Add(LocalStruct);
				}
			}
		}

		FindSession_CallbackBP.ExecuteIfBound(bWasSuccess, SessionResult_Array);
	}
}


void UEIK_Subsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result==EOnJoinSessionCompleteResult::Success)
	{
		if(APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(),0))
		{
			if(const IOnlineSubsystem *SubsystemRef =  IOnlineSubsystem::Get())
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
					if(!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress,ETravelType::TRAVEL_Absolute);
						JoinSession_CallbackBP.ExecuteIfBound(true);
						return;
					}
					else
					{
						JoinSession_CallbackBP.ExecuteIfBound(false);
						return;
					}
				}
				else
				{
					JoinSession_CallbackBP.ExecuteIfBound(false);
					return;
				}
			}
			else
			{
				JoinSession_CallbackBP.ExecuteIfBound(false);
				return;
			}
		}
		else
		{
			JoinSession_CallbackBP.ExecuteIfBound(false);
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Join Session Error with Reason of %d"), Result);
		JoinSession_CallbackBP.ExecuteIfBound(false);
		return;
	}
	JoinSession_CallbackBP.ExecuteIfBound(false);
}
void UEIK_Subsystem::OnDestroySessionCompleted(FName SessionName, bool bWasSuccess) const
{
	DestroySession_CallbackBP.ExecuteIfBound(bWasSuccess);
}

void UEIK_Subsystem::OnUpdateStatsCompleted(const FOnlineError& Result) const
{
	if(Result == FOnlineError::Success())
	{
		UpdateStat_CallbackBP.ExecuteIfBound(true);
	}
	else
	{
		UpdateStat_CallbackBP.ExecuteIfBound(false);
	}
}

void UEIK_Subsystem::OnGetStatsCompleted(const FOnlineError& ResultState,
	const TArray<TSharedRef<const FOnlineStatsUserStats>>& UsersStatsResult) const
{
	if(ResultState.WasSuccessful())
	{
		TArray<FEIK_Stats> LocalStatsArray;
		for(const auto& StatsVar : UsersStatsResult)
		{
			for(auto StoredValueRef : StatsVar->Stats)
			{
				FString Keyname = StoredValueRef.Key;
				int32 Value;
				StoredValueRef.Value.GetValue(Value);
				FEIK_Stats LocalStats;
				LocalStats.StatsName = Keyname;
				LocalStats.StatsValue = FString::FromInt(Value);
				LocalStatsArray.Add(LocalStats);
			}
		}
		GetStats_CallbackBP.ExecuteIfBound(true, LocalStatsArray);
	}
	else
	{
		GetStats_CallbackBP.ExecuteIfBound(false, TArray<FEIK_Stats>());
		UE_LOG(LogTemp,Warning,TEXT("Getting stats failed with error - %s"), *ResultState.ToLogString());
	}
}

void UEIK_Subsystem::OnWriteFileComplete(bool bSuccess, const FUniqueNetId& UserID, const FString& FileName) const
{
	WriteFile_CallbackBP.ExecuteIfBound(true);
}

void UEIK_Subsystem::OnGetFileComplete(bool bSuccess, const FUniqueNetId& UserID, const FString& FileName) const
{
	if(bSuccess)
	{
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if(const IOnlineUserCloudPtr CloudPointerRef = SubsystemRef->GetUserCloudInterface())
				{
					TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
					TArray<uint8> FileContents;
					CloudPointerRef->GetFileContents(*UserIDRef,FileName,FileContents);
					if(!FileContents.IsEmpty())
					{
						USaveGame* LocalSaveGame = UGameplayStatics::LoadGameFromMemory(FileContents);
						GetFile_CallbackBP.ExecuteIfBound(true, LocalSaveGame);
					}
					else
					{
						GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
					}
				}
				else
				{
					GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
				}
			}
			else
			{
				GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
			}
		}
		else
		{
			GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
		}
	}
	else
	{
		GetFile_CallbackBP.ExecuteIfBound(false, nullptr);
	}
}

void UEIK_Subsystem::OnDownloadFile(bool bSuccess, const FUniqueNetId& UserID, const FString& FileName) const
{
	FString Path;
	if (bSuccess)
	{
		if (const IOnlineSubsystem* SubsystemRef = IOnlineSubsystem::Get())
		{
			if (const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if (const IOnlineUserCloudPtr CloudPointerRef = SubsystemRef->GetUserCloudInterface())
				{
					TSharedPtr<const FUniqueNetId> UserIDRef = IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef();
					TArray<uint8> FileContents;
					CloudPointerRef->GetFileContents(*UserIDRef, FileName, FileContents);
					if (!FileContents.IsEmpty())
					{
						Path = FPaths::ProjectSavedDir() + "/PlayerData/" + FileName;
						FFileHelper::SaveArrayToFile(FileContents, *Path);
						DownloadFile_CallbackBP.ExecuteIfBound(true, Path);
					}
					else
					{
						DownloadFile_CallbackBP.ExecuteIfBound(false, Path);
					}
				}
				else
				{
					DownloadFile_CallbackBP.ExecuteIfBound(false, Path);
				}
			}
			else
			{
				DownloadFile_CallbackBP.ExecuteIfBound(false, Path);
			}
		}
		else
		{
			DownloadFile_CallbackBP.ExecuteIfBound(false, Path);
		}
	}
	else
	{
		DownloadFile_CallbackBP.ExecuteIfBound(false, Path);
	}
}

void UEIK_Subsystem::OnTitleFileListComplete(bool bSuccess, const FString& Error) const
{
	TitleFileList_CallbackBP.ExecuteIfBound(bSuccess, Error);
}

void UEIK_Subsystem::OnTitleFileComplete(bool bSuccess, const FString& FileName) const
{
	GetTitleFile_CallbackBP.ExecuteIfBound(bSuccess);
}

void UEIK_Subsystem::OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
                                                 FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("User %s has accepted an invitation to join session %s"), *UserId->ToString(), *InviteResult.GetSessionIdStr());
		// Join the session
		if (const IOnlineSessionPtr SessionInt = Online::GetSessionInterface(GetWorld()))
		{
			SessionInt->OnJoinSessionCompleteDelegates.AddUObject(this, &UEIK_Subsystem::OnJoinSessionCompleted);
			const FString SessionName = InviteResult.GetSessionIdStr();
			UE_LOG(LogTemp, Log, TEXT("User %s has accepted an invitation to join session %s"), *UserId->ToString(), *SessionName);
			SessionInt->JoinSession(0, FName(*SessionName),InviteResult);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to accept invitation to join session"));
	} 
}

//Callback Functions
void UEIK_Subsystem::LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId,
                                       const FString& Error) const
{
	LoginCallBackBP.ExecuteIfBound(bWasSuccess,Error);
}

void UEIK_Subsystem::LogoutCallback(int32 LocalUserNum, bool bWasSuccess) const
{
	LogoutCallbackBP.ExecuteIfBound(bWasSuccess);
}

void UEIK_Subsystem::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful) const
{
	if(bWasSuccessful)
	{
		FDelegateHandle SessionJoinHandle;
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
		{
			if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
			{
					if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
					{
						SessionPtrRef->RegisterPlayer(SessionName, *IdentityPointerRef->GetUniquePlayerId(0),false);
						CreateSession_CallbackBP.ExecuteIfBound(bWasSuccessful, SessionName);
					}
			}
			else
			{
				CreateSession_CallbackBP.ExecuteIfBound(false, SessionName);
			}
		}
		else
		{
			CreateSession_CallbackBP.ExecuteIfBound(false, SessionName);
		}
	}
	else
	{
		CreateSession_CallbackBP.ExecuteIfBound(false, SessionName);
	}
}
