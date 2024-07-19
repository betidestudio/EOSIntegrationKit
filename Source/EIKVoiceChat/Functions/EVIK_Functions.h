// Copyright 2023 Betide Studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIKVoiceChat/Subsystem/EIK_Voice_Subsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "eos_common.h"

#include "EVIK_Functions.generated.h"

/**
 * 
 */
UCLASS()
// This class contains functions for the EOS Voice Integration Kit
class EIKVOICECHAT_API UEVIK_Functions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    // Initializes the EOS Voice Chat system.
	UFUNCTION(BlueprintCallable, DisplayName="Initialize EOS Voice Chat", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
	static bool InitializeEOSVoiceChat(const UObject* WorldContextObject);

    // Connects the player to the EOS Voice Chat system.
	UFUNCTION(BlueprintCallable, DisplayName="Connect to EOS Voice Chat", Category="EOS Integration Kit|Voice Chat", meta = (AutoCreateRefTerm = "Result", WorldContext="WorldContextObject"))
	static void ConnectVoiceChat(const UObject* WorldContextObject, const FEIKResultDelegate& Result);

	// Returns whether the EOS Voice Chat system is connected.
	UFUNCTION(BlueprintPure, DisplayName = "Is EOS Voice Chat Connected?", Category = "EOS Integration Kit|Voice Chat", meta = (WorldContext = "WorldContextObject"))
	static bool IsVoiceChatConnected(const UObject* WorldContextObject);

    // Logs the player into the EOS Voice Chat system.
	UFUNCTION(BlueprintCallable, DisplayName="Login to EOS Voice Chat", Category="EOS Integration Kit|Voice Chat", meta = (AutoCreateRefTerm = "Result", WorldContext="WorldContextObject"))
	static void LoginEOSVoiceChat(const UObject* WorldContextObject, FString PlayerName, const FEIKResultDelegate& Result);

    // Logs the player out of the EOS Voice Chat system.
	UFUNCTION(BlueprintCallable, DisplayName="Logout from EOS Voice Chat", Category="EOS Integration Kit|Voice Chat", meta = (AutoCreateRefTerm = "Result", WorldContext="WorldContextObject"))
	static void LogoutEOSVoiceChat(const UObject* WorldContextObject, FString PlayerName, const FEIKResultDelegate& Result);

	// Returns whether the EOS Voice Chat system is currently logging-in.
	UFUNCTION(BlueprintPure, DisplayName = "Is EOS Voice Chat Logging-In?", Category = "EOS Integration Kit|Voice Chat", meta = (WorldContext = "WorldContextObject"))
	static bool IsEOSVoiceChatLoggingIn(const UObject* WorldContextObject);

	// Returns whether the EOS Voice Chat system is logged-in.
	UFUNCTION(BlueprintPure, DisplayName = "Is EOS Voice Chat Logged-In?", Category = "EOS Integration Kit|Voice Chat", meta = (WorldContext = "WorldContextObject"))
	static bool IsEOSVoiceChatLoggedIn(const UObject* WorldContextObject);

    // Gets the logged-in user's name for the EOS Voice Chat system.
	UFUNCTION(BlueprintPure, DisplayName="Get Logged in EOS Voice Chat User", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
	static FString LoggedInUser(const UObject* WorldContextObject);
	
    // Requests a room token for the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Get EOS Voice Room Token", Category="EOS Integration Kit|Voice Chat", meta = (AutoCreateRefTerm = "Result", WorldContext="WorldContextObject"))
	static void EOSRoomToken(FString VoiceRoomName, FString PlayerName, FString ClientIP, const FEIKRoomTokenResultDelegate& Result);
	
    // Joins the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Join EOS Voice Room", Category="EOS Integration Kit|Voice Chat", meta = (AutoCreateRefTerm = "Result", WorldContext="WorldContextObject"))
	static void JoinEOSRoom(const UObject* WorldContextObject, FString VoiceRoomName, FString RoomData, bool bEnableEcho, const FEIKResultDelegate& Result);

    // Leaves the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Leave EOS Voice Room", Category="EOS Integration Kit|Voice Chat", meta = (AutoCreateRefTerm = "Result", WorldContext="WorldContextObject"))
	static void LeaveEOSRoom(const UObject* WorldContextObject, FString VoiceRoomName, const FEIKResultDelegate& Result);

    // Gets a list of players in the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Get Players in EOS Voice Room", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
	static TArray<FString> GetPlayersInRoom(const UObject* WorldContextObject, FString VoiceRoomName);

    // Gets a list of all joined voice rooms.
	UFUNCTION(BlueprintCallable, DisplayName="Get Joined EOS Voice Rooms", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
	static TArray<FString> GetAllRooms(const UObject* WorldContextObject);

    // Gets the volume for the specified player.
	UFUNCTION(BlueprintPure, DisplayName="Get EOS Voice Player Volume", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
	static float GetPlayerVolume(const UObject* WorldContextObject, const FString& PlayerName);

	//UFUNCTION(BlueprintPure, DisplayName="Get EOS Voice ProductUserID", Category="EOS Integration Kit|Voice Chat")
	static char* GetProductUserID(const FString& PlayerName);
	
    // Sets the volume for the specified player.
	UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Player Volume", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
	static bool SetPlayerVolume(const UObject* WorldContextObject, const FString& PlayerName, float NewVolume);

	// Checks if the specified player is muted in EOS Voice Chat.
    UFUNCTION(BlueprintPure, DisplayName="Is EOS Voice Player Muted", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static bool IsPlayerMuted(const UObject* WorldContextObject, const FString& PlayerName);

    // Sets the specified player's mute status in EOS Voice Chat.
    UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Player Muted", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static bool SetPlayerMuted(const UObject* WorldContextObject, const FString& PlayerName, bool MutePlayer);

    // Transmits the player's voice to all joined EOS Voice Rooms.
    UFUNCTION(BlueprintCallable, DisplayName="Transmit Voice To All EOS Voice Rooms", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static bool TransmitToAllRooms(const UObject* WorldContextObject);

    // Transmits the player's voice to a selected EOS Voice Room.
    UFUNCTION(BlueprintCallable, DisplayName="Transmit Voice To Selected EOS Voice Room", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static bool TransmitToSelectedRoom(const UObject* WorldContextObject, FString RoomName);

    // Stops transmitting the player's voice to any EOS Voice Room.
    UFUNCTION(BlueprintCallable, DisplayName="Transmit Voice To No EOS Voice Room", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static bool TransmitToNoRoom(const UObject* WorldContextObject);

    // Retrieves all available input methods for EOS Voice Chat.
    UFUNCTION(BlueprintCallable, DisplayName="Get All EOS Voice Input Methods", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static TArray<FDeviceEVIKSettings> GetInputMethods(const UObject* WorldContextObject);

    // Retrieves all available output methods for EOS Voice Chat.
    UFUNCTION(BlueprintCallable, DisplayName="Get All EOS Voice Output Methods", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static TArray<FDeviceEVIKSettings> GetOutputMethods(const UObject* WorldContextObject);

    // Sets the EOS Voice Chat output method based on the provided method ID.
    UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Output Method", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static bool SetOutputMethods(const UObject* WorldContextObject, FString MethodID);

    // Sets the EOS Voice Chat input method based on the provided method ID.
    UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Input Method", Category="EOS Integration Kit|Voice Chat", meta=(WorldContext="WorldContextObject"))
    static bool SetInputMethods(const UObject* WorldContextObject, FString MethodID);

	// Checks if specified player is talking
	UFUNCTION(BlueprintPure, DisplayName = "Is EOS Player Talking", Category = "EOS Integration Kit|Voice Chat", meta = (WorldContext = "WorldContextObject"))
	static bool IsPlayerTalking(const UObject* WorldContextObject, FString PlayerName);

	// Mutes or unmutes local input device, returnes true if success
	UFUNCTION(BlueprintCallable, DisplayName = "Set Is EOS Voice Input Method Muted", Category = "EOS Integration Kit|Voice Chat", meta = (WorldContext = "WorldContextObject"))
	static void MuteInputDevice(const UObject* WorldContextObject, bool Mute, bool& bWasSuccess);
};