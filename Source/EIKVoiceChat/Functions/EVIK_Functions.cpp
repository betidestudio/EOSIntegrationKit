// Copyright 2023 Betide Studio. All rights reserved.


#include "EVIK_Functions.h"
#include "Runtime\Engine/Classes/GameFramework/PlayerController.h"
#include "Interfaces/IHttpResponse.h"


bool UEVIK_Functions::InitializeEOSVoiceChat(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				return LocalVoiceSubsystem->EVIK_Local_Initialize();
			}
		}
	}
	return false;
}

void UEVIK_Functions::ConnectVoiceChat(APlayerController* PlayerController, const FEIKResultDelegate& Result)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if(LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->Connect(FOnVoiceChatConnectCompleteDelegate::CreateLambda([Result](const FVoiceChatResult& Result1)
					{
						if(Result1.IsSuccess())
						{
							Result.ExecuteIfBound(true, EEVIKResultCodes::Success);
						}
						else
						{
							Result.ExecuteIfBound(false, EEVIKResultCodes::Failed);
						}
					}
					));
				}	
			}
		}
	}
}

void UEVIK_Functions::LoginEOSVoiceChat(APlayerController* PlayerController, FString PlayerName, const FEIKResultDelegate& Result)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if(LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					//LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->Login(0, PlayerName, "", FOnVoiceChatLoginCompleteDelegate::CreateLambda([Result](const FString& PlayerName, const FVoiceChatResult& Result1)
					{
						if(Result1.IsSuccess())
						{
							EOS_ProductUserId ProductUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*PlayerName));
							if(ProductUserId != nullptr)
							{
								UE_LOG(LogTemp, Warning, TEXT("ProductUserId: %p"), ProductUserId);
							}
							UE_LOG(LogTemp, Warning, TEXT("Player Name: %s"), *PlayerName);
							Result.ExecuteIfBound(true, EEVIKResultCodes::Success);
						}
						else
						{
							Result.ExecuteIfBound(false, EEVIKResultCodes::Failed);
						}
					}
					));
				}	
			}
		}
	}
}

void UEVIK_Functions::LogoutEOSVoiceChat(APlayerController* PlayerController, FString PlayerName,
	const FEIKResultDelegate& Result)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if(LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->Logout(FOnVoiceChatLogoutCompleteDelegate::CreateLambda([Result](const FString& PlayerName, const FVoiceChatResult& Result1)
					{
						if(Result1.IsSuccess())
						{
							Result.ExecuteIfBound(true, EEVIKResultCodes::Success);
						}
						else
						{
							Result.ExecuteIfBound(false, EEVIKResultCodes::Failed);
						}
					}
					));
				}	
			}
		}
	}
}

FString UEVIK_Functions::LoggedInUser(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if(LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					FString PlayerName = LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->GetLoggedInPlayerName();
					UE_LOG(LogTemp, Warning, TEXT("Playername is %s"), *PlayerName);
					return PlayerName;
				}
			}
		}
	}
	return FString();
}

#include "Misc/ConfigCacheIni.h"
#include "Http.h"
#include "Json.h"

