// Copyright Epic Games, Inc. All Rights Reserved.

#include "EIK_BlueprintFunctions.h"

#include "EIKSettings.h"
#include "HttpModule.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "UserManagerEOS.h"
#include "Engine/GameInstance.h"
#include "Containers/Array.h"
#include "EIKVoiceChat/Private/Android/AndroidEOSVoiceChatUser.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Base64.h"
#include "OnlineSubsystemEIK/AsyncFunctions/Beacons/PingHost.h"
#include "OnlineSubsystemEIK/AsyncFunctions/Beacons/PingHostObject.h"
#include "OnlineSubsystemEIK/AsyncFunctions/Login/EIK_Login_AsyncFunction.h"

FString UEIK_BlueprintFunctions::GetEpicAccountId(UObject* Context)
{
	if(Context)
	{
		if(!Context->GetWorld())
		{
			return FString();
		}
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
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

FEIK_CurrentSessionInfo UEIK_BlueprintFunctions::GetCurrentSessionInfo(UObject* Context, bool& bIsSessionPresent,FName SessionName)
{
	bIsSessionPresent = false;
	if(Context)
	{
		if(!Context->GetWorld())
		{
			return FEIK_CurrentSessionInfo();
		}
		if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(Context->GetWorld(), "EIK"))
		{
			if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
			{
				if(!SessionPtrRef->GetNamedSession(SessionName))
				{
					UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetCurrentSessionInfo: Session not found"));
					return FEIK_CurrentSessionInfo();
				}
				bIsSessionPresent = true;
				FEIK_CurrentSessionInfo SessionInfo(*SessionPtrRef->GetNamedSession(SessionName));
				return SessionInfo;
			}
			return FEIK_CurrentSessionInfo();
		}
		return FEIK_CurrentSessionInfo();
	}
	return FEIK_CurrentSessionInfo();
}

TArray<FName> UEIK_BlueprintFunctions::GetAllCurrentSessionNames(UObject* Context)
{
	if(Context)
	{
		if(!Context->GetWorld())
		{
			return TArray<FName>();
		}
		if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
		{
			if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
			{
				TArray<FName> SessionNames;
				for(auto SessionV : EOSRef->SessionInterfacePtr->Sessions)
				{
					SessionNames.Add(SessionV.SessionName);
				}
				return SessionNames;
			}
		}
	}
	return TArray<FName>();
}

FString UEIK_BlueprintFunctions::GetProductUserID(UObject* Context)
{
	if(Context)
	{
		if(!Context->GetWorld())
		{
			return FString();
		}
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
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

IVoiceChatUser* UEIK_BlueprintFunctions::GetLobbyVoiceChat(UObject* Context)
{
	if (Context)
	{
		UWorld* World = Context->GetWorld();
		if (!World)
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetLobbyVoiceChat: World is null"));
			return nullptr;
		}

		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK");
		if (!OnlineSub)
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetLobbyVoiceChat: OnlineSubsystem is null"));
			return nullptr;
		}

		FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub);
		if (!EOSRef)
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetLobbyVoiceChat: EOSRef is null"));
			return nullptr;
		}

		IOnlineIdentityPtr IdentityPointerRef = OnlineSub->GetIdentityInterface();
		if (!IdentityPointerRef.IsValid())
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetLobbyVoiceChat: IdentityPointerRef is null"));
			return nullptr;
		}

		TSharedPtr<const FUniqueNetId> UniquePlayerId = IdentityPointerRef->GetUniquePlayerId(0);
		if (!UniquePlayerId.IsValid())
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetLobbyVoiceChat: UniquePlayerId is null"));
			return nullptr;
		}
		return EOSRef->GetVoiceChatUserInterface(*UniquePlayerId);
	}
	return nullptr;
}

bool UEIK_BlueprintFunctions::MuteLobbyVoiceChat(UObject* Context, bool bMute)
{
	if(GetLobbyVoiceChat(Context))
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK");
		if (!OnlineSub)
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::MuteLobbyVoiceChat: OnlineSubsystem is null"));
			return false;
		}
		IOnlineIdentityPtr IdentityPointerRef = OnlineSub->GetIdentityInterface();
		if (!IdentityPointerRef.IsValid())
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::MuteLobbyVoiceChat: IdentityPointerRef is null"));
			return false;
		}
		GetLobbyVoiceChat(Context)->SetPlayerMuted(*IdentityPointerRef->GetUniquePlayerId(0)->ToString(), bMute);
		return true;
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::MuteLobbyVoiceChat: VoiceChatUserInterface is null"));
	return false;
}


