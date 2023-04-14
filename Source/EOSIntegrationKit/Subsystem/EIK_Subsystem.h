//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineStatsInterface.h"
#ifdef PLAYFAB_PLUGIN_INSTALLED
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#endif
#include "EIK_Subsystem.generated.h"

USTRUCT(BlueprintType)
struct FOffersStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	FText LongDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	FText RegularPriceText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	int64 RegularPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	FText PriceText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	int64 NumericPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	FDateTime ReleaseDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIK Nodes")
	FDateTime ExpirationDate;

};

USTRUCT(BlueprintType)
struct FSessionFindStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="EOS Struct")
	FBlueprintSessionResult SessionResult;

	UPROPERTY(BlueprintReadWrite, Category="EOS Struct")
	TMap<FString, FString> SessionSettings;
	

	UPROPERTY(BlueprintReadWrite, Category="EOS Struct")
	FString SessionName;

	UPROPERTY(BlueprintReadWrite, Category="EOS Struct")
	int32 CurrentNumberOfPlayers;

	UPROPERTY(BlueprintReadWrite, Category="EOS Struct")
	int32 MaxNumberOfPlayers;

	UPROPERTY(BlueprintReadWrite, Category="EOS Struct")
	bool bIsDedicatedServer;
};

USTRUCT(BlueprintType)
struct FParticipantRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Voice")
	FString PuID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Voice")
	FString ClientIP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Voice")
	bool HardMuted;
};
UENUM(BlueprintType)
enum class ERegionInfo : uint8 {
	RE_NoSelection       UMETA(DisplayName="No Selection"),
	RE_Asia               UMETA(DisplayName="Asia"),
	RE_NorthAmerica        UMETA(DisplayName="North America"),
	RE_SouthAmerica        UMETA(DisplayName="South America"),
	RE_Africa              UMETA(DisplayName="Africa"),
	RE_Europe              UMETA(DisplayName="Europe"),
	RE_Australia           UMETA(DisplayName="Australia"),

};

UENUM(BlueprintType)
enum class EMatchType : uint8 {
	MT_MatchMakingSession       UMETA(DisplayName="Matchmaking Session"),
	MT_Lobby					UMETA(DisplayName="Lobby Session"),
};
//Love you VaRest for this :D

USTRUCT(BlueprintType)
struct FEIKUniqueNetId
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
	FORCEINLINE bool operator==(const FEIKUniqueNetId& Other) const
	{
		return (IsValid() && Other.IsValid() && (*GetUniqueNetId() == *Other.GetUniqueNetId()));
	}

	FORCEINLINE bool operator!=(const FEIKUniqueNetId& Other) const
	{
		return !(IsValid() && Other.IsValid() && (*GetUniqueNetId() == *Other.GetUniqueNetId()));
	}

	FEIKUniqueNetId()
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Stats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="EIK Struct")
	FString StatsName;

	UPROPERTY(BlueprintReadWrite, Category="EIK Struct")
	FString StatsValue;
	
};
//Delegates for callbacks in BP
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FBP_Login_Callback, bool, bWasSuccess, const FEIKUniqueNetId&, UniqueNetId ,const FString&,Error);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_Logout_Callback, bool, bWasSuccess);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_CreateSession_Callback, bool, bWasSuccess, const FName&,SessionName);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_CreateLobby_Callback, bool, bWasSuccess, const FName&,SessionName);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_DestroySession_Callback, bool, bWasSuccess);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_PurchaseOffer_Callback, bool, bWasSuccess, const FString&, Error);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_JoinSession_Callback, bool, bWasSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_UpdateStat_Callback, bool, bWasSuccess);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_GetStats_Callback, bool, bWasSuccess, const TArray<FEIK_Stats>&,Stats);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_GetVoiceToken_Callback, bool, bWasSuccess, const FString&,Voice_Access_Token);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_FindSession_Callback, bool, bWasSuccess, const TArray<FSessionFindStruct>&, SessionResults);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_WriteFile_Callback, bool, bWasSuccess);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_GetFile_Callback, bool, bWasSuccess, USaveGame*,SaveGame);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_ConnectEOSAndPlayFab_Callback, bool, bWasSuccess, const FString&, Error);