void UEVIK_Functions::EOSRoomToken(APlayerController* PlayerController, FString VoiceRoomName, FString PlayerName, FString ClientIP, const FEIKRoomTokenResultDelegate& Result)
{
    FString ProductId, SandboxId, DeploymentId, ClientId, ClientSecret, EncryptionKey;
    bool bEnabled;
    if (!GConfig->GetBool(TEXT("EOSVoiceChat"), TEXT("bEnabled"), bEnabled, GEngineIni) || !bEnabled)
    {
    	Result.ExecuteIfBound(false, "Error");
    	UE_LOG(LogTemp, Warning, TEXT("Code 14"));
        return;
    }

    bool bSuccess = true;
    bSuccess &= GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("ProductId"), ProductId, GEngineIni);
    bSuccess &= GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("SandboxId"), SandboxId, GEngineIni);
    bSuccess &= GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("DeploymentId"), DeploymentId, GEngineIni);
    bSuccess &= GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("ClientId"), ClientId, GEngineIni);
    bSuccess &= GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("ClientSecret"), ClientSecret, GEngineIni);

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing values in DefaultEngine.ini"));
    	Result.ExecuteIfBound(false, "Error");
        return;
    }

    auto HttpRequest = FHttpModule::Get().CreateRequest();
    const FString Base64Credentials = FBase64::Encode(FString::Printf(TEXT("%s:%s"), *ClientId, *ClientSecret));
    const FString ContentString = FString::Printf(TEXT("grant_type=client_credentials&deployment_id=%s"), *DeploymentId);

    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
    HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
    HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Basic %s"), *Base64Credentials));
    HttpRequest->SetContentAsString(ContentString);
    HttpRequest->SetURL("https://api.epicgames.dev/auth/v1/oauth/token");
    HttpRequest->SetVerb("POST");

    HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool bConnectedSuccessfully)
    {
    	UE_LOG(LogTemp, Warning, TEXT("Response -> %s"), *ResponsePtr->GetContentAsString());
        FString AccessTokenString;
        TSharedPtr<FJsonObject> JsonObject;

        if (bConnectedSuccessfully && FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ResponsePtr->GetContentAsString()), JsonObject) && JsonObject.IsValid())
        {
            TSharedPtr<FJsonValue> AccessTokenObject = JsonObject->TryGetField(TEXT("access_token"));
            AccessTokenString = AccessTokenObject ? AccessTokenObject->AsString() : FString();
        }
        else
        {
        	UE_LOG(LogTemp, Warning, TEXT("Code 1"));
        	Result.ExecuteIfBound(false, "Error");
        	return;
        }

		if (!AccessTokenString.IsEmpty())
		{
			auto RoomTokenRequest = FHttpModule::Get().CreateRequest();

			RoomTokenRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
			RoomTokenRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
			RoomTokenRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessTokenString));

			const FString ProductUserId = PlayerName;
			bool bHardMuted = false; 

			const FString JsonRequestString = FString::Printf(TEXT("{\"participants\":[{\"puid\":\"%s\",\"clientIp\":\"%s\",\"hardMuted\":%s}] }"), *ProductUserId, *ClientIP, bHardMuted ? TEXT("true") : TEXT("false"));
			UE_LOG(LogTemp, Warning, TEXT("Fd -> %s"), *JsonRequestString);
			RoomTokenRequest->SetContentAsString(JsonRequestString);
			RoomTokenRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/rtc/v1/%s/room/%s"), *DeploymentId, *VoiceRoomName));
			RoomTokenRequest->SetVerb("POST");

			RoomTokenRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
			{
				UE_LOG(LogTemp, Warning, TEXT("Response -> %s"), *HttpResponsePtr->GetContentAsString());
				FString TokenString, ClientBaseUrlString;

				if (bConnectedSuccessfully)
				{
					TSharedPtr<FJsonObject> JsonObject;
					const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());
					UE_LOG(LogTemp, Warning, TEXT("roken d2ew2e232sds"));
					if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
					{
						const TSharedPtr<FJsonValue> ClientBaseUrlObject = JsonObject->TryGetField(TEXT("clientBaseUrl"));
						const TSharedPtr<FJsonValue> ParticipantsObject = JsonObject->TryGetField(TEXT("participants"));
						UE_LOG(LogTemp, Warning, TEXT("roken dsds"));

						if (ParticipantsObject)
						{
							ClientBaseUrlString = ClientBaseUrlObject->AsString();
							UE_LOG(LogTemp, Warning, TEXT("ClientUrl -> %s"), *ClientBaseUrlString);
							TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ParticipantsObject->AsArray();

							for (const auto& Element : ParticipantsArray)
							{
								if (Element->Type != EJson::Object)
									continue;

								auto& Object = Element->AsObject();
								TokenString = Object->TryGetField("token")->AsString();
								UE_LOG(LogTemp, Warning, TEXT("Token -> %s"), *TokenString);
							}
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Code 17"));
						Result.ExecuteIfBound(false, "Error");
						return;
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Code 97"));
					Result.ExecuteIfBound(false, "Error");
					return;
				}

				if (!TokenString.IsEmpty())
				{
					FEVIKChannelCredentials ChannelCredentials;
					ChannelCredentials.OverrideUserId = PlayerName;
					ChannelCredentials.ClientBaseUrl = ClientBaseUrlString;
					ChannelCredentials.ParticipantToken = TokenString;
					Result.ExecuteIfBound(true, ChannelCredentials.ToJson(false));
					return;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Code 1345"));
					Result.ExecuteIfBound(false, "Error");
				}
			});

			RoomTokenRequest->ProcessRequest();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Code 12241"));
			Result.ExecuteIfBound(false, "Error");
		}
    });

    HttpRequest->ProcessRequest();
}