bool UEIK_BlueprintFunctions::IsLobbyVoiceChatMuted(UObject* Context)
{
	if(GetLobbyVoiceChat(Context))
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK");
		if (!OnlineSub)
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::IsLobbyVoiceChatMuted: OnlineSubsystem is null"));
			return false;
		}
		IOnlineIdentityPtr IdentityPointerRef = OnlineSub->GetIdentityInterface();
		if (!IdentityPointerRef.IsValid())
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::IsLobbyVoiceChatMuted: IdentityPointerRef is null"));
			return false;
		}
		UE_LOG(LogEIK, Verbose, TEXT("UEIK_BlueprintFunctions::IsLobbyVoiceChatMuted: Checking if player is muted"));
		return GetLobbyVoiceChat(Context)->IsPlayerMuted(*IdentityPointerRef->GetUniquePlayerId(0)->ToString());
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::IsLobbyVoiceChatMuted: Context is null"));
	return false;
}

bool UEIK_BlueprintFunctions::SetLobbyOutputMethod(UObject* Context, FString MethodID)
{
	if(GetLobbyVoiceChat(Context))
	{
		GetLobbyVoiceChat(Context)->SetOutputDeviceId(MethodID);
		return true;
	}
	return false;
}

bool UEIK_BlueprintFunctions::SetLobbyInputMethod(UObject* Context, FString MethodID)
{
	if(GetLobbyVoiceChat(Context))
	{
		GetLobbyVoiceChat(Context)->SetInputDeviceId(MethodID);
		return true;
	}
	return false;
}

bool UEIK_BlueprintFunctions::BlockLobbyVoiceChatPlayers(UObject* Context, TArray<FString> BlockedPlayers)
{
	if(GetLobbyVoiceChat(Context))
	{
		GetLobbyVoiceChat(Context)->BlockPlayers(BlockedPlayers);
		return true;
	}
	return false;
}

bool UEIK_BlueprintFunctions::UnblockLobbyVoiceChatPlayers(UObject* Context, TArray<FString> UnblockedPlayers)
{
	if(GetLobbyVoiceChat(Context))
	{
		GetLobbyVoiceChat(Context)->UnblockPlayers(UnblockedPlayers);
		return true;
	}
	return false;
}

float UEIK_BlueprintFunctions::GetLobbyVoiceChatOutputVolume(UObject* Context)
{
	if(GetLobbyVoiceChat(Context))
	{
		return GetLobbyVoiceChat(Context)->GetAudioOutputVolume();
	}
	return -1.0f;
}

bool UEIK_BlueprintFunctions::SetLobbyVoiceChatOutputVolume(UObject* Context, float Volume)
{
	if(GetLobbyVoiceChat(Context))
	{
		GetLobbyVoiceChat(Context)->SetAudioOutputVolume(Volume);
		return true;
	}
	return false;
}

bool UEIK_BlueprintFunctions::SetLobbyVoiceChatInputVolume(UObject* Context, float Volume)
{
	if(GetLobbyVoiceChat(Context))
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK");
		if (!OnlineSub)
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::IsLobbyVoiceChatMuted: OnlineSubsystem is null"));
			return false;
		}
		IOnlineIdentityPtr IdentityPointerRef = OnlineSub->GetIdentityInterface();
		if (!IdentityPointerRef.IsValid())
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::IsLobbyVoiceChatMuted: IdentityPointerRef is null"));
			return false;
		}
		GetLobbyVoiceChat(Context)->SetAudioInputVolume(Volume);
		return true;
	}
	return false;
}

float UEIK_BlueprintFunctions::GetLobbyVoiceChatInputVolume(UObject* Context)
{
	if(GetLobbyVoiceChat(Context))
	{
		return GetLobbyVoiceChat(Context)->GetAudioInputVolume();
	}
	return -1.0f;
}

bool UEIK_BlueprintFunctions::SetLobbyPlayerVoiceChatVolume(UObject* Context, FString PlayerName, float Volume)
{
	if(GetLobbyVoiceChat(Context))
	{
		GetLobbyVoiceChat(Context)->SetPlayerVolume(PlayerName, Volume);
		UE_LOG(LogEIK, Verbose, TEXT("UEIK_BlueprintFunctions::SetLobbyPlayerVoiceChatVolume: Player volume set"));
		return true;
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::SetLobbyPlayerVoiceChatVolume: VoiceChatUserInterface is null"));
	return false;
}

float UEIK_BlueprintFunctions::GetLobbyPlayerVoiceChatVolume(UObject* Context, FString PlayerName)
{
	if(GetLobbyVoiceChat(Context))
	{
		return GetLobbyVoiceChat(Context)->GetPlayerVolume(PlayerName);
	}
	return -1.0f;
}

bool UEIK_BlueprintFunctions::ShowFriendsList()
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get("EIK"); // Get the Online Subsystem
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

FEIKUniqueNetId UEIK_BlueprintFunctions::MakeEIKUniqueNetId(FString EpicAccountId, FString ProductUserId)
{
	if(ProductUserId.IsEmpty())
	{
		return FEIKUniqueNetId();
	}
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface())
		{
			if(EpicAccountId.IsEmpty() && !ProductUserId.IsEmpty())
			{
				FEIKUniqueNetId UserId;
				UserId.SetUniqueNetId(IdentityInterface->CreateUniquePlayerId(ProductUserId));
				return UserId;
			}
			FEIKUniqueNetId UserId;
			UserId.SetUniqueNetId(IdentityInterface->CreateUniquePlayerId(EpicAccountId + TEXT("|") + ProductUserId));
			return UserId;
		}
	}
	return FEIKUniqueNetId();
}