UCLASS()
class EOSINTEGRATIONKIT_API UEIK_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	bool InitializeEIK();
	
	// This C++ method logs in a user to an online subsystem using their device ID and sets up a callback function to handle the login response.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/with-device-id
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	void LoginWithDeviceID(int32 LocalUserNum, FString DisplayName, FString DeviceName, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem through an account portal and sets up a callback function to handle the login response.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/with-account-portal
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	void LoginWithAccountPortal(int32 LocalUserNum, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem through an account portal and sets up a callback function to handle the login response.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/with-steam
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	void LoginWithSteam(int32 LocalUserNum, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem using persistent authentication and sets up a callback function to handle the login response.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/with-persistent-auth
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	void LoginWithPersistantAuth(int32 LocalUserNum, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem using a developer tool and sets up a callback function to handle the login response.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/with-devtool
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	void LoginWithDeveloperTool(int32 LocalUserNum, FString LocalIP, FString Credential, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem using the Epic Launcher and sets up a callback function to handle the login response.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/with-epic-launcher
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	void LoginWithEpicLauncher(int32 LocalUserNum, const FBP_Login_Callback& Result);

	// This is a C++ method definition for logging out a user from an online subsystem, and it sets up a callback function to handle the logout response.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/authentication/logout
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Login")
	void Logout(int32 LocalUserNum, const FBP_Logout_Callback& Result);

	// This is a C++ method definition for getting the nickname of a player from an online subsystem.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/getplayernickname
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra")
	FString GetPlayerNickname(const int32 LocalUserNum) const;

	// This is a C++ method definition for getting the login status of a player from an online subsystem.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/getloginstatus
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra")
	bool GetLoginStatus(const int32 LocalUserNum) const;
	
	//This is a C++ method definition for creating Epic Online Services Sessions
	UFUNCTION(BlueprintCallable,DisplayName="Create EOS Session", Category="EOS Integration Kit || Sessions")
	void CreateEOSSession( const FBP_CreateSession_Callback& Result, TMap<FString, FString> Custom_Settings, FString SessionName = "Modified_EOS_Session", 
								bool bIsDedicatedServer = false, 
								bool bIsLan = false,	
								int32 NumberOfPublicConnections = 4, 
								ERegionInfo Region = ERegionInfo::RE_NoSelection);

	//This is a C++ method definition for creating Epic Online Services Lobbies
	UFUNCTION(BlueprintCallable,DisplayName="Create EOS Lobby", Category="EOS Integration Kit || Sessions")
	void CreateEOSLobby( const FBP_CreateLobby_Callback& Result, TMap<FString, FString> Custom_Settings, FString SessionName = "Modified_EOS_Session",
							  bool bUseVoiceChat = true,
							  bool bUsePresence = true,
							  bool bAllowInvites = true,
							  bool bAdvertise = true,
							  bool bAllowJoinInProgress = true, 
							  bool bIsLan = false, 
							  int32 NumberOfPublicConnections = 4,
							  int32 NumberOfPrivateConnections = 4);

	// This is a C++ method definition for finding Epic Online Services Sessions
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/sessions/find-sessions-and-lobbies
	UFUNCTION(BlueprintCallable, DisplayName="Find EOS Session", Category="EOS Integration Kit || Sessions")
	void FindEOSSession(const FBP_FindSession_Callback& Result, TMap<FString, FString> Search_Settings, EMatchType MatchType = EMatchType::MT_Lobby, ERegionInfo RegionToSearch = ERegionInfo::RE_NoSelection);

	// This is a C++ method definition for destroying Epic Online Services Sessions
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/sessions/destroy-sessions
	UFUNCTION(BlueprintCallable, DisplayName="Destroy EOS Session", Category="EOS Integration Kit || Sessions")
	void DestroyEosSession(const FBP_DestroySession_Callback& Result, FName SessionName);

	// This is a C++ method definition for joining Epic Online Services Sessions
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/sessions/join-sessions
	UFUNCTION(BlueprintCallable, DisplayName="Join EOS Session", Category="EOS Integration Kit || Sessions")
	void JoinEosSession(const FBP_JoinSession_Callback& Result, FName SessionName, bool bIsDedicatedServerSession, FBlueprintSessionResult SessionResult);

	// This is a C++ method definition for getting the User Unique NetID
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/get-useruniqueid
	UFUNCTION(BlueprintPure, DisplayName="Get User Unique NetID", Category="EOS Integration Kit || Extra")
	FEIKUniqueNetId GetUserUniqueID() const;

	// This is a C++ method definition for getting the Product UserID
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/get-productuserid
	UFUNCTION(BlueprintPure, DisplayName="Get Product UserID", Category="EOS Integration Kit || Extra")
	static FString GetProductUserID(const FEIKUniqueNetId& UniqueNetId);

	// This is a C++ method definition for getting the Epic ID
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/get-epicid
	UFUNCTION(BlueprintPure, DisplayName="Get Epic ID", Category="EOS Integration Kit || Extra")
	static FString GetEpicID(const FEIKUniqueNetId& UniqueNetId);


	//This is a C++ method definition for getting the auth token of a player from an online subsystem.
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Friend")
	bool ShowFriendUserInterface();

	//Stat Functions
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Statistics")
	void UpdateStats(const FBP_UpdateStat_Callback& Result, FString StatName, int32 Amount);
	
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Statistics")
	void GetStats(const FBP_GetStats_Callback& Result, TArray<FString> StatName);
	
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Data")
	void SetPlayerData(const FBP_WriteFile_Callback& Result, FString FileName, USaveGame* SavedGame);

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Data")
	void GetPlayerData(const FBP_GetFile_Callback& Result, FString FileName);

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Leaderboard")
	void GetLeaderboard(const FBP_GetFile_Callback& Result, FString FileName);
	
	//This is a C++ method definition for finding Epic Online Services Sessions
	UFUNCTION(BlueprintCallable,DisplayName="Connect EOS And PlayFab", Category="EOS Integration Kit || PlayFab")
	void ConnectEosAndPlayFab( const FBP_ConnectEOSAndPlayFab_Callback& Result);

// This is a C++ method definition for setting voice credentials in Epic Online Services.
UFUNCTION(BlueprintCallable, DisplayName="Set Voice Credentials", Category="EOS Integration Kit || Voice Server")
void SetVoiceCredentials(FString ClientID, FString ClientSecret, FString DeploymentID, FString ProductUserID)
{
    Voice_ClientID = ClientID;
    Voice_ClientSecret = ClientSecret;
    Voice_DeploymentID = DeploymentID;
    Voice_ProductUserID = ProductUserID;
    return;
}

// This is a C++ method definition for purchasing an item from the store.
UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Store")
void PurchaseItem(const FBP_PurchaseOffer_Callback& Result, FText ItemName);

// This is a C++ method definition for logging in a user locally.
void Login(int32 LocalUserNum, FString ID, FString Token, FString Type, const FBP_Login_Callback& Result);

// The following are C++ callback method definitions for handling various events in the online subsystem.
void LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error) const;
void LogoutCallback(int32 LocalUserNum, bool bWasSuccess) const;
void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful) const;
void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
void OnCreateLobbyCompleted(FName SessionName, bool bWasSuccessful) const;
void OnFindSessionCompleted(bool bWasSuccess) const;
void OnDestroySessionCompleted(FName SessionName, bool bWasSuccess) const;
void OnUpdateStatsCompleted(const FOnlineError& Result) const;
void OnGetStatsCompleted(const FOnlineError &ResultState, const TArray<TSharedRef<const FOnlineStatsUserStats>> &UsersStatsResult) const;
void OnWriteFileComplete(bool bSuccess, const FUniqueNetId& UserID, const FString& FileName) const;
void OnGetFileComplete(bool bSuccess, const FUniqueNetId& UserID, const FString& FileName) const;

#ifdef PLAYFAB_PLUGIN_INSTALLED
	//PlayFabEvents
	void OnLoginWithEpicIDPFSuccess(const PlayFab::ClientModels::FLoginResult& Result) const
	{
		UE_LOG(LogTemp, Log, TEXT("Congratulations, you made your first successful API call!"));
		ConnectEosAndPlayFab_CallbackBP.ExecuteIfBound(true, "");
	}

	void OnLoginWithEpicIDPFFailure(const PlayFab::FPlayFabCppError& ErrorResult) const
	{
		UE_LOG(LogTemp, Error, TEXT("Something went wrong with your first API call.\nHere's some debug information:\n%s"), *ErrorResult.GenerateErrorReport());
		ConnectEosAndPlayFab_CallbackBP.ExecuteIfBound(false, TEXT("Something went wrong."));
	}
#endif
// The following are C++ variables used to store callback instances.
FBP_Login_Callback LoginCallBackBP;
FBP_Logout_Callback LogoutCallbackBP;
FBP_CreateSession_Callback CreateSession_CallbackBP;
FBP_CreateLobby_Callback CreateLobby_CallbackBP;
FBP_JoinSession_Callback JoinSession_CallbackBP;
FBP_FindSession_Callback FindSession_CallbackBP;
FBP_DestroySession_Callback DestroySession_CallbackBP;
FBP_ConnectEOSAndPlayFab_Callback ConnectEosAndPlayFab_CallbackBP;
FBP_UpdateStat_Callback UpdateStat_CallbackBP;
FBP_GetStats_Callback GetStats_CallbackBP;
FBP_GetFile_Callback GetFile_CallbackBP;
FBP_PurchaseOffer_Callback PurchaseOffer_CallbackBP;
FBP_WriteFile_Callback WriteFile_CallbackBP;

	FOnSessionUserInviteAcceptedDelegate OnSessionUserInviteAcceptedDelegate;
	void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);


// This is a C++ variable for storing a reference to an online session search.
TSharedPtr<FOnlineSessionSearch> SessionSearch;

// The following are C++ variables used for local information and settings.
FString LocalPortInfo;
bool Local_bIsDedicatedServerSession = false;

// The following are C++ variables for storing voice server information.
FString Voice_ClientID;
FString Voice_ClientSecret;
FString Voice_DeploymentID;
FString Voice_ProductUserID;



	
};