void UEVIK_Functions::JoinEOSRoom(APlayerController* PlayerController, FString VoiceRoomName, FString ChannelCredentialsJson,bool bEnableEcho, const FEIKResultDelegate& Result)
{
    if (PlayerController)
    {
        if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
        {
            UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
            if (LocalVoiceSubsystem)
            {
                if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
                {
                    FVoiceChatChannel3dProperties Properties;
                    Properties.AttenuationModel = EVoiceChatAttenuationModel::InverseByDistance;
                    Properties.MaxDistance = 100.f;
                    Properties.MinDistance = 1.f;
                    Properties.Rolloff = 0.5f;

                    TSharedPtr<FJsonObject> ChannelCredentialsObject;
                    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ChannelCredentialsJson);
                    if (FJsonSerializer::Deserialize(JsonReader, ChannelCredentialsObject) && ChannelCredentialsObject.IsValid())
                    {
                        FString ClientBaseUrl = ChannelCredentialsObject->GetStringField(TEXT("ClientBaseUrl"));
                        FString ParticipantToken = ChannelCredentialsObject->GetStringField(TEXT("ParticipantToken"));
                    	EVoiceChatChannelType ChannelType = EVoiceChatChannelType::NonPositional;
                    	if(bEnableEcho)
                    	{
                    		ChannelType = EVoiceChatChannelType::Echo;
                    		UE_LOG(LogTemp, Warning, TEXT("Setting Echo -> %hhd"), bEnableEcho);
                    	}
                    	UE_LOG(LogTemp, Warning, TEXT("bEnableEcho -> %hhd"), bEnableEcho);

                        LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->JoinChannel(VoiceRoomName, ChannelCredentialsJson, ChannelType, FOnVoiceChatChannelJoinCompleteDelegate::CreateLambda([Result](const FString& ChannelName, const FVoiceChatResult& JoinResult)
                        {
                        	if(JoinResult.IsSuccess())
                        	{
								Result.ExecuteIfBound(true,EEVIKResultCodes::Success);
							}
							else
							{
								Result.ExecuteIfBound(false,EEVIKResultCodes::Failed);
							}
                        }), Properties);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to parse ChannelCredentialsJson"));
                    }
                }
            }
        }
    }
}

void UEVIK_Functions::LeaveEOSRoom(APlayerController* PlayerController, FString VoiceRoomName,
	const FEIKResultDelegate& Result)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->LeaveChannel(VoiceRoomName,FOnVoiceChatChannelLeaveCompleteDelegate::CreateLambda([Result](const FString& ChannelName, const FVoiceChatResult& JoinResult)
						{
							if(JoinResult.IsSuccess())
							{
								Result.ExecuteIfBound(true,EEVIKResultCodes::Success);
							}
							else
							{
								Result.ExecuteIfBound(false,EEVIKResultCodes::Failed);
							}
						}));
				}
			}
		}
	}
}

TArray<FString> UEVIK_Functions::GetPlayersInRoom(APlayerController* PlayerController, FString VoiceRoomName)
{
	TArray<FString> Local_PlayersInRoom;
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					Local_PlayersInRoom = LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->GetPlayersInChannel(VoiceRoomName);
				}
			}
		}
	}
	return Local_PlayersInRoom;
}

TArray<FString> UEVIK_Functions::GetAllRooms(APlayerController* PlayerController)
{
	TArray<FString> Local_PlayersJoinedRooms;
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					Local_PlayersJoinedRooms = LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->GetChannels();
				}
			}
		}
	}
	return Local_PlayersJoinedRooms;
}

float UEVIK_Functions::GetPlayerVolume(const APlayerController* PlayerController, const FString& PlayerName)
{
	float Local_PlayerVolume = 0;
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					Local_PlayerVolume = LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->GetPlayerVolume(PlayerName);
				}
			}
		}
	}
	return Local_PlayerVolume;
}

