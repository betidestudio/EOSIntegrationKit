//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EIK_BlueprintFunctions.generated.h"

UENUM(BlueprintType)
enum class EEIK_LoginStatus : uint8
{
	/** Player has not logged in or chosen a local profile */
	NotLoggedIn,
	/** Player is using a local profile but is not logged in */
	UsingLocalProfile,
	/** Player has been validated by the platform specific authentication service */
	LoggedIn,
};
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_BlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Extra", meta=( WorldContext = "Context" ))
	static FString GetEpicAccountId(UObject* Context);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Extra", meta=( WorldContext = "Context" ))
	static FString GetProductUserID(UObject* Context);

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Friends")
	static bool ShowFriendsList();

	// This is a C++ method definition for starting lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="Start EIK Session", Category="EOS Integration Kit || Sessions")
	static bool StartSession(FName SessionName);

	// This is a C++ method definition for registering players in lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="Register EIK Player In Session", Category="EOS Integration Kit || Sessions")
	static bool RegisterPlayer(FName SessionName, FEIKUniqueNetId PlayerId, bool bWasInvited = false);

	// This is a C++ method definition for unregistering players from lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="Unregister EIK Player In Session", Category="EOS Integration Kit || Sessions")
	static bool UnRegisterPlayer(FName SessionName, FEIKUniqueNetId PlayerId);

	// This is a C++ method definition for ending lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="End EIK Session", Category="EOS Integration Kit || Sessions")
	static bool EndSession(FName SessionName);

	// This is a C++ method definition for getting the nickname of a player from an online subsystem.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/getplayernickname
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Extra")
	static FString GetPlayerNickname(const int32 LocalUserNum);

	// This is a C++ method definition for getting the login status of a player from an online subsystem.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/getloginstatus
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Extra")
	static EEIK_LoginStatus GetLoginStatus(const int32 LocalUserNum);

	// This is a C++ method definition for purchasing an item from the store.
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra")
	static FString GenerateSessionCode(int32 CodeLength = 9);
};