bool UEIK_BlueprintFunctions::AcceptSessionInvite(FString InviteId, FString LocalUserId, FString InviterUserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_ProductUserId LocalProductUserId = EOS_ProductUserId_FromString(TCHAR_TO_ANSI(*LocalUserId));
			EOS_ProductUserId InviterProductUserId = EOS_ProductUserId_FromString(TCHAR_TO_ANSI(*InviterUserId));
			EOSRef->SessionInterfacePtr->OnLobbyInviteAccepted(TCHAR_TO_ANSI(*InviteId), LocalProductUserId, InviterProductUserId);
			return true;
		}
	}
	return false;
}

bool UEIK_BlueprintFunctions::RejectSessionInvite(FString InviteId, FString LocalUserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_ProductUserId LocalProductUserId = EOS_ProductUserId_FromString(TCHAR_TO_ANSI(*LocalUserId));
			EOS_Lobby_RejectInviteOptions RejectOptions;
			RejectOptions.ApiVersion = EOS_LOBBY_REJECTINVITE_API_LATEST;
			RejectOptions.LocalUserId = LocalProductUserId;
			RejectOptions.InviteId = TCHAR_TO_ANSI(*InviteId);
			EOS_Lobby_RejectInvite(EOSRef->SessionInterfacePtr->LobbyHandle, &RejectOptions, nullptr, nullptr);
			return true;
		}
	}
	return false;
}

bool UEIK_BlueprintFunctions::StartSession(FName SessionName)
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			return SessionPtrRef->StartSession(SessionName);
		}
	}
	return false;
}

bool UEIK_BlueprintFunctions::RegisterPlayer(FName SessionName,FEIKUniqueNetId PlayerId, bool bWasInvited)
{
	if(!PlayerId.IsValid())
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::RegisterPlayer: PlayerId is not valid"));
		return false;
	}
	if(!PlayerId.UniqueNetId.IsValid())
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::RegisterPlayer: PlayerId.UniqueNetId is not valid"));
		return false;
	}
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
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
	if(!PlayerId.IsValid())
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::UnRegisterPlayer: PlayerId is not valid"));
		return false;
	}
	if(!PlayerId.UniqueNetId.IsValid())
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::UnRegisterPlayer: PlayerId.UniqueNetId is not valid"));
		return false;
	}
	
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
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
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
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

bool UEIK_BlueprintFunctions::IsInSession(FName SessionName,FEIKUniqueNetId PlayerId)
{
	if(!PlayerId.IsValid())
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::IsInSession: PlayerId is not valid"));
		return false;
	}
	if(!PlayerId.UniqueNetId.IsValid())
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::IsInSession: PlayerId.UniqueNetId is not valid"));
		return false;
	}
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			return SessionPtrRef->IsPlayerInSession(SessionName, PlayerId.UniqueNetId.ToSharedRef().Get());
		}
	}
	return false;
}

