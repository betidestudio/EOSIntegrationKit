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
	UFUNCTION(BlueprintCallable, DisplayName="Intialize EOS Voice Chat", Category="EVIK")
	static bool InitializeEOSVoiceChat(APlayerController* PlayerController);

    // Connects the player to the EOS Voice Chat system.
	UFUNCTION(BlueprintCallable, DisplayName="Connect to EOS Voice Chat", Category="EVIK", meta = (AutoCreateRefTerm = "Result"))
	static void ConnectVoiceChat(APlayerController* PlayerController, const FEIKResultDelegate& Result);

    // Logs the player into the EOS Voice Chat system.
	UFUNCTION(BlueprintCallable, DisplayName="Login to EOS Voice Chat", Category="EVIK", meta = (AutoCreateRefTerm = "Result"))
	static void LoginEOSVoiceChat(APlayerController* PlayerController, FString PlayerName, const FEIKResultDelegate& Result);

    // Logs the player out of the EOS Voice Chat system.
	UFUNCTION(BlueprintCallable, DisplayName="Logout from EOS Voice Chat", Category="EVIK", meta = (AutoCreateRefTerm = "Result"))
	static void LogoutEOSVoiceChat(APlayerController* PlayerController, FString PlayerName, const FEIKResultDelegate& Result);

    // Gets the logged-in user's name for the EOS Voice Chat system.
	UFUNCTION(BlueprintPure, DisplayName="Get Logged in EOS Voice Chat User", Category="EVIK")
	static FString LoggedInUser(APlayerController* PlayerController);
	
    // Requests a room token for the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Get EOS Voice Room Token", Category="EVIK", meta = (AutoCreateRefTerm = "Result"))
	static void EOSRoomToken(APlayerController* PlayerController, FString VoiceRoomName, FString PlayerName, FString ClientIP, const FEIKRoomTokenResultDelegate& Result);
	
    // Joins the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Join EOS Voice Room", Category="EVIK", meta = (AutoCreateRefTerm = "Result"))
	static void JoinEOSRoom(APlayerController* PlayerController, FString VoiceRoomName, FString RoomData, bool bEnableEcho, const FEIKResultDelegate& Result);

    // Leaves the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Leave EOS Voice Room", Category="EVIK", meta = (AutoCreateRefTerm = "Result"))
	static void LeaveEOSRoom(APlayerController* PlayerController, FString VoiceRoomName, const FEIKResultDelegate& Result);

    // Gets a list of players in the specified voice room.
	UFUNCTION(BlueprintCallable, DisplayName="Get Players in EOS Voice Room", Category="EVIK")
	static TArray<FString> GetPlayersInRoom(APlayerController* PlayerController, FString VoiceRoomName);

    // Gets a list of all joined voice rooms.
	UFUNCTION(BlueprintCallable, DisplayName="Get Joined EOS Voice Rooms", Category="EVIK")
	static TArray<FString> GetAllRooms(APlayerController* PlayerController);

    // Gets the volume for the specified player.
	UFUNCTION(BlueprintPure, DisplayName="Get EOS Voice Player Volume", Category="EVIK")
	static float GetPlayerVolume(const APlayerController* PlayerController, const FString& PlayerName);

	//UFUNCTION(BlueprintPure, DisplayName="Get EOS Voice ProductUserID", Category="EVIK")
	static char* GetProductUserID(const FString& PlayerName);
	
    // Sets the volume for the specified player.
	UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Player Volume", Category="EVIK")
	static bool SetPlayerVolume(const APlayerController* PlayerController, const FString& PlayerName, float NewVolume);

	    // Checks if the specified player is muted in EOS Voice Chat.
    UFUNCTION(BlueprintPure, DisplayName="Is EOS Voice Player Muted", Category="EVIK")
    static bool IsPlayerMuted(const APlayerController* PlayerController, const FString& PlayerName);

    // Sets the specified player's mute status in EOS Voice Chat.
    UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Player Muted", Category="EVIK")
    static bool SetPlayerMuted(const APlayerController* PlayerController, const FString& PlayerName, bool MutePlayer);

    // Transmits the player's voice to all joined EOS Voice Rooms.
    UFUNCTION(BlueprintCallable, DisplayName="Transmit Voice To All EOS Voice Rooms", Category="EVIK")
    static bool TransmitToAllRooms(const APlayerController* PlayerController);

    // Transmits the player's voice to a selected EOS Voice Room.
    UFUNCTION(BlueprintCallable, DisplayName="Transmit Voice To Selected EOS Voice Room", Category="EVIK")
    static bool TransmitToSelectedRoom(const APlayerController* PlayerController, FString RoomName);

    // Stops transmitting the player's voice to any EOS Voice Room.
    UFUNCTION(BlueprintCallable, DisplayName="Transmit Voice To No EOS Voice Room", Category="EVIK")
    static bool TransmitToNoRoom(const APlayerController* PlayerController);

    // Retrieves all available input methods for EOS Voice Chat.
    UFUNCTION(BlueprintCallable, DisplayName="Get All EOS Voice Input Methods", Category="EVIK")
    static TArray<FDeviceEVIKSettings> GetInputMethods(const APlayerController* PlayerController);

    // Retrieves all available output methods for EOS Voice Chat.
    UFUNCTION(BlueprintCallable, DisplayName="Get All EOS Voice Output Methods", Category="EVIK")
    static TArray<FDeviceEVIKSettings> GetOutputMethods(const APlayerController* PlayerController);

    // Sets the EOS Voice Chat output method based on the provided method ID.
    UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Output Method", Category="EVIK")
    static bool SetOutputMethods(const APlayerController* PlayerController, FString MethodID);

    // Sets the EOS Voice Chat input method based on the provided method ID.
    UFUNCTION(BlueprintCallable, DisplayName="Set EOS Voice Output Method", Category="EVIK")
    static bool SetInputMethods(const APlayerController* PlayerController, FString MethodID);

	
};
