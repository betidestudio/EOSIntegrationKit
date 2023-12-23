//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIK_BlueprintFunctions.h"

#include "EIKSettings.h"
#include "HttpModule.h"
#include "Engine/GameInstance.h"
#include "Containers/Array.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/Base64.h"
#include "OnlineSubsystemEIK/AsyncFunctions/Login/EIK_Login_AsyncFunction.h"

FString UEIK_BlueprintFunctions::GetEpicAccountId(UObject* Context)
{
	if(Context)
	{
		if(!Context->GetWorld())
		{
			return FString();
		}
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if(const TSharedPtr<const FUniqueNetId> EIK_NetID = UGameplayStatics::GetGameInstance(Context->GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId())
				{
					const FString String_UserID = EIK_NetID.Get()->ToString();
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
	else
	{
		return FString();
	}
}

FString UEIK_BlueprintFunctions::GetProductUserID(UObject* Context)
{
	if(Context)
	{
		if(!Context->GetWorld())
		{
			return FString();
		}
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				if(const TSharedPtr<const FUniqueNetId> EIK_NetID = UGameplayStatics::GetGameInstance(Context->GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId())
				{
					const FString String_UserID = EIK_NetID.Get()->ToString();
					TArray<FString> Substrings;
					String_UserID.ParseIntoArray(Substrings, TEXT("|"));
					// Check if the split was successful
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
	else
	{
		return FString();
	}
}

bool UEIK_BlueprintFunctions::ShowFriendsList()
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

bool UEIK_BlueprintFunctions::StartSession(FName SessionName)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			return SessionPtrRef->StartSession(SessionName);
		}
	}
	return false;
}

bool UEIK_BlueprintFunctions::RegisterPlayer(FName SessionName, FEIKUniqueNetId PlayerId, bool bWasInvited)
{
	if(!PlayerId.UniqueNetId.IsValid())
	{
		return false;
	}
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				return SessionPtrRef->RegisterPlayer(SessionName, PlayerId.UniqueNetId.ToSharedRef().Get(),bWasInvited);
			}
		}
	}
	return false;
}

bool UEIK_BlueprintFunctions::UnRegisterPlayer(FName SessionName, FEIKUniqueNetId PlayerId)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				return SessionPtrRef->UnregisterPlayer(SessionName, PlayerId.UniqueNetId.ToSharedRef().Get());
			}
		}
	}
	return false;
}

bool UEIK_BlueprintFunctions::EndSession(FName SessionName)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			return SessionPtrRef->EndSession(SessionName);
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

bool UEIK_BlueprintFunctions::IsInSession(FName SessionName, FEIKUniqueNetId PlayerId)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerId.UniqueNetId.IsValid() %s"), PlayerId.UniqueNetId.IsValid() ? TEXT("true") : TEXT("false"));
			return SessionPtrRef->IsPlayerInSession(SessionName, PlayerId.UniqueNetId.ToSharedRef().Get());
		}
	}
	return false;
}

FString UEIK_BlueprintFunctions::GetPlayerNickname(const int32 LocalUserNum)
{
	FString Nickname;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			Nickname =  IdentityPointerRef->GetPlayerNickname(LocalUserNum);
		}
	}
	if(Nickname.IsEmpty())
	{
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::GetByPlatform(false))
		{
			if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
			{
				Nickname =  IdentityPointerRef->GetPlayerNickname(LocalUserNum);
			}
		}
	}
	return Nickname;
}

EEIK_LoginStatus UEIK_BlueprintFunctions::GetLoginStatus(const int32 LocalUserNum)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			switch (IdentityPointerRef->GetLoginStatus(LocalUserNum))
			{
			case ELoginStatus::NotLoggedIn:
				return EEIK_LoginStatus::NotLoggedIn;
				break;
			case ELoginStatus::UsingLocalProfile:
				return EEIK_LoginStatus::UsingLocalProfile;
				break;
			case ELoginStatus::LoggedIn:
				return EEIK_LoginStatus::LoggedIn;
				break;
			default:
				return EEIK_LoginStatus::NotLoggedIn;
			}
		}
		return EEIK_LoginStatus::NotLoggedIn;
	}
	return EEIK_LoginStatus::NotLoggedIn;
}