FString UEIK_BlueprintFunctions::GetPlayerNickname(const int32 LocalUserNum)
{
	FString Nickname;
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
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
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
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
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get("EIK"))
	{
		return true;
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


FString UEIK_BlueprintFunctions::ByteArrayToString(const TArray<uint8>& DataToConvert)
{
	FString ResultString;

	// Iterate through each element of the array
	for (uint8 Element : DataToConvert)
	{
		// Append the character represented by the uint8 value to the result string
		ResultString.AppendChar((TCHAR)Element);
	}
	return ResultString;
}

TArray<uint8> UEIK_BlueprintFunctions::StringToByteArray(const FString& DataToConvert)
{

	TArray<uint8> ResultArray;

	// Iterate through each character of the string
	for (int32 Index = 0; Index < DataToConvert.Len(); Index++)
	{
		// Get the ASCII value of the character and add it to the array
		uint8 CharAsUint8 = (uint8)DataToConvert[Index];
		ResultArray.Add(CharAsUint8);
	}
	return ResultArray;
}

FEIKUniqueNetId UEIK_BlueprintFunctions::GetUserUniqueID(const APlayerController* PlayerController, bool& bIsValid)
{
#if ENGINE_MAJOR_VERSION == 5
	if(const APlayerState* PlayerState = PlayerController->PlayerState; !PlayerState)
#else
	if(!PlayerController)
	{
		bIsValid = false;
		return FEIKUniqueNetId();
	}
	if(!PlayerController->PlayerState)
	{
		bIsValid = false;
		return FEIKUniqueNetId();
	}	
	const APlayerState* PlayerState = PlayerController->PlayerState;
	if(!PlayerState)
#endif
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

FEIKUniqueNetId UEIK_BlueprintFunctions::GetUserUniqueIDFromPlayerState(const APlayerState* PlayerState, bool& bIsValid)
{
	if(!PlayerState)
	{
		bIsValid = false;
		return FEIKUniqueNetId();
	}
	if (!PlayerState->GetUniqueId().IsValid())
	{
		bIsValid = false;
		return FEIKUniqueNetId();
	}
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
	bIsValid = false;
	return FEIKUniqueNetId();
}

FString UEIK_BlueprintFunctions::GetEOSSDKVersion()
{
	return EOS_GetVersion();
}

FString UEIK_BlueprintFunctions::GetEIKPluginVersion()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("EOSIntegrationKit"));
	if(Plugin.IsValid())
	{
		return Plugin->GetDescriptor().VersionName;
	}
	return FString(TEXT("Plugin not found"));
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

FDateTime UEIK_BlueprintFunctions::ConvertPosixTimeToDateTime(int64 PosixTime)
{
	return FDateTime::FromUnixTimestamp(PosixTime);
}

FString UEIK_BlueprintFunctions::GetResolvedConnectString(FName SessionName)
{
	if (const IOnlineSubsystem* SubsystemRef = IOnlineSubsystem::Get("EIK"))
	{
		if(const IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface())
		{
			FString ConnectString;
			SessionPtrRef->GetResolvedConnectString(SessionName, ConnectString);
			if(ConnectString.Contains(":GameSession:97"))
			{
				ConnectString = ConnectString.Replace(TEXT(":GameSession:97"), TEXT(":GameNetDriver:26"));
			}
			return ConnectString;
		}
	}
	return FString();
}

bool UEIK_BlueprintFunctions::GetAutoLoginAttemptedStatus()
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->UserManager)
			{
				return EOSRef->UserManager->bAutoLoginAttempted;
			}
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetAutoLoginAttemptedStatus: OnlineSubsystem is null"));
	return false;
}

bool UEIK_BlueprintFunctions::GetAutoLoginInProgressStatus()
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			if(EOSRef->UserManager)
			{
				return EOSRef->UserManager->bAutoLoginInProgress;
			}
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::GetAutoLoginInProgressStatus: OnlineSubsystem is null"));
	return false;
}

FString UEIK_BlueprintFunctions::GetEnvironmentVariable(const FString& EnvVariableName)
{
	if(EnvVariableName.IsEmpty())
	{
		return FString();
	}
	return FPlatformMisc::GetEnvironmentVariable(*EnvVariableName);
}

bool UEIK_BlueprintFunctions::InitPingBeacon(UObject* Context, AGameModeBase* GameMode)
{
	if(Context)
	{
		if(Context->GetWorld() && GameMode)
		{
			if(GameMode->GetNetMode() == NM_DedicatedServer || GameMode ->GetNetMode() == NM_ListenServer)
			{
				APingHost* Host = Context->GetWorld()->SpawnActor<APingHost>(APingHost::StaticClass());
				if(Host && Host->InitializeHost())
				{
					APingHostObject* HostObject = Context->GetWorld()->SpawnActor<APingHostObject>(APingHostObject::StaticClass());
					if(HostObject)
					{
						Host->RegisterHostObject(HostObject);
						return true;
					}
					else
					{
						UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::InitPingBeacon: HostObject is null or failed to initialize"));
					}
				}
				else
				{
					UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::InitPingBeacon: Host is null or failed to initialize"));
				}
			}
			else
			{
				UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::InitPingBeacon: GameMode is not a server"));
			}
		}
		else
		{
			UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::InitPingBeacon: Context or GameMode is null"));
		}
	}
	else
	{
		UE_LOG(LogEIK, Error, TEXT("UEIK_BlueprintFunctions::InitPingBeacon: Context is null"));
	}
	return false;
}
