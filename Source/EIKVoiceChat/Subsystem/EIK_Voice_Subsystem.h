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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EVIK")
	FString ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EVIK")
	FString DisplayName;
	
};

USTRUCT(BlueprintType)
struct FEVIKPlayerList
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EVIK")
	FString PlayerEOSVoiceChatName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EVIK")
	AActor* PlayerActor = nullptr;
	
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIKResultDelegate, bool, bWasSuccess, EEVIKResultCodes, Result);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIKRoomTokenResultDelegate, bool, bWasSuccess, FString, RoomData);

/**
 * 
 */
UCLASS()
class EIKVOICECHAT_API UEIK_Voice_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(Category="EVIK")
	bool EVIK_Local_Initialize();
	
	IVoiceChat* EVIK_Local_GetVoiceChat();

	UFUNCTION(Category="EVIK")
	void EVIK_Local_Connect(const FEIKResultDelegate& ResultDelegate);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="EVIK")
	void SetupPlayerList(const TArray<FEVIKPlayerList>& PlayerList);

	UFUNCTION(BlueprintCallable, Category="EVIK")
	void UpdatePositionalVoiceChat(bool UsePositionalVoiceChat, float FMaxHearingDistance, bool V_bUseDebugPoint, FVector V_DebugPointLocation);

	UFUNCTION(BlueprintCallable, Category="EVIK")
	void ForceUpdateOutputVolume(bool bUseThisVolume, bool bUseWithPositionalChat,float Volume);
	FTimerHandle UpdatePositionalVoiceChatTimerHandle;

	UFUNCTION()
	void PlayerListUpdated();
	
	IVoiceChat* EVIK_VoiceChat;

	UPROPERTY(ReplicatedUsing= PlayerListUpdated)
	TArray<FEVIKPlayerList> PlayerListVar;

	bool bIsPositionalVoiceChatUsed = false;
	bool bUseDebugPoint = false;
	FVector DebugPointLocation;

	float MaxHearingDistance;
	float OutputVolume = 1.0f;
	bool bUseOutputVolume = false;
	bool bUseOutputVolumeWithPositionalChat = false;
};