FString UEIK_BlueprintFunctions::GenerateSessionCode(int32 CodeLength)
{
	FString SessionCode;

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

bool UEIK_BlueprintFunctions::IsEIKActive()
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		const FName ActiveSubsystemName = OnlineSubsystem->GetSubsystemName();
		if(ActiveSubsystemName=="EIK")
		{
			return true;
		}
	}
	return false;
}

FName UEIK_BlueprintFunctions::GetActiveSubsystem()
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		return  OnlineSubsystem->GetSubsystemName();
	}
	return FName();
}

ELoginTypes UEIK_BlueprintFunctions::GetActivePlatformSubsystem()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::GetByPlatform(false))
	{
		if(SubsystemRef->GetSubsystemName() == TEXT("STEAM"))
		{
			return ELoginTypes::Steam;
		}
		else if(SubsystemRef->GetSubsystemName() == TEXT("GOOGLEPLAY"))
		{
			return ELoginTypes::Google;
		}
	}
	return ELoginTypes::None;
}

FString UEIK_BlueprintFunctions::ByteArrayToString(const TArray<uint8>& DataToConvert)
{
	FString Result = FBase64::Encode(DataToConvert.GetData(), DataToConvert.Num());
	return Result;
}

TArray<uint8> UEIK_BlueprintFunctions::StringToByteArray(const FString& DataToConvert)
{
	TArray<uint8> Result;
	FBase64::Decode(DataToConvert, Result);
	return Result;
}

FEIKUniqueNetId UEIK_BlueprintFunctions::GetUserUniqueID(const APlayerController* PlayerController, bool& bIsValid)
{
	if(const APlayerState* PlayerState = PlayerController->PlayerState; !PlayerState)
	{
		bIsValid = false;
	}
	else
	{
		if(const TSharedPtr<const FUniqueNetId> EIK_NetID = PlayerState->GetUniqueId().GetUniqueNetId())
		{
			FEIKUniqueNetId LocalUNetID;
			LocalUNetID.SetUniqueNetId(EIK_NetID);
			if(LocalUNetID.IsValid())
			{
				bIsValid = true;
				return LocalUNetID;
			}
		}
	}
	
	const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(PlayerController->GetWorld());
	if(!IdentityInterface.IsValid())
	{
		bIsValid = false;
		return FEIKUniqueNetId();
	}
	if(const TSharedPtr<const FUniqueNetId> EIK_NetID = IdentityInterface->GetUniquePlayerId(0))
	{
		FEIKUniqueNetId LocalUNetID;
		LocalUNetID.SetUniqueNetId(EIK_NetID);
		if(LocalUNetID.IsValid())
		{
			bIsValid = true;
			return LocalUNetID;
		}
	}
	return FEIKUniqueNetId();
}

FString UEIK_BlueprintFunctions::GetEOSSDKVersion()
{
	return EOS_GetVersion();
}

bool UEIK_BlueprintFunctions::IsValidSession(FSessionFindStruct Session)
{
	if(Session.bIsDedicatedServer)
	{
		return true;
	}
	if(Session.SessionResult.OnlineResult.Session.NumOpenPublicConnections > 0 || Session.SessionResult.OnlineResult.Session.NumOpenPrivateConnections > 0)
	{
		return true;
	}
	return false;
}

bool UEIK_BlueprintFunctions::Initialize_EIK_For_Friends(APlayerController* PlayerController)
{
	if (UEIK_Subsystem* EIK_Subsystem = PlayerController->GetGameInstance()->GetSubsystem<UEIK_Subsystem>())
	{
		return EIK_Subsystem->InitializeEIK();
	}
	return false;
}

FString UEIK_BlueprintFunctions::GetCurrentPort(AGameModeBase* CurrentGameMode)
{
	if(!CurrentGameMode)
	{
		return FString();
	}
	return FString::FromInt(CurrentGameMode->GetWorld()->URL.Port);
}

