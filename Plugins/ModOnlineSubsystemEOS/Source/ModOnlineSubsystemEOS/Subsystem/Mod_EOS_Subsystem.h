// Betide Studio - 2023 

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Mod_EOS_Subsystem.generated.h"

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
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_CreateSession_Callback, bool, bWasSuccess, const FName&,SessionName);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_CreateLobby_Callback, bool, bWasSuccess, const FName&,SessionName);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_DestroySession_Callback, bool, bWasSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FBP_JoinSession_Callback, bool, bWasSuccess);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBP_FindSession_Callback, bool, bWasSuccess, const TArray<FSessionFindStruct>&, SessionResults);

UCLASS()
class MODONLINESUBSYSTEMEOS_API UMod_EOS_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// This C++ method logs in a user to an online subsystem using their device ID and sets up a callback function to handle the login response.
	UFUNCTION(BlueprintCallable, Category="Modified EOS || Login")
	void LoginWithDeviceID(int32 LocalUserNum, FString DisplayName, FString DeviceName, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem through an account portal and sets up a callback function to handle the login response.
	UFUNCTION(BlueprintCallable, Category="Modified EOS || Login")
	void LoginWithAccountPortal(int32 LocalUserNum, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem using persistent authentication and sets up a callback function to handle the login response.
	UFUNCTION(BlueprintCallable, Category="Modified EOS || Login")
	void LoginWithPersistantAuth(int32 LocalUserNum, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem using a developer tool and sets up a callback function to handle the login response.
	UFUNCTION(BlueprintCallable, Category="Modified EOS || Login")
	void LoginWithDeveloperTool(int32 LocalUserNum, FString LocalIP, FString Credential, const FBP_Login_Callback& Result);

	// This C++ method logs in a user to an online subsystem using the Epic Launcher and sets up a callback function to handle the login response.
	UFUNCTION(BlueprintCallable, Category="Modified EOS || Login")
	void LoginWithEpicLauncher(int32 LocalUserNum, const FBP_Login_Callback& Result);
	
	//This is a C++ method definition for logging out a user from an online subsystem, and it sets up a callback function to handle the logout response.
	UFUNCTION(BlueprintCallable, Category="Modified EOS || Login")
	void Logout(int32 LocalUserNum, const FBP_Logout_Callback& Result);

	//This is a C++ method definition for getting the nickname of a player from an online subsystem.
	UFUNCTION(BlueprintPure, Category="Modified EOS || Extra")
	FString GetPlayerNickname(const int32 LocalUserNum) const;

	//This is a C++ method definition for getting the login status of a player from an online subsystem.
	UFUNCTION(BlueprintPure, Category="Modified EOS || Extra")
	FString GetLoginStatus(const int32 LocalUserNum) const;
	
	//This is a C++ method definition for creating Epic Online Services Sessions
	UFUNCTION(BlueprintCallable,DisplayName="Create EOS Session", Category="Modified EOS || Sessions")
	void CreateEOSSession( const FBP_CreateSession_Callback& Result, TMap<FString, FString> Custom_Settings, FString SessionName = "Modified_EOS_Session", 
								bool bIsDedicatedServer = false, 
								bool bIsLan = false,	
								int32 NumberOfPublicConnections = 4, 
								ERegionInfo Region = ERegionInfo::RE_NoSelection);

	//This is a C++ method definition for creating Epic Online Services Lobbies
	UFUNCTION(BlueprintCallable,DisplayName="Create EOS Lobby", Category="Modified EOS || Sessions")
	void CreateEOSLobby( const FBP_CreateLobby_Callback& Result, TMap<FString, FString> Custom_Settings, FString SessionName = "Modified_EOS_Session",
							  bool bUseVoiceChat = true,
							  bool bUsePresence = true,
							  bool bAllowInvites = true,
							  bool bAdvertise = true,
							  bool bAllowJoinInProgress = true, 
							  bool bIsLan = false, 
							  int32 NumberOfPublicConnections = 4,
							  int32 NumberOfPrivateConnections = 4);

	//This is a C++ method definition for finding Epic Online Services Sessions
	UFUNCTION(BlueprintCallable,DisplayName="Find EOS Session", Category="Modified EOS || Sessions")
	void FindEOSSession( const FBP_FindSession_Callback& Result, TMap<FString, FString> Search_Settings, EMatchType MatchType = EMatchType::MT_Lobby, ERegionInfo RegionToSearch = ERegionInfo::RE_NoSelection);

	//This is a C++ method definition for finding Epic Online Services Sessions
	UFUNCTION(BlueprintCallable,DisplayName="Destroy EOS Session", Category="Modified EOS || Sessions")
	void DestroyEosSession( const FBP_DestroySession_Callback& Result, FName SessionName);

	//This is a C++ method definition for finding Epic Online Services Sessions
	UFUNCTION(BlueprintCallable,DisplayName="Join EOS Session", Category="EOS Integration Kit || Sessions")
	void JoinEosSession( const FBP_JoinSession_Callback& Result, FName SessionName, bool bIsDedicatedServerSession, FBlueprintSessionResult SessionResult);
	
	//Local Functions
	void Login(int32 LocalUserNum, FString ID, FString Token, FString Type, const FBP_Login_Callback& Result);
	//Return Functions
	void LoginCallback(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error) const;
	void LogoutCallback(int32 LocalUserNum,bool bWasSuccess) const;
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful) const;
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnCreateLobbyCompleted(FName SessionName, bool bWasSuccessful) const;
	void OnFindSessionCompleted(bool bWasSuccess) const;
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccess) const;



	//Variables
	FBP_Login_Callback LoginCallBackBP;
	FBP_Logout_Callback LogoutCallbackBP;
	FBP_CreateSession_Callback CreateSession_CallbackBP;
	FBP_CreateLobby_Callback CreateLobby_CallbackBP;
	FBP_JoinSession_Callback JoinSession_CallbackBP;
	FBP_FindSession_Callback FindSession_CallbackBP;
	FBP_DestroySession_Callback DestroySession_CallbackBP;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FString LocalPortInfo;
	bool Local_bIsDedicatedServerSession;

};






