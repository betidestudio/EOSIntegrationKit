// Copyright 2023 Betide Studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VoiceChat.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Misc/Base64.h"
#include "Misc/ConfigCacheIni.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EIK_Voice_Subsystem.generated.h"

UENUM(BlueprintType)
enum class EEVIKResultCodes :uint8
{
	Success = 0,
	Failed = 1
};

struct FEVIKChannelCredentials : public FJsonSerializable
{
	FString OverrideUserId;
	FString ClientBaseUrl;
	FString ParticipantToken;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("override_userid", OverrideUserId);
	JSON_SERIALIZE("client_base_url", ClientBaseUrl);
	JSON_SERIALIZE("participant_token", ParticipantToken);
	END_JSON_SERIALIZER
};

USTRUCT(BlueprintType)
struct FDeviceEVIKSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EOS Integration Kit|Voice Chat")
	FString ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EOS Integration Kit|Voice Chat")
	FString DisplayName;
	
};

USTRUCT(BlueprintType)
struct FEVIKPlayerList
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EOS Integration Kit|Voice Chat")
	FString PlayerEOSVoiceChatName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EOS Integration Kit|Voice Chat")
	AActor* PlayerActor = nullptr;
	
};


USTRUCT(BlueprintType)
struct FEIK_PositionalVoiceChat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EOS Integration Kit|Voice Chat")
	FString ChannelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EOS Integration Kit|Voice Chat")
	float MaxHearingDistance;	
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIKResultDelegate, bool, bWasSuccess, EEVIKResultCodes, Result);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIKRoomTokenResultDelegate, bool, bWasSuccess, FString, RoomData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_OnPlayerAdded, FString, ChannelName, FString, PlayerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_OnPlayerRemoved, FString, ChannelName, FString, PlayerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_OnChannelExited, FString, ChannelName, FString, Result);
/**
 * 
 */
UCLASS()
class EIKVOICECHAT_API UEIK_Voice_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(Category="EOS Integration Kit|Voice Chat")
	bool EVIK_Local_Initialize();
	
	IVoiceChat* EVIK_Local_GetVoiceChat();

	UFUNCTION(Category="EOS Integration Kit|Voice Chat")
	void EVIK_Local_Connect(const FEIKResultDelegate& ResultDelegate);

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit|Voice Chat")
	void UpdatePositionalVoiceChat(bool UsePositionalVoiceChat, TArray<FEIK_PositionalVoiceChat> PositionalVoiceChatData, bool bApplyPositionalVoiceChatOnAllChannels);

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit|Voice Chat")
	void SetDebugForPositionalVoiceChat(bool bEnableDebug);
	
	//void UpdatePositionalVoiceChat(bool UsePositionalVoiceChat, float FMaxHearingDistance, bool V_bUseDebugPoint, FVector V_DebugPointLocation);

	//UFUNCTION(BlueprintCallable, Category="EOS Integration Kit|Voice Chat")


	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit|Voice Chat")
	void ForceUpdateOutputVolume(bool bUseThisVolume, bool bUseWithPositionalChat,float Volume);
	FTimerHandle UpdatePositionalVoiceChatTimerHandle;

	UFUNCTION()
	void PlayerListUpdated();
	
	IVoiceChat* EVIK_VoiceChat;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Voice Chat")
	FEIK_OnPlayerRemoved OnPlayerRemoved;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Voice Chat")
	FEIK_OnPlayerAdded OnPlayerAdded;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Voice Chat")
	FEIK_OnChannelExited OnChannelExited;

	bool Var_IsPositionalVoiceChatUsed = false;
	TArray<FEIK_PositionalVoiceChat> Var_PositionalVoiceChatData;
	bool Var_ApplyPositionalVoiceChatOnAllChannels = false;
	bool bUseDebugPoint = false;

	float OutputVolume = 1.0f;
	bool bUseOutputVolume = false;
	bool bUseOutputVolumeWithPositionalChat = false;
};