void UEIK_BlueprintFunctions::MakeSanctionAppeal(FString AccessToken, EEOSSanctionType Reason,const FOnResponseFromSanctions& OnResponseFromSanctions)
{
	FEOSArtifactSettings ArtifactSettings;
	UEIKSettings::GetSettingsForArtifact(UEIKSettings::GetSettings().DefaultArtifactName, ArtifactSettings);
	FString DeploymentId = ArtifactSettings.DeploymentId;
	FString ApiEndpoint = FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/%s/appeals"), *DeploymentId);

	int32 ReasonInt = 0;
	switch (Reason)
	{
	case EEOSSanctionType::IncorrectSanction:
		ReasonInt = 1;
		break;
	case EEOSSanctionType::CompromisedAccount:
		ReasonInt = 2;
		break;
	case EEOSSanctionType::UnfairPunishment:
		ReasonInt = 3;
		break;
	case EEOSSanctionType::AppealForForgiveness:
		ReasonInt = 4;
		break;
	default:
		ReasonInt = 4;
		break;
	}
	
	// Generate a random referenceId
	FGuid RandomGuid = FGuid::NewGuid();
	FString RandomReferenceId = RandomGuid.ToString();
	TSharedPtr<FJsonObject> RequestData = MakeShareable(new FJsonObject);
    RequestData->SetStringField("referenceId", RandomReferenceId);
	RequestData->SetNumberField("reason", ReasonInt);

	FString JsonRequestData;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonRequestData);
	FJsonSerializer::Serialize(RequestData.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(ApiEndpoint);
	HttpRequest->SetContentAsString(JsonRequestData);
	
	HttpRequest->OnProcessRequestComplete().BindLambda([OnResponseFromSanctions](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful && Response.IsValid())
		{
			int32 ResponseCode = Response->GetResponseCode();
			FString ResponseContent = Response->GetContentAsString();
			if (ResponseCode == 200)
			{
				// Successfully created the appeal, parse the response JSON
				TSharedPtr<FJsonObject> JsonResponse;
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseContent);

				if (FJsonSerializer::Deserialize(JsonReader, JsonResponse))
				{
					OnResponseFromSanctions.ExecuteIfBound(true);
					return;
				}
			}
		}
		OnResponseFromSanctions.ExecuteIfBound(false);
	});
	HttpRequest->ProcessRequest();
}

void UEIK_BlueprintFunctions::RequestEOSAccessToken(const FOnResponseFromEpicForAccessToken& Response)
{
	FString ProductId, SandboxId, DeploymentId, ClientId, ClientSecret, EncryptionKey;
	FEOSArtifactSettings ArtifactSettings;
	UEIKSettings::GetSettingsForArtifact(UEIKSettings::GetSettings().DefaultArtifactName, ArtifactSettings);
	ProductId = ArtifactSettings.ProductId;
	SandboxId = ArtifactSettings.SandboxId;
	DeploymentId = ArtifactSettings.DeploymentId;
	ClientId = ArtifactSettings.ClientId;
	ClientSecret = ArtifactSettings.ClientSecret;
	if (ProductId.IsEmpty() || SandboxId.IsEmpty() || DeploymentId.IsEmpty() || ClientId.IsEmpty() || ClientSecret.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Missing values in DefaultEngine.ini"));
		Response.ExecuteIfBound(false, "Error");
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
		FString AccessTokenString;
		TSharedPtr<FJsonObject> JsonObject;

		if (bConnectedSuccessfully && FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ResponsePtr->GetContentAsString()), JsonObject) && JsonObject.IsValid())
		{
			TSharedPtr<FJsonValue> AccessTokenObject = JsonObject->TryGetField(TEXT("access_token"));
			AccessTokenString = AccessTokenObject ? AccessTokenObject->AsString() : FString();
			Response.ExecuteIfBound(true, AccessTokenString);
		}
		else
		{
			Response.ExecuteIfBound(false, "Error");
			return;
		}
	});
	HttpRequest->ProcessRequest();
}