char* UEVIK_Functions::GetProductUserID(const FString& PlayerName)
{
	EOS_ProductUserId ProductUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*PlayerName));
	if(ProductUserId != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProductUserId: %p"), ProductUserId);
	}
	char* Local_ProductUserID = nullptr;
	int32_t myInt = 128;
	EOS_ProductUserId_ToString(ProductUserId, Local_ProductUserID,&myInt);

	return Local_ProductUserID;
}

bool UEVIK_Functions::SetPlayerVolume(const APlayerController* PlayerController, const FString& PlayerName,
                                      float NewVolume)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->SetPlayerVolume(PlayerName, NewVolume);
					return true;
				}
			}
		}
	}
	return false;
}

bool UEVIK_Functions::IsPlayerMuted(const APlayerController* PlayerController, const FString& PlayerName)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					return LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->IsPlayerMuted(PlayerName);
				}
			}
		}
	}
	return false;
}

bool UEVIK_Functions::SetPlayerMuted(const APlayerController* PlayerController, const FString& PlayerName, bool MutePlayer)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->SetPlayerMuted(PlayerName, MutePlayer);
					return true;
				}
			}
		}
	}
	return false;
}

bool UEVIK_Functions::TransmitToAllRooms(const APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->TransmitToAllChannels();
					return true;
				}
			}
		}
	}
	return false;
}

bool UEVIK_Functions::TransmitToSelectedRoom(const APlayerController* PlayerController, FString RoomName)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->TransmitToSpecificChannel(RoomName);
					return true;
				}
			}
		}
	}
	return false;
}

bool UEVIK_Functions::TransmitToNoRoom(const APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->TransmitToNoChannels();
					return true;
				}
			}
		}
	}
	return false;
}

TArray<FDeviceEVIKSettings> UEVIK_Functions::GetInputMethods(const APlayerController* PlayerController)
{
	TArray<FDeviceEVIKSettings> Local_DeviceSettingArray;
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					TArray<FVoiceChatDeviceInfo> AvailableMethods = LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->GetAvailableInputDeviceInfos();
					for(int i=0;i<AvailableMethods.Num();i++)
					{
						FDeviceEVIKSettings DeviceEvikSettings;
						DeviceEvikSettings.DisplayName = AvailableMethods[i].DisplayName;
						DeviceEvikSettings.ID = AvailableMethods[i].Id;
						Local_DeviceSettingArray.Add(DeviceEvikSettings);
					}
					return Local_DeviceSettingArray;
				}
			}
		}
	}
	return Local_DeviceSettingArray;
}

TArray<FDeviceEVIKSettings> UEVIK_Functions::GetOutputMethods(const APlayerController* PlayerController)
{
	TArray<FDeviceEVIKSettings> Local_DeviceSettingArray;
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					TArray<FVoiceChatDeviceInfo> AvailableMethods = LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->GetAvailableOutputDeviceInfos();
					for(int i=0;i<AvailableMethods.Num();i++)
					{
						FDeviceEVIKSettings DeviceEvikSettings;
						DeviceEvikSettings.DisplayName = AvailableMethods[i].DisplayName;
						DeviceEvikSettings.ID = AvailableMethods[i].Id;
						Local_DeviceSettingArray.Add(DeviceEvikSettings);
					}
					return Local_DeviceSettingArray;
				}
			}
		}
	}
	return Local_DeviceSettingArray;
}

bool UEVIK_Functions::SetOutputMethods(const APlayerController* PlayerController, FString MethodID)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->SetOutputDeviceId(MethodID);
					return true;
				}
			}
		}
	}
	return false;
}

bool UEVIK_Functions::SetInputMethods(const APlayerController* PlayerController, FString MethodID)
{
	if (PlayerController)
	{
		if (const UGameInstance* GameInstance = PlayerController->GetGameInstance())
		{
			UEIK_Voice_Subsystem* LocalVoiceSubsystem = GameInstance->GetSubsystem<UEIK_Voice_Subsystem>();
			if (LocalVoiceSubsystem)
			{
				if (LocalVoiceSubsystem->EVIK_Local_GetVoiceChat())
				{
					LocalVoiceSubsystem->EVIK_Local_GetVoiceChat()->SetInputDeviceId(MethodID);
					return true;
				}
			}
		}
	}
	return false;
}

