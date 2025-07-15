// Copyright Epic Games, Inc. All Rights Reserved.

#include "UserManagerEOS.h"
#include "CoreMinimal.h"
#include "CoreMinimal.h"
#include "EIKSettings.h"
#include "eos_connect_types.h"
#include "IEOSSDKManager.h"
#include "IPAddress.h"
#include "OnlineError.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEOSPrivate.h"
#include "SocketSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"
#include "Misc/Guid.h"
#include "Misc/OutputDeviceRedirector.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"


#include "Interfaces/IPluginManager.h"

#include COMPILED_PLATFORM_HEADER(EOSHelpers.h)

#if WITH_EOS_SDK

#include "eos_auth.h"
#include "eos_friends.h"
#include "eos_presence.h"
#include "eos_ui.h"
#include "eos_userinfo.h"

static inline EInviteStatus::Type ToEInviteStatus(EOS_EFriendsStatus InStatus)
{
	switch (InStatus)
	{
		case EOS_EFriendsStatus::EOS_FS_InviteSent:
		{
			return EInviteStatus::PendingOutbound;
		}
		case EOS_EFriendsStatus::EOS_FS_InviteReceived:
		{
			return EInviteStatus::PendingInbound;
		}
		case EOS_EFriendsStatus::EOS_FS_Friends:
		{
			return EInviteStatus::Accepted;
		}
	}
	return EInviteStatus::Unknown;
}

static inline EOnlinePresenceState::Type ToEOnlinePresenceState(EOS_Presence_EStatus InStatus)
{
	switch (InStatus)
	{
		case EOS_Presence_EStatus::EOS_PS_Online:
		{
			return EOnlinePresenceState::Online;
		}
		case EOS_Presence_EStatus::EOS_PS_Away:
		{
			return EOnlinePresenceState::Away;
		}
		case EOS_Presence_EStatus::EOS_PS_ExtendedAway:
		{
			return EOnlinePresenceState::ExtendedAway;
		}
		case EOS_Presence_EStatus::EOS_PS_DoNotDisturb:
		{
			return EOnlinePresenceState::DoNotDisturb;
		}
	}
	return EOnlinePresenceState::Offline;
}

static inline EOS_Presence_EStatus ToEOS_Presence_EStatus(EOnlinePresenceState::Type InStatus)
{
	switch (InStatus)
	{
		case EOnlinePresenceState::Online:
		{
			return EOS_Presence_EStatus::EOS_PS_Online;
		}
		case EOnlinePresenceState::Away:
		{
			return EOS_Presence_EStatus::EOS_PS_Away;
		}
		case EOnlinePresenceState::ExtendedAway:
		{
			return EOS_Presence_EStatus::EOS_PS_ExtendedAway;
		}
		case EOnlinePresenceState::DoNotDisturb:
		{
			return EOS_Presence_EStatus::EOS_PS_DoNotDisturb;
		}
	}
	return EOS_Presence_EStatus::EOS_PS_Offline;
}

static inline EOS_EExternalCredentialType ToEOS_EExternalCredentialType(FName OSSName, const FOnlineAccountCredentials& AccountCredentials)
{
#if PLATFORM_DESKTOP
	if (OSSName == STEAM_SUBSYSTEM)
	{
		return EOS_EExternalCredentialType::EOS_ECT_STEAM_SESSION_TICKET;
	}
#endif
	if (OSSName == PS4_SUBSYSTEM || USE_PSN_ID_TOKEN)
	{
		return EOS_EExternalCredentialType::EOS_ECT_PSN_ID_TOKEN;
	}
	else if (USE_XBL_XSTS_TOKEN)
	{
		return EOS_EExternalCredentialType::EOS_ECT_XBL_XSTS_TOKEN;
	}
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	else if (OSSName == NINTENDO_SUBSYSTEM)
#else
	else if (OSSName == SWITCH_SUBSYSTEM)
#endif
	{
		if (AccountCredentials.Type == TEXT("NintendoAccount"))
		{
			return EOS_EExternalCredentialType::EOS_ECT_NINTENDO_ID_TOKEN;
		}
		else
		{
			return EOS_EExternalCredentialType::EOS_ECT_NINTENDO_NSA_ID_TOKEN;
		}
	}
	else if (OSSName == APPLE_SUBSYSTEM)
	{
		return EOS_EExternalCredentialType::EOS_ECT_APPLE_ID_TOKEN;
	}
	// Unknown means OpenID
	return EOS_EExternalCredentialType::EOS_ECT_OPENID_ACCESS_TOKEN;
}

/** Delegates that are used for internal calls and are meant to be ignored */
IOnlinePresence::FOnPresenceTaskCompleteDelegate IgnoredPresenceDelegate;
IOnlineUser::FOnQueryExternalIdMappingsComplete IgnoredMappingDelegate;

#if ENGINE_MAJOR_VERSION == 5
typedef TEIKGlobalCallback<EOS_UI_OnDisplaySettingsUpdatedCallback, EOS_UI_OnDisplaySettingsUpdatedCallbackInfo, FUserManagerEOS> FOnDisplaySettingsUpdatedCallback;
typedef TEIKGlobalCallback<EOS_Connect_OnLoginStatusChangedCallback, EOS_Connect_LoginStatusChangedCallbackInfo, FUserManagerEOS> FConnectLoginStatusChangedCallback;
typedef TEIKGlobalCallback<EOS_Connect_OnAuthExpirationCallback, EOS_Connect_AuthExpirationCallbackInfo, FUserManagerEOS> FRefreshAuthCallback;
#else
typedef TEIKGlobalCallback<EOS_UI_OnDisplaySettingsUpdatedCallback, EOS_UI_OnDisplaySettingsUpdatedCallbackInfo> FOnDisplaySettingsUpdatedCallback;
typedef TEIKGlobalCallback<EOS_Connect_OnLoginStatusChangedCallback, EOS_Connect_LoginStatusChangedCallbackInfo> FConnectLoginStatusChangedCallback;
typedef TEIKGlobalCallback<EOS_Connect_OnAuthExpirationCallback, EOS_Connect_AuthExpirationCallbackInfo> FRefreshAuthCallback;
#endif

FUserManagerEOS::FUserManagerEOS(FOnlineSubsystemEOS* InSubsystem)
	: TSharedFromThis<FUserManagerEOS, ESPMode::ThreadSafe>()
	, EOSSubsystem(InSubsystem)
	, DefaultLocalUser(-1)
	, LoginNotificationId(0)
	, LoginNotificationCallback(nullptr)
	, FriendsNotificationId(0)
	, FriendsNotificationCallback(nullptr)
	, PresenceNotificationId(0)
	, PresenceNotificationCallback(nullptr)
	, DisplaySettingsUpdatedId(0)
	, DisplaySettingsUpdatedCallback(nullptr)
{
}

void FUserManagerEOS::Init()
{
	// This delegate would cause a crash when running a dedicated server
	if (!IsRunningDedicatedServer())
	{
		// Adding subscription to external ui display change event
		EOS_UI_AddNotifyDisplaySettingsUpdatedOptions Options = {};
		Options.ApiVersion = EOS_UI_ADDNOTIFYDISPLAYSETTINGSUPDATED_API_LATEST;
#if ENGINE_MAJOR_VERSION == 5
		FOnDisplaySettingsUpdatedCallback* CallbackObj = new FOnDisplaySettingsUpdatedCallback(AsWeak());
#else
		FOnDisplaySettingsUpdatedCallback* CallbackObj = new FOnDisplaySettingsUpdatedCallback();
#endif
		DisplaySettingsUpdatedCallback = CallbackObj;
		CallbackObj->CallbackLambda = [this](const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo* Data)
		{
			TriggerOnExternalUIChangeDelegates((bool)Data->bIsVisible);
		};
		DisplaySettingsUpdatedId = EOS_UI_AddNotifyDisplaySettingsUpdated(EOSSubsystem->UIHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
}

void FUserManagerEOS::Shutdown()
{
	// This delegate would cause a crash when running a dedicated server
	if (DisplaySettingsUpdatedId != EOS_INVALID_NOTIFICATIONID)
	{
		// Removing subscription to external ui display change event
		EOS_UI_RemoveNotifyDisplaySettingsUpdated(EOSSubsystem->UIHandle, DisplaySettingsUpdatedId);

		if (DisplaySettingsUpdatedCallback)
		{
			delete DisplaySettingsUpdatedCallback;
		}
	}
}



FUserManagerEOS::~FUserManagerEOS()
{
	Shutdown();
}

void FUserManagerEOS::LoginStatusChanged(const EOS_Auth_LoginStatusChangedCallbackInfo* Data)
{
	if (Data->CurrentStatus == EOS_ELoginStatus::EOS_LS_NotLoggedIn)
	{
		if (AccountIdToUserNumMap.Contains(Data->LocalUserId))
		{
			int32 LocalUserNum = AccountIdToUserNumMap[Data->LocalUserId];
			FUniqueNetIdEOSPtr UserNetId = UserNumToNetIdMap[LocalUserNum];
			TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::LoggedIn, ELoginStatus::NotLoggedIn, *UserNetId);
			// Need to remove the local user
			RemoveLocalUser(LocalUserNum);

			// Clean up user based notifies if we have no logged in users
			if (UserNumToNetIdMap.Num() == 0)
			{
				if (LoginNotificationId > 0)
				{
					// Remove the callback
					EOS_Auth_RemoveNotifyLoginStatusChanged(EOSSubsystem->AuthHandle, LoginNotificationId);
					delete LoginNotificationCallback;
					LoginNotificationCallback = nullptr;
					LoginNotificationId = 0;
				}
				if (FriendsNotificationId > 0)
				{
					EOS_Friends_RemoveNotifyFriendsUpdate(EOSSubsystem->FriendsHandle, FriendsNotificationId);
					delete FriendsNotificationCallback;
					FriendsNotificationCallback = nullptr;
					FriendsNotificationId = 0;
				}
				if (PresenceNotificationId > 0)
				{
					EOS_Presence_RemoveNotifyOnPresenceChanged(EOSSubsystem->PresenceHandle, PresenceNotificationId);
					delete PresenceNotificationCallback;
					PresenceNotificationCallback = nullptr;
					PresenceNotificationId = 0;
				}
				// Remove the per user connect login notification
				if (LocalUserNumToConnectLoginNotifcationMap.Contains(LocalUserNum))
				{
					FNotificationIdCallbackPair* NotificationPair = LocalUserNumToConnectLoginNotifcationMap[LocalUserNum];
					LocalUserNumToConnectLoginNotifcationMap.Remove(LocalUserNum);

					EOS_Connect_RemoveNotifyAuthExpiration(EOSSubsystem->ConnectHandle, NotificationPair->NotificationId);

					delete NotificationPair;
				}
			}
		}
	}
}

IOnlineSubsystem* FUserManagerEOS::GetPlatformOSS() const
{
	IOnlineSubsystem* PlatformOSS = IOnlineSubsystem::GetByPlatform();
	if (PlatformOSS == nullptr)
#if !PLATFORM_DESKTOP
	{
		UE_LOG_ONLINE(Error, TEXT("GetPlatformOSS() failed due to no platform OSS being configured"));
	}
#else
	{
		// Attempt to load Steam before treating it as an error
		PlatformOSS = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	}
#endif
	return PlatformOSS;
}

void FUserManagerEOS::GetPlatformAuthToken(int32 LocalUserNum, const FOnGetLinkedAccountAuthTokenCompleteDelegate& Delegate) const
{
	IOnlineSubsystem* PlatformOSS = GetPlatformOSS();
	if (PlatformOSS == nullptr)
	{
		UE_LOG_ONLINE(Error, TEXT("ConnectLoginNoEAS(%d) failed due to no platform OSS"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FExternalAuthToken());
		return;
	}
	IOnlineIdentityPtr PlatformIdentity = PlatformOSS->GetIdentityInterface();
	if (!PlatformIdentity.IsValid())
	{
		UE_LOG_ONLINE(Error, TEXT("ConnectLoginNoEAS(%d) failed due to no platform OSS identity interface"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FExternalAuthToken());
		return;
	}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 2
	FString TokenType;
	// TODO config map of OSS -> token type?
	if (PlatformOSS->GetSubsystemName() == STEAM_SUBSYSTEM)
	{
		TokenType = TEXT("Session");
	}

	// Request the auth token from the platform
	PlatformIdentity->GetLinkedAccountAuthToken(LocalUserNum, TokenType, Delegate);
#else
	PlatformIdentity->GetLinkedAccountAuthToken(LocalUserNum, Delegate);
#endif
	// Request the auth token from the platform
}

FString FUserManagerEOS::GetPlatformDisplayName(int32 LocalUserNum) const
{
	FString Result;

	IOnlineSubsystem* PlatformOSS = GetPlatformOSS();
	if (PlatformOSS == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("GetPlatformDisplayName(%d) failed due to no platform OSS"), LocalUserNum);
		return Result;
	}
	IOnlineIdentityPtr PlatformIdentity = PlatformOSS->GetIdentityInterface();
	if (!PlatformIdentity.IsValid())
	{
		UE_LOG_ONLINE(Warning, TEXT("GetPlatformDisplayName(%d) failed due to no platform OSS identity interface"), LocalUserNum);
		return Result;
	}

	Result = PlatformIdentity->GetPlayerNickname(LocalUserNum);

	return Result;
}
#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Auth_OnLoginCallback, EOS_Auth_LoginCallbackInfo, FUserManagerEOS> FLoginCallback;
typedef TEOSCallback<EOS_Connect_OnLoginCallback, EOS_Connect_LoginCallbackInfo, FUserManagerEOS> FConnectLoginCallback;
typedef TEOSCallback<EOS_Connect_OnCreateDeviceIdCallback, EOS_Connect_CreateDeviceIdCallbackInfo, FUserManagerEOS> FCreateDeviceIDCallback;
typedef TEOSCallback<EOS_Connect_OnCreateUserCallback, EOS_Connect_CreateUserCallbackInfo, FUserManagerEOS> FCreateUserCallback;
typedef TEOSCallback<EOS_Connect_OnDeleteDeviceIdCallback, EOS_Connect_DeleteDeviceIdCallbackInfo, FUserManagerEOS> FConnectDeleteDeviceIdCallback;
typedef TEOSCallback<EOS_Auth_OnDeletePersistentAuthCallback, EOS_Auth_DeletePersistentAuthCallbackInfo, FUserManagerEOS> FDeletePersistentAuthCallback;

#else
typedef TEOSCallback<EOS_Auth_OnLoginCallback, EOS_Auth_LoginCallbackInfo> FLoginCallback;
typedef TEOSCallback<EOS_Connect_OnLoginCallback, EOS_Connect_LoginCallbackInfo> FConnectLoginCallback;
typedef TEOSCallback<EOS_Connect_OnCreateDeviceIdCallback, EOS_Connect_CreateDeviceIdCallbackInfo> FCreateDeviceIDCallback;
typedef TEOSCallback<EOS_Connect_OnCreateUserCallback, EOS_Connect_CreateUserCallbackInfo> FCreateUserCallback;
typedef TEOSCallback<EOS_Connect_OnDeleteDeviceIdCallback, EOS_Connect_DeleteDeviceIdCallbackInfo> FConnectDeleteDeviceIdCallback;
typedef TEOSCallback<EOS_Auth_OnDeletePersistentAuthCallback, EOS_Auth_DeletePersistentAuthCallbackInfo> FDeletePersistentAuthCallback;
#endif

// Chose arbitrarily since the SDK doesn't define it
#define EOS_MAX_TOKEN_SIZE 4096

struct FAuthCredentials :
	public EOS_Auth_Credentials
{
	FAuthCredentials() :
		EOS_Auth_Credentials()
	{
		ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
		Id = IdAnsi;
		Token = TokenAnsi;

		FMemory::Memset(IdAnsi, 0, sizeof(IdAnsi));
		FMemory::Memset(TokenAnsi, 0, sizeof(TokenAnsi));
	}

	FAuthCredentials(const FAuthCredentials& Other)
	{
		ApiVersion = Other.ApiVersion;
		Id = IdAnsi;
		Token = TokenAnsi;
		Type = Other.Type;
		SystemAuthCredentialsOptions = Other.SystemAuthCredentialsOptions;
		ExternalType = Other.ExternalType;

		FCStringAnsi::Strncpy(IdAnsi, Other.IdAnsi, EOS_OSS_STRING_BUFFER_LENGTH);
		FCStringAnsi::Strncpy(TokenAnsi, Other.TokenAnsi, EOS_MAX_TOKEN_SIZE);
	}

	FAuthCredentials& operator=(FAuthCredentials& Other)
	{
		ApiVersion = Other.ApiVersion;
		Type = Other.Type;
		SystemAuthCredentialsOptions = Other.SystemAuthCredentialsOptions;
		ExternalType = Other.ExternalType;

		FCStringAnsi::Strncpy(IdAnsi, Other.IdAnsi, EOS_OSS_STRING_BUFFER_LENGTH);
		FCStringAnsi::Strncpy(TokenAnsi, Other.TokenAnsi, EOS_MAX_TOKEN_SIZE);

		return *this;
	}

	FAuthCredentials(EOS_EExternalCredentialType InExternalType, const FExternalAuthToken& AuthToken) :
		EOS_Auth_Credentials()
	{
		if (AuthToken.HasTokenData())
		{
			Init(InExternalType, AuthToken.TokenData);
		}
		else if (AuthToken.HasTokenString())
		{
			Init(InExternalType, AuthToken.TokenString);
		}
		else
		{
			UE_LOG_ONLINE(Error, TEXT("FAuthCredentials object cannot be constructed with invalid FExternalAuthToken parameter"));
		}
	}
	

	void Init(EOS_EExternalCredentialType InExternalType, const FString& InTokenString)
	{
		ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
		Type = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
		ExternalType = InExternalType;
		Id = IdAnsi;
		Token = TokenAnsi;

		FCStringAnsi::Strncpy(TokenAnsi, TCHAR_TO_UTF8(*InTokenString), InTokenString.Len()+1);
	}

	void Init(EOS_EExternalCredentialType InExternalType, const TArray<uint8>& InToken)
	{
		ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
		Type = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
		ExternalType = InExternalType;
		Id = IdAnsi;
		Token = TokenAnsi;

		uint32_t InOutBufferLength = EOS_MAX_TOKEN_SIZE;
		EOS_ByteArray_ToString(InToken.GetData(), InToken.Num(), TokenAnsi, &InOutBufferLength);
	}

	char IdAnsi[EOS_OSS_STRING_BUFFER_LENGTH];
	char TokenAnsi[EOS_MAX_TOKEN_SIZE];
};

void FUserManagerEOS::CreateDeviceID(const FOnlineAccountCredentials& AccountCredentials)
{
	EOS_Connect_CreateDeviceIdOptions DeviceIdOptions = {};
	DeviceIdOptions.ApiVersion = EOS_CONNECT_CREATEDEVICEID_API_LATEST;
	FString DisplayName = AccountCredentials.Id;
	if (DisplayName.IsEmpty() || DisplayName.Equals(""))
	{
		DisplayName = "DefaultName";
	}
	FString DisplayNameStr = DisplayName;
	DeviceIdOptions.DeviceModel = "DefaultModel";
	
	int32 LocalUserNum = 0;
#if ENGINE_MAJOR_VERSION == 5
	FCreateDeviceIDCallback* CallbackObj = new FCreateDeviceIDCallback(AsWeak());
#else
	FCreateDeviceIDCallback* CallbackObj = new FCreateDeviceIDCallback();
#endif
	CallbackObj->CallbackLambda = [LocalUserNum, AccountCredentials, this](const EOS_Connect_CreateDeviceIdCallbackInfo* Data)
	{
		if(Data->ResultCode == EOS_EResult::EOS_Success || Data->ResultCode == EOS_EResult::EOS_DuplicateNotAllowed )
		{
			LoginViaConnectInterface(AccountCredentials);
		}
		else
		{
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), FString());
			UE_LOG(LogOnline, Warning, TEXT("EOS Create Device ID Failed due to %hs"), EOS_EResult_ToString(Data->ResultCode))
		}
	};
	EOS_Connect_CreateDeviceId(EOSSubsystem->ConnectHandle, &DeviceIdOptions,(void*)CallbackObj, CallbackObj->GetCallbackPtr() );
}

void FUserManagerEOS::CreateConnectID(EOS_ContinuanceToken ContinuanceToken, const FOnlineAccountCredentials& AccountCredentials)
{
	EOS_Connect_CreateUserOptions CreateUserOptions = {};
	CreateUserOptions.ApiVersion = EOS_CONNECT_CREATEUSER_API_LATEST;
	CreateUserOptions.ContinuanceToken = ContinuanceToken;
#if ENGINE_MAJOR_VERSION == 5
	FCreateUserCallback* CallbackObj = new FCreateUserCallback(AsWeak());
#else
	FCreateUserCallback* CallbackObj = new FCreateUserCallback();
#endif
	CallbackObj->CallbackLambda = [this,AccountCredentials](const EOS_Connect_CreateUserCallbackInfo* Data)
	{
		if(Data->ResultCode == EOS_EResult::EOS_Success)
		{
			CompleteDeviceIDLogin(0,nullptr,Data->LocalUserId);
		}
		else
		{
			TriggerOnLoginCompleteDelegates(0, false, *FUniqueNetIdEOS::EmptyId(), FString());
			UE_LOG(LogTemp, Warning, TEXT("EOS Create User Failed due to %hs"), EOS_EResult_ToString(Data->ResultCode));
		}
	};
	EOS_Connect_CreateUser(EOSSubsystem->ConnectHandle, &CreateUserOptions, CallbackObj, CallbackObj->GetCallbackPtr());
}

void FUserManagerEOS::DeleteDeviceID(const FOnlineAccountCredentials& AccountCredentials)
{
	EOS_Connect_DeleteDeviceIdOptions DeleteDeviceIdOptions;
	DeleteDeviceIdOptions.ApiVersion = EOS_CONNECT_DELETEDEVICEID_API_LATEST;

	int32 LocalUserNum = 0;
#if ENGINE_MAJOR_VERSION == 5
	FConnectDeleteDeviceIdCallback* CallbackObj = new FConnectDeleteDeviceIdCallback(AsWeak());
#else
	FConnectDeleteDeviceIdCallback* CallbackObj = new FConnectDeleteDeviceIdCallback();
#endif
	CallbackObj->CallbackLambda = [this, LocalUserNum, AccountCredentials](const EOS_Connect_DeleteDeviceIdCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success || Data->ResultCode == EOS_EResult::EOS_NotFound)
		{
			UE_LOG_ONLINE(Display, TEXT(" Device deleted! Or no existing device has been found! Code: (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		else
		{
			UE_LOG_ONLINE(Error, TEXT("Delete Device ID failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
	};
	EOS_Connect_DeleteDeviceId(EOSSubsystem->ConnectHandle, &DeleteDeviceIdOptions, CallbackObj, CallbackObj->GetCallbackPtr());
}

void FUserManagerEOS::CompleteDeviceIDLogin(int32 LocalUserNum, EOS_EpicAccountId AccountId, EOS_ProductUserId UserId)
{
	if (LoginNotificationId == 0)
	{
#if ENGINE_MAJOR_VERSION == 5
		FConnectLoginStatusChangedCallback* CallbackObj = new FConnectLoginStatusChangedCallback(AsWeak());
#else
		FConnectLoginStatusChangedCallback* CallbackObj = new FConnectLoginStatusChangedCallback();
#endif
		LoginNotificationCallback = CallbackObj;
		CallbackObj->CallbackLambda = [this](const EOS_Connect_LoginStatusChangedCallbackInfo* Data)
		{
			if(Data->CurrentStatus == EOS_ELoginStatus::EOS_LS_NotLoggedIn)
			{
				//LoginWithDeviceID(*LocalUserNumToLastLoginCredentials[0]);
			}	
		};

		EOS_Connect_AddNotifyLoginStatusChangedOptions Options = { };
		Options.ApiVersion = EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST;
		LoginNotificationId = EOS_Connect_AddNotifyLoginStatusChanged(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	}

	if (!LocalUserNumToConnectLoginNotifcationMap.Contains(LocalUserNum))
	{
		FNotificationIdCallbackPair* NotificationPair = new FNotificationIdCallbackPair();
		LocalUserNumToConnectLoginNotifcationMap.Emplace(LocalUserNum, NotificationPair);

#if ENGINE_MAJOR_VERSION == 5
		FRefreshAuthCallback* CallbackObj = new FRefreshAuthCallback(AsWeak());
#else
		FRefreshAuthCallback* CallbackObj = new FRefreshAuthCallback();
#endif
		NotificationPair->Callback = CallbackObj;
		CallbackObj->CallbackLambda = [LocalUserNum, this](const EOS_Connect_AuthExpirationCallbackInfo* Data)
		{
			RefreshConnectLogin(LocalUserNum);	
		};

		EOS_Connect_AddNotifyAuthExpirationOptions Options = { };
		Options.ApiVersion = EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST;
		NotificationPair->NotificationId = EOS_Connect_AddNotifyAuthExpiration(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}

	AddLocalUser(LocalUserNum, AccountId, UserId);
	FUniqueNetIdEOSPtr UserNetId = GetLocalUniqueNetIdEOS(LocalUserNum);
	check(UserNetId.IsValid());


	TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserNetId, FString());
	TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::NotLoggedIn, ELoginStatus::LoggedIn, *UserNetId);
}

void FUserManagerEOS::OpenIDLogin(const FOnlineAccountCredentials& AccountCredentials)
{
#if ENGINE_MAJOR_VERSION == 5
	FConnectLoginCallback* CallbackObj = new FConnectLoginCallback(AsWeak());
#else
	FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
#endif
	EOS_Connect_Credentials UserCredentials;
	UserCredentials.Type = EOS_EExternalCredentialType::EOS_ECT_OPENID_ACCESS_TOKEN;
	UserCredentials.Token = TCHAR_TO_UTF8(*AccountCredentials.Token);
	UserCredentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
	
	EOS_Connect_UserLoginInfo LoginInfo;
	LoginInfo.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
	LoginInfo.DisplayName = TCHAR_TO_UTF8(*AccountCredentials.Id);

	EOS_Connect_LoginOptions LoginOptions;
	LoginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
	LoginOptions.UserLoginInfo = &LoginInfo;
	LoginOptions.Credentials = &UserCredentials;

	int32 LocalUserNum = 0;
	CallbackObj->CallbackLambda = [LocalUserNum, AccountCredentials, UserCredentials, this](const EOS_Connect_LoginCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			//TriggerOnLoginCompleteDelegates(LocalUserNum, true, *NetIdEos.ToSharedRef(), "");
			CompleteDeviceIDLogin(LocalUserNum, nullptr, Data->LocalUserId);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EOS Login using OpenID Failed due to %hs"), EOS_EResult_ToString(Data->ResultCode));
			EOS_EResult ResultCode = Data->ResultCode;
			const char* ResultCodeStr = EOS_EResult_ToString(ResultCode);
			FString ResultCodeString = FString(ResultCodeStr);
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), *ResultCodeString);
		}		
	};
	EOS_Connect_Login(EOSSubsystem->ConnectHandle, &LoginOptions, CallbackObj, CallbackObj->GetCallbackPtr());
}

void FUserManagerEOS::LoginViaConnectInterface(const FOnlineAccountCredentials& AccountCredentials)
{
	TArray<FString> BrokenTypeString;
	EEIK_EExternalCredentialType ExternalCredentialType;
	if(AccountCredentials.Type.ParseIntoArray(BrokenTypeString, TEXT("_+_"), true) > 0)
	{
		ExternalCredentialType = GetExternalCredentialType(BrokenTypeString[1]);
	}
	else
	{
		UE_LOG(LogEIK, Error, TEXT("Failed to parse AccountCredentials.Type into BrokenTypeString"));
		ExternalCredentialType = EEIK_EExternalCredentialType::EIK_ECT_EPIC;
	}
	EOS_Connect_Credentials UserCredentials;
	UserCredentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
	if(AccountCredentials.Token.IsEmpty())
	{
		UserCredentials.Token = nullptr;
	}
	else
	{
		const char* TokenRef = new char[EOS_MAX_TOKEN_SIZE];
		FCStringAnsi::Strncpy(const_cast<char*>(TokenRef), TCHAR_TO_ANSI(*AccountCredentials.Token), EOS_MAX_TOKEN_SIZE);
		UserCredentials.Token = TokenRef;
	}
	UserCredentials.Type = static_cast<EOS_EExternalCredentialType>(ExternalCredentialType);
	EOS_Connect_UserLoginInfo LoginInfo;
	LoginInfo.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
	if(AccountCredentials.Id.IsEmpty())
	{
		LoginInfo.DisplayName = nullptr;
	}
	else
	{
		const char* CharDisplayName = new char[EOS_MAX_TOKEN_SIZE];
		FCStringAnsi::Strncpy(const_cast<char*>(CharDisplayName), TCHAR_TO_ANSI(*AccountCredentials.Id), EOS_MAX_TOKEN_SIZE);
		LoginInfo.DisplayName = CharDisplayName;
	}
	LoginInfo.NsaIdToken = nullptr;
	EOS_Connect_LoginOptions LoginOptions;
	LoginOptions.Credentials = &UserCredentials;
	LoginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
	LoginOptions.UserLoginInfo = &LoginInfo;
	int32 LocalUserNum = 0;
	UE_LOG(LogEIK, Log, TEXT("LoginViaConnectInterface called. Login Method: %s"), *AccountCredentials.Type);
	EOS_Connect_Login(EOSSubsystem->ConnectHandle, &LoginOptions, this, &FUserManagerEOS::LoginViaConnectInterfaceCallback);
}

EEIK_EExternalCredentialType FUserManagerEOS::GetExternalCredentialType(const FString& Type)
{
	UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/OnlineSubsystemEIK.EEIK_EExternalCredentialType"));
	if (!EnumPtr)
	{
		UE_LOG(LogEIK, Error, TEXT("Enum not found in GetExternalCredentialType! Type: %s"), *Type);
		return EEIK_EExternalCredentialType::EIK_ECT_EPIC; // Return a default value or handle error
	}
	int32 EnumValue = EnumPtr->GetValueByName(FName(*Type));
	if (EnumValue == INDEX_NONE)
	{
		UE_LOG(LogEIK, Error, TEXT("Enum value not found in GetExternalCredentialType! Type: %s"), *Type);
		return EEIK_EExternalCredentialType::EIK_ECT_EPIC; // Return a default value or handle error
	}
	return static_cast<EEIK_EExternalCredentialType>(EnumValue);	
}

EEIK_ELoginCredentialType FUserManagerEOS::GetLoginCredentialType(const FString& Type)
{
	UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/OnlineSubsystemEIK.EEIK_ELoginCredentialType"));
	if (!EnumPtr)
	{
		UE_LOG(LogEIK, Error, TEXT("Enum not found in GetLoginCredentialType! Type: %s"), *Type);
		return EEIK_ELoginCredentialType::EIK_LCT_AccountPortal; // Return a default value or handle error
	}

	int32 EnumValue = EnumPtr->GetValueByName(FName(*Type));
	if (EnumValue == INDEX_NONE)
	{
		UE_LOG(LogEIK, Error, TEXT("Enum value not found in GetLoginCredentialType! Type: %s"), *Type);
		return EEIK_ELoginCredentialType::EIK_LCT_AccountPortal; // Return a default value or handle error
	}
	return static_cast<EEIK_ELoginCredentialType>(EnumValue);	
}

bool FUserManagerEOS::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	LocalUserNumToLastLoginCredentials.Emplace(LocalUserNum, MakeShared<FOnlineAccountCredentials>(AccountCredentials));
	FEOSSettings Settings = UEIKSettings::GetSettings();

	TArray<FString> BrokenTypeString;
	if(AccountCredentials.Type.ParseIntoArray(BrokenTypeString, TEXT("_+_"), true) > 0)
	{
		if(BrokenTypeString[0] == "noeas")
		{
			UE_LOG(LogEIK, Log, TEXT("Login using EIK called. Login Method: %s and UseEas: false"), *BrokenTypeString[1]);
			LoginViaConnectInterface(AccountCredentials);
			return true;
		}
		if(BrokenTypeString[0] == "eas")
		{
			if(BrokenTypeString.Num() > 2)
			{
				UE_LOG(LogEIK, Log, TEXT("Login using EIK called. Login Method: %s  UseEas: true and ExternalAuth: %s"), *BrokenTypeString[1], *BrokenTypeString[2]);
			}
			else if(BrokenTypeString.Num() > 1)
			{
				UE_LOG(LogEIK, Log, TEXT("Login using EIK called. Login Method: %s  UseEas: true"), *BrokenTypeString[1]);
			}
			LoginViaAuthInterface(LocalUserNum, AccountCredentials);
			return true;
		}
		UE_LOG(LogEIK, Warning, TEXT("Login using EIK called. But the format of the type is not correct. Type: %s"), *AccountCredentials.Type);
	}
	else
	{
		UE_LOG(LogEIK, Warning, TEXT("Login using EIK called. But the format of the type is not correct. Type: %s"), *AccountCredentials.Type);
	}
	// Are we configured to run at all?
	// ReSharper disable once CppUnreachableCode
	if (!EOSSubsystem->bIsDefaultOSS && !EOSSubsystem->bIsPlatformOSS && !Settings.bUseEAS && !Settings.bUseEOSConnect)
	{
		UE_LOG_ONLINE(Warning, TEXT("Neither EAS nor EOS are configured to be used. Failed to login in user (%d)"), LocalUserNum);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), FString(TEXT("Not configured")));
		return true;
	}
	
	// See if we are configured to just use EOS and not EAS
	if (!EOSSubsystem->bIsDefaultOSS && !EOSSubsystem->bIsPlatformOSS && !Settings.bUseEAS && Settings.bUseEOSConnect)
	{
		// Call the EOS + Platform login path
		return ConnectLoginNoEAS(LocalUserNum);
	}

	// See if we are logging in using platform credentials to link to EAS
	if (!EOSSubsystem->bIsDefaultOSS && !EOSSubsystem->bIsPlatformOSS && Settings.bUseEAS)
	{
		LoginViaExternalAuth(LocalUserNum);
		return true;
	}
	if(AccountCredentials.Type == TEXT("deviceid") || AccountCredentials.Type == TEXT("openid") || AccountCredentials.Type == TEXT("oculus") || AccountCredentials.Type == TEXT("steam") || AccountCredentials.Type == TEXT("google") )
	{
		LoginViaConnectInterface(AccountCredentials);
		return true;
	}
	if(AccountCredentials.Type == TEXT("steam"))
	{
		return ConnectLoginNoEAS(LocalUserNum);
	}
	if(AccountCredentials.Type == TEXT("apple"))
	{
		LoginViaConnectInterface(AccountCredentials);
		return true;
	}
	if(AccountCredentials.Type == TEXT("openid"))
	{
		OpenIDLogin(AccountCredentials);
		return true;
	}
	EOS_Auth_LoginOptions LoginOptions = { };
	LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	if(UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
	{
		/*if(EIKSettings->bUseCountryScope)
		{
			LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence | EOS_EAuthScopeFlags::EOS_AS_Country;
		}
		else
		{
			LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence;

		}*/
	}
	else
	{
		LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence;
	}

	FPlatformEOSHelpersPtr EOSHelpers = EOSSubsystem->GetEOSHelpers();

	FAuthCredentials Credentials;
	LoginOptions.Credentials = &Credentials;
	EOSHelpers->PlatformAuthCredentials(Credentials);

	bool bIsPersistentLogin = false;

	if (AccountCredentials.Type == TEXT("exchangecode"))
	{
		// This is how the Epic launcher will pass credentials to you
		FCStringAnsi::Strncpy(Credentials.TokenAnsi, TCHAR_TO_UTF8(*AccountCredentials.Token), EOS_MAX_TOKEN_SIZE);
		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExchangeCode;
	}
	else if (AccountCredentials.Type == TEXT("developer"))
	{
		// This is auth via the EOS auth tool
		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Developer;
		FCStringAnsi::Strncpy(Credentials.IdAnsi, TCHAR_TO_UTF8(*AccountCredentials.Id), EOS_OSS_STRING_BUFFER_LENGTH);
		FCStringAnsi::Strncpy(Credentials.TokenAnsi, TCHAR_TO_UTF8(*AccountCredentials.Token), EOS_MAX_TOKEN_SIZE);
	}
	else if (AccountCredentials.Type == TEXT("password"))
	{
		// This is using a direct username / password. Restricted and not generally available.
		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Password;
		FCStringAnsi::Strncpy(Credentials.IdAnsi, TCHAR_TO_UTF8(*AccountCredentials.Id), EOS_OSS_STRING_BUFFER_LENGTH);
		FCStringAnsi::Strncpy(Credentials.TokenAnsi, TCHAR_TO_UTF8(*AccountCredentials.Token), EOS_MAX_TOKEN_SIZE);
	}
	else if (AccountCredentials.Type == TEXT("accountportal"))
	{
		// This is auth via the EOS Account Portal
		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_AccountPortal;
	}
	else if (AccountCredentials.Type == TEXT("persistentauth"))
	{
		// Use locally stored token managed by EOSSDK keyring to attempt login.
		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_PersistentAuth;

		// Id and Token must be null when using EOS_ELoginCredentialType::EOS_LCT_PersistentAuth
		Credentials.Id = nullptr;
		Credentials.Token = nullptr;

		// Store selection of persistent auth.
		// The persistent auth token is handled by the EOSSDK. On a login failure the persistent token may need to be deleted if it is invalid.
		bIsPersistentLogin = true;
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Unable to Login() user (%d) due to missing auth parameters"), LocalUserNum);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), FString(TEXT("Missing auth parameters")));
		return false;
	}

#if ENGINE_MAJOR_VERSION == 5
	FLoginCallback* CallbackObj = new FLoginCallback(AsWeak());
#else
	FLoginCallback* CallbackObj = new FLoginCallback();
#endif
	CallbackObj->CallbackLambda = [this, LocalUserNum, bIsPersistentLogin](const EOS_Auth_LoginCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			// Continue the login process by getting the product user id for EAS only
			ConnectLoginEAS(LocalUserNum, Data->LocalUserId);
		}
		else
		{
			auto TriggerLoginFailure = [this, LocalUserNum, LoginResultCode = Data->ResultCode]()
			{
				FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(LoginResultCode)));
				UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);
				TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), ErrorString);
			};

			const bool bShouldRemoveCachedToken =
				Data->ResultCode == EOS_EResult::EOS_InvalidAuth ||
				Data->ResultCode == EOS_EResult::EOS_AccessDenied ||
				Data->ResultCode == EOS_EResult::EOS_Auth_InvalidToken;

			// Check for invalid persistent login credentials.
			if (bIsPersistentLogin && bShouldRemoveCachedToken)
			{
#if ENGINE_MAJOR_VERSION == 5
				FDeletePersistentAuthCallback* DeleteAuthCallbackObj = new FDeletePersistentAuthCallback(AsWeak());
#else
				FDeletePersistentAuthCallback* DeleteAuthCallbackObj = new FDeletePersistentAuthCallback();
#endif
				DeleteAuthCallbackObj->CallbackLambda = [this, LocalUserNum, TriggerLoginFailure](const EOS_Auth_DeletePersistentAuthCallbackInfo* Data)
				{
					// Deleting the auth token is best effort.
					TriggerLoginFailure();
				};

				EOS_Auth_DeletePersistentAuthOptions DeletePersistentAuthOptions;
				DeletePersistentAuthOptions.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
				DeletePersistentAuthOptions.RefreshToken = nullptr;
				EOS_Auth_DeletePersistentAuth(EOSSubsystem->AuthHandle, &DeletePersistentAuthOptions, (void*)DeleteAuthCallbackObj, DeleteAuthCallbackObj->GetCallbackPtr());
			}
			else
			{
				TriggerLoginFailure();
			}
		}
	};
	// Perform the auth call
	EOS_Auth_Login(EOSSubsystem->AuthHandle, &LoginOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
	return true;
}

void FUserManagerEOS::LoginViaAuthInterface(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	TArray<FString> BrokenTypeString;
	EEIK_EExternalCredentialType ExternalCredentialType = EEIK_EExternalCredentialType::EIK_ECT_EPIC;
	EEIK_ELoginCredentialType LoginCredentialType = EEIK_ELoginCredentialType::EIK_LCT_AccountPortal;
	if(AccountCredentials.Type.ParseIntoArray(BrokenTypeString, TEXT("_+_"), true) > 0)
	{
		if(BrokenTypeString.Num() > 2)
		{
			ExternalCredentialType = GetExternalCredentialType(BrokenTypeString[2]);
			LoginCredentialType = GetLoginCredentialType(BrokenTypeString[1]);
		}
		else if(BrokenTypeString.Num() > 1)
		{
			LoginCredentialType = GetLoginCredentialType(BrokenTypeString[1]);
		}
		else
		{
			UE_LOG(LogEIK, Error, TEXT("Failed to parse AccountCredentials.Type into BrokenTypeString"));
		}
	}
	else
	{
		UE_LOG(LogEIK, Error, TEXT("Failed to parse AccountCredentials.Type into BrokenTypeString"));
	}
	bool bIsPersistentLogin = false;
	EOS_Auth_LoginOptions LoginOptions = { };
	LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>();
	if(EIKSettings)
	{
		if(EIKSettings->LoginFlags.Contains(EEIK_LoginFlags_LocalForSettings::T_EOS_AS_NoFlags))
		{
			LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_NoFlags;
		}
		else
		{
			if(EIKSettings->LoginFlags.Contains(EEIK_LoginFlags_LocalForSettings::EOS_AS_Email))
			{
				LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Email;
			}
			if(EIKSettings->LoginFlags.Contains(EEIK_LoginFlags_LocalForSettings::EOS_AS_FriendsList))
			{
				LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_FriendsList;
			}
			if(EIKSettings->LoginFlags.Contains(EEIK_LoginFlags_LocalForSettings::EOS_AS_Presence))
			{
				LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Presence;
			}
			if(EIKSettings->LoginFlags.Contains(EEIK_LoginFlags_LocalForSettings::EOS_AS_Country))
			{
				LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Country;
			}
			if(EIKSettings->LoginFlags.Contains(EEIK_LoginFlags_LocalForSettings::EOS_AS_BasicProfile))
			{
				LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_BasicProfile;
			}
		}
	}
	else
	{
		UE_LOG(LogEIK, Warning, TEXT("EIKSettings is null in LoginViaAuthInterface, using default settings"));
		LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence;
	}
	EOS_Auth_Credentials TempCredentials;
	if(AccountCredentials.Id.IsEmpty())
	{
		TempCredentials.Id = nullptr;
	}
	else
	{
		const char* IdRef = new char[EOS_MAX_TOKEN_SIZE];
		FCStringAnsi::Strncpy(const_cast<char*>(IdRef), TCHAR_TO_ANSI(*AccountCredentials.Id), EOS_MAX_TOKEN_SIZE);
		TempCredentials.Id = IdRef;
	}
	if(AccountCredentials.Token.IsEmpty())
	{
		TempCredentials.Token = nullptr;
	}
	else
	{
		const char* TokenRef = new char[EOS_MAX_TOKEN_SIZE];
		FCStringAnsi::Strncpy(const_cast<char*>(TokenRef), TCHAR_TO_ANSI(*AccountCredentials.Token), EOS_MAX_TOKEN_SIZE);
		TempCredentials.Token = TokenRef;
	}
	TempCredentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	TempCredentials.Type = static_cast<EOS_ELoginCredentialType>(LoginCredentialType);
	TempCredentials.ExternalType = static_cast<EOS_EExternalCredentialType>(ExternalCredentialType);
	if(LoginCredentialType == EEIK_ELoginCredentialType::EIK_LCT_PersistentAuth)
	{
		bIsPersistentLogin = true;
	}
	LoginOptions.Credentials = &TempCredentials;
#if ENGINE_MAJOR_VERSION == 5
	FLoginCallback* CallbackObj = new FLoginCallback(AsWeak());
#else
	FLoginCallback* CallbackObj = new FLoginCallback();
#endif
	CallbackObj->CallbackLambda = [this, LocalUserNum, bIsPersistentLogin](const EOS_Auth_LoginCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			// Continue the login process by getting the product user id for EAS only
			ConnectLoginEAS(LocalUserNum, Data->LocalUserId);
		}
		else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser)
		{
			// Link the account
			LinkEAS(LocalUserNum, Data->ContinuanceToken);
		}
		else
		{
			auto TriggerLoginFailure = [this, LocalUserNum, LoginResultCode = Data->ResultCode]()
			{
				FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(LoginResultCode)));
				UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);
				TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), ErrorString);
			};

			const bool bShouldRemoveCachedToken =
				Data->ResultCode == EOS_EResult::EOS_InvalidAuth ||
				Data->ResultCode == EOS_EResult::EOS_AccessDenied ||
				Data->ResultCode == EOS_EResult::EOS_Auth_InvalidToken;

			// Check for invalid persistent login credentials.
			if (bIsPersistentLogin && bShouldRemoveCachedToken)
			{
#if ENGINE_MAJOR_VERSION == 5
				FDeletePersistentAuthCallback* DeleteAuthCallbackObj = new FDeletePersistentAuthCallback(AsWeak());
#else
				FDeletePersistentAuthCallback* DeleteAuthCallbackObj = new FDeletePersistentAuthCallback();
#endif
				DeleteAuthCallbackObj->CallbackLambda = [this, LocalUserNum, TriggerLoginFailure](const EOS_Auth_DeletePersistentAuthCallbackInfo* Data)
				{
					// Deleting the auth token is best effort.
					TriggerLoginFailure();
				};

				EOS_Auth_DeletePersistentAuthOptions DeletePersistentAuthOptions;
				DeletePersistentAuthOptions.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
				DeletePersistentAuthOptions.RefreshToken = nullptr;
				EOS_Auth_DeletePersistentAuth(EOSSubsystem->AuthHandle, &DeletePersistentAuthOptions, (void*)DeleteAuthCallbackObj, DeleteAuthCallbackObj->GetCallbackPtr());
			}
			else
			{
				TriggerLoginFailure();
			}
		}
	};
	// Perform the auth call
	EOS_Auth_Login(EOSSubsystem->AuthHandle, &LoginOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
}

void FUserManagerEOS::LoginViaConnectInterfaceCallback(const EOS_Connect_LoginCallbackInfo* Data)
{
	UE_LOG(LogEIK, Log, TEXT("LoginViaConnectInterfaceCallback called. ResultCode: %hs"), EOS_EResult_ToString(Data->ResultCode));
	if(FUserManagerEOS* UserManager = static_cast<FUserManagerEOS*>(Data->ClientData))
	{
		if(Data->ResultCode == EOS_EResult::EOS_Success)
		{
			UserManager->CompleteDeviceIDLogin(0,nullptr,Data->LocalUserId);
		}
		else if(Data->ResultCode == EOS_EResult::EOS_NotFound)
		{
			if(UserManager->LocalUserNumToLastLoginCredentials[0].Get().Type == "noeas_+_EIK_ECT_DEVICEID_ACCESS_TOKEN")
			{
				UserManager->CreateDeviceID(UserManager->LocalUserNumToLastLoginCredentials[0].Get());
			}
			else
			{
				EOS_EResult ResultCode = Data->ResultCode;
				const char* ResultCodeStr = EOS_EResult_ToString(ResultCode);
				FString ResultCodeString = FString(ResultCodeStr);
				UserManager->TriggerOnLoginCompleteDelegates(0, false, *FUniqueNetIdEOS::EmptyId(), *ResultCodeString);
			}
		}
		else if(Data->ResultCode == EOS_EResult::EOS_InvalidUser)
		{
			UserManager->CreateConnectID(Data->ContinuanceToken,UserManager->LocalUserNumToLastLoginCredentials[0].Get());
		}
		else
		{
			EOS_EResult ResultCode = Data->ResultCode;
			const char* ResultCodeStr = EOS_EResult_ToString(ResultCode);
			FString ResultCodeString = FString(ResultCodeStr);
			UserManager->TriggerOnLoginCompleteDelegates(0, false, *FUniqueNetIdEOS::EmptyId(), *ResultCodeString);

		}
	}
}

void FUserManagerEOS::LoginViaExternalAuth(int32 LocalUserNum)
{
	GetPlatformAuthToken(LocalUserNum,
#if ENGINE_MAJOR_VERSION == 5
	FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this, WeakThis = AsWeak()](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
{
	if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
	{
#else
		FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
		{
			if (FUserManagerEOSPtr StrongThis = AsShared())
			{
#endif
				if (!bWasSuccessful || !AuthToken.IsValid())
				{
					UE_LOG_ONLINE(Warning, TEXT("Unable to Login() user (%d) due to an empty platform auth token"), LocalUserNum);
					TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), FString(TEXT("Missing platform auth token")));
					return;
				}

				EOS_Auth_LoginOptions LoginOptions = { };
				LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
				LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence;

				check(LocalUserNumToLastLoginCredentials.Contains(LocalUserNum));
				FAuthCredentials Credentials(ToEOS_EExternalCredentialType(GetPlatformOSS()->GetSubsystemName(), *LocalUserNumToLastLoginCredentials[LocalUserNum]), AuthToken);
				LoginOptions.Credentials = &Credentials;
#if ENGINE_MAJOR_VERSION == 5
			FLoginCallback* CallbackObj = new FLoginCallback(AsWeak());
#else
				FLoginCallback* CallbackObj = new FLoginCallback();
#endif
			CallbackObj->CallbackLambda = [this, LocalUserNum](const EOS_Auth_LoginCallbackInfo* Data)
				{
					if (Data->ResultCode == EOS_EResult::EOS_Success)
					{
						ConnectLoginEAS(LocalUserNum, Data->LocalUserId);
					}
					else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser)
					{
						// Link the account
						LinkEAS(LocalUserNum, Data->ContinuanceToken);
					}
					else
					{
						FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
						UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);
						TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), ErrorString);
					}
				};
				// Perform the auth call
				EOS_Auth_Login(EOSSubsystem->AuthHandle, &LoginOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
			}
		}));
}

struct FLinkAccountOptions :
	public EOS_Auth_LinkAccountOptions
{
	FLinkAccountOptions(EOS_ContinuanceToken Token)
		: EOS_Auth_LinkAccountOptions()
	{
		ApiVersion = EOS_AUTH_LINKACCOUNT_API_LATEST;
		ContinuanceToken = Token;
	}
};

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Auth_OnLinkAccountCallback, EOS_Auth_LinkAccountCallbackInfo, FUserManagerEOS> FLinkAccountCallback;
#else
typedef TEOSCallback<EOS_Auth_OnLinkAccountCallback, EOS_Auth_LinkAccountCallbackInfo> FLinkAccountCallback;
#endif

void FUserManagerEOS::LinkEAS(int32 LocalUserNum, EOS_ContinuanceToken Token)
{
	FLinkAccountOptions Options(Token);
#if ENGINE_MAJOR_VERSION == 5
	FLinkAccountCallback* CallbackObj = new FLinkAccountCallback(AsWeak());
#else
	FLinkAccountCallback* CallbackObj = new FLinkAccountCallback();
#endif
	CallbackObj->CallbackLambda = [this, LocalUserNum](const EOS_Auth_LinkAccountCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			// Continue the login process by getting the product user id
			ConnectLoginEAS(LocalUserNum, Data->LocalUserId);
		}
		else
		{
			FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), ErrorString);
		}
	};
	EOS_Auth_LinkAccount(EOSSubsystem->AuthHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
}

struct FConnectCredentials :
	public EOS_Connect_Credentials
{
	FConnectCredentials(EOS_EExternalCredentialType InType, const FExternalAuthToken& AuthToken) :
		EOS_Connect_Credentials()
	{
		if (AuthToken.HasTokenData())
		{
			Init(InType, AuthToken.TokenData);
		}
		else if (AuthToken.HasTokenString())
		{
			Init(InType, AuthToken.TokenString);
		}
		else
		{
			UE_LOG_ONLINE(Error, TEXT("FConnectCredentials object cannot be constructed with invalid FExternalAuthToken parameter"));
		}
	}

	void Init(EOS_EExternalCredentialType InType, const FString& InTokenString)
	{
		ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Token = TokenAnsi;
		Type = InType;

		FCStringAnsi::Strncpy(TokenAnsi, TCHAR_TO_UTF8(*InTokenString), InTokenString.Len() + 1);
	}

	void Init(EOS_EExternalCredentialType InType, const TArray<uint8>& InToken)
	{
		ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Token = TokenAnsi;
		Type = InType;

		uint32_t InOutBufferLength = EOS_MAX_TOKEN_SIZE;
		EOS_ByteArray_ToString(InToken.GetData(), InToken.Num(), TokenAnsi, &InOutBufferLength);
	}

	char TokenAnsi[EOS_MAX_TOKEN_SIZE];
};

bool FUserManagerEOS::ConnectLoginNoEAS(int32 LocalUserNum)
{
	GetPlatformAuthToken(LocalUserNum,
#if ENGINE_MAJOR_VERSION == 5
	FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this, WeakThis = AsWeak()](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
{
	if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
	{
#else
	FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
		{
			if (FUserManagerEOSPtr StrongThis = AsShared())
			{
#endif
				if (!bWasSuccessful || !AuthToken.IsValid())
				{
					const FString ErrorString = FString::Printf(TEXT("ConnectLoginNoEAS(%d) failed due to the platform OSS giving an empty auth token"), LocalUserNum);
					UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);
					TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), ErrorString);
					return;
				}

				// Now login into our EOS account
				check(LocalUserNumToLastLoginCredentials.Contains(LocalUserNum));
				FConnectCredentials Credentials(ToEOS_EExternalCredentialType(GetPlatformOSS()->GetSubsystemName(), *LocalUserNumToLastLoginCredentials[LocalUserNum]), AuthToken);
				EOS_Connect_LoginOptions Options = { };
				Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
				Options.Credentials = &Credentials;

#if ADD_USER_LOGIN_INFO
				EOS_Connect_UserLoginInfo UserLoginInfo = {};
				UserLoginInfo.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
				const FTCHARToUTF8 DisplayNameUtf8(*GetPlatformDisplayName(LocalUserNum));
				UserLoginInfo.DisplayName = DisplayNameUtf8.Get();

				Options.UserLoginInfo = &UserLoginInfo;
#endif

#if ENGINE_MAJOR_VERSION == 5
				FConnectLoginCallback* CallbackObj = new FConnectLoginCallback(AsWeak());
#else
				FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
#endif
				CallbackObj->CallbackLambda = [this, LocalUserNum](const EOS_Connect_LoginCallbackInfo* Data)
				{
					if (Data->ResultCode == EOS_EResult::EOS_Success)
					{
						// We have an account mapping to the platform account, skip to final login
						FullLoginCallback(LocalUserNum, nullptr, Data->LocalUserId);
					}
					else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser)
					{
						// We need to create the platform account mapping for this user using the continuation token
						CreateConnectedLogin(LocalUserNum, nullptr, Data->ContinuanceToken);
					}
					else
					{
						const FString ErrorString = FString::Printf(TEXT("ConnectLoginNoEAS(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
						UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);
						TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), ErrorString);
					}
				};
				EOS_Connect_Login(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
			}
		}));

	return true;
}

bool FUserManagerEOS::ConnectLoginEAS(int32 LocalUserNum, EOS_EpicAccountId AccountId)
{
	EOS_Auth_Token* AuthToken = nullptr;
	EOS_Auth_CopyUserAuthTokenOptions CopyOptions = { };
	CopyOptions.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

	EOS_EResult CopyResult = EOS_Auth_CopyUserAuthToken(EOSSubsystem->AuthHandle, &CopyOptions, AccountId, &AuthToken);
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		EOS_Connect_Credentials Credentials = { };
		Credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Credentials.Type = EOS_EExternalCredentialType::EOS_ECT_EPIC;
		Credentials.Token = AuthToken->AccessToken;

		EOS_Connect_LoginOptions Options = { };
		Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
		Options.Credentials = &Credentials;

#if ENGINE_MAJOR_VERSION == 5
		FConnectLoginCallback* CallbackObj = new FConnectLoginCallback(AsWeak());
#else
		FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
#endif
		CallbackObj->CallbackLambda = [LocalUserNum, AccountId, this](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				// We have an account mapping, skip to final login
				FullLoginCallback(LocalUserNum, AccountId, Data->LocalUserId);
			}
			else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser)
			{
				// We need to create the mapping for this user using the continuation token
				CreateConnectedLogin(LocalUserNum, AccountId, Data->ContinuanceToken);
			}
			else
			{
				UE_LOG_ONLINE(Error, TEXT("ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
				Logout(LocalUserNum);
			}
		};
		EOS_Connect_Login(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

		EOS_Auth_Token_Release(AuthToken);
	}
	else
	{
		UE_LOG_ONLINE(Error, TEXT("ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(CopyResult)));
		Logout(LocalUserNum);
	}
	return true;
}

void FUserManagerEOS::RefreshConnectLogin(int32 LocalUserNum)
{
	if (!UserNumToAccountIdMap.Contains(LocalUserNum))
	{
		UE_LOG_ONLINE(Error, TEXT("Can't refresh ConnectLogin(%d) since (%d) is not logged in"), LocalUserNum, LocalUserNum);
		return;
	}

	if(LocalUserNumToLastLoginCredentials[0]->Type == TEXT("deviceid"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Refresh Connect Login for Device ID"));
		LoginViaConnectInterface(*LocalUserNumToLastLoginCredentials[0]);
		return;
	}

	const FEOSSettings Settings = UEIKSettings::GetSettings();
	if (true)
	{
		EOS_EpicAccountId AccountId = UserNumToAccountIdMap[LocalUserNum];
		EOS_Auth_Token* AuthToken = nullptr;
		EOS_Auth_CopyUserAuthTokenOptions CopyOptions = { };
		CopyOptions.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

		EOS_EResult CopyResult = EOS_Auth_CopyUserAuthToken(EOSSubsystem->AuthHandle, &CopyOptions, AccountId, &AuthToken);
		if (CopyResult == EOS_EResult::EOS_Success)
		{
			// We update the auth token cached in the user account, along with the user information
			const FUniqueNetIdEOSPtr UniqueNetId = UserNumToNetIdMap.FindChecked(LocalUserNum);
			const FUserOnlineAccountEOSRef UserAccountRef = StringToUserAccountMap.FindChecked(UniqueNetId->ToString());
			UserAccountRef->SetAuthAttribute(AUTH_ATTR_ID_TOKEN, AuthToken->AccessToken);
			UpdateUserInfo(UserAccountRef, AccountId, AccountId);

			EOS_Connect_Credentials Credentials = { };
			Credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
			Credentials.Type = EOS_EExternalCredentialType::EOS_ECT_EPIC;
			Credentials.Token = AuthToken->AccessToken;

			EOS_Connect_LoginOptions Options = { };
			Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
			Options.Credentials = &Credentials;
#if ENGINE_MAJOR_VERSION == 5
			FConnectLoginCallback* CallbackObj = new FConnectLoginCallback(AsWeak());
#else
			FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
#endif
			CallbackObj->CallbackLambda = [LocalUserNum, AccountId, this](const EOS_Connect_LoginCallbackInfo* Data)
			{
				if (Data->ResultCode != EOS_EResult::EOS_Success)
				{
					UE_LOG_ONLINE(Error, TEXT("Failed to refresh ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
					Logout(LocalUserNum);
				}
			};
			EOS_Connect_Login(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

			EOS_Auth_Token_Release(AuthToken);
		}
		else
		{
			UE_LOG_ONLINE(Error, TEXT("Failed to refresh ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(CopyResult)));
			Logout(LocalUserNum);
		}
	}
	else
	{
#if ENGINE_MAJOR_VERSION == 5
		// Not using EAS so grab the platform auth token
GetPlatformAuthToken(LocalUserNum,
	FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this, WeakThis = AsWeak()](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
	{
		if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
		{
#else
		// Not using EAS so grab the platform auth token
		GetPlatformAuthToken(LocalUserNum,
			FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
			{
				if (FUserManagerEOSPtr StrongThis = AsShared())
				{
#endif
					if (!bWasSuccessful || !AuthToken.IsValid())
					{
						UE_LOG_ONLINE(Error, TEXT("ConnectLoginNoEAS(%d) failed due to the platform OSS giving an empty auth token"), LocalUserNum);
						Logout(LocalUserNum);
						return;
					}

					// Now login into our EOS account
					check(LocalUserNumToLastLoginCredentials.Contains(LocalUserNum));
					const FOnlineAccountCredentials& Creds = *LocalUserNumToLastLoginCredentials[LocalUserNum];
					EOS_EExternalCredentialType CredType = ToEOS_EExternalCredentialType(GetPlatformOSS()->GetSubsystemName(), Creds);
					FConnectCredentials Credentials(CredType, AuthToken);
					EOS_Connect_LoginOptions Options = { };
					Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
					Options.Credentials = &Credentials;

#if ENGINE_MAJOR_VERSION == 5
					FConnectLoginCallback* CallbackObj = new FConnectLoginCallback(AsWeak());
#else
					FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
#endif
					CallbackObj->CallbackLambda = [this, LocalUserNum](const EOS_Connect_LoginCallbackInfo* Data)
					{
						if (Data->ResultCode != EOS_EResult::EOS_Success)
						{
							UE_LOG_ONLINE(Error, TEXT("Failed to refresh ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
							Logout(LocalUserNum);
						}
					};
					EOS_Connect_Login(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
				}
			}));
	}
}

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Connect_OnCreateUserCallback, EOS_Connect_CreateUserCallbackInfo, FUserManagerEOS> FCreateUserCallback;
#else
typedef TEOSCallback<EOS_Connect_OnCreateUserCallback, EOS_Connect_CreateUserCallbackInfo> FCreateUserCallback;
#endif

void FUserManagerEOS::CreateConnectedLogin(int32 LocalUserNum, EOS_EpicAccountId AccountId, EOS_ContinuanceToken Token)
{
	EOS_Connect_CreateUserOptions Options = { };
	Options.ApiVersion = EOS_CONNECT_CREATEUSER_API_LATEST;
	Options.ContinuanceToken = Token;

#if ENGINE_MAJOR_VERSION == 5
	FCreateUserCallback* CallbackObj = new FCreateUserCallback(AsWeak());
#else
	FCreateUserCallback* CallbackObj = new FCreateUserCallback();
#endif
	CallbackObj->CallbackLambda = [LocalUserNum, AccountId, this](const EOS_Connect_CreateUserCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			FullLoginCallback(LocalUserNum, AccountId, Data->LocalUserId);
		}
		else
		{
// @todo joeg - logout?
			FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdEOS::EmptyId(), ErrorString);
		}
	};
	EOS_Connect_CreateUser(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
}

#if ENGINE_MAJOR_VERSION == 5
typedef TEIKGlobalCallback<EOS_Connect_OnAuthExpirationCallback, EOS_Connect_AuthExpirationCallbackInfo, FUserManagerEOS> FRefreshAuthCallback;
typedef TEIKGlobalCallback<EOS_Presence_OnPresenceChangedCallback, EOS_Presence_PresenceChangedCallbackInfo, FUserManagerEOS> FPresenceChangedCallback;
typedef TEIKGlobalCallback<EOS_Friends_OnFriendsUpdateCallback, EOS_Friends_OnFriendsUpdateInfo, FUserManagerEOS> FFriendsStatusUpdateCallback;
typedef TEIKGlobalCallback<EOS_Auth_OnLoginStatusChangedCallback, EOS_Auth_LoginStatusChangedCallbackInfo, FUserManagerEOS> FLoginStatusChangedCallback;
#else
typedef TEIKGlobalCallback<EOS_Connect_OnAuthExpirationCallback, EOS_Connect_AuthExpirationCallbackInfo> FRefreshAuthCallback;
typedef TEIKGlobalCallback<EOS_Presence_OnPresenceChangedCallback, EOS_Presence_PresenceChangedCallbackInfo> FPresenceChangedCallback;
typedef TEIKGlobalCallback<EOS_Friends_OnFriendsUpdateCallback, EOS_Friends_OnFriendsUpdateInfo> FFriendsStatusUpdateCallback;
typedef TEIKGlobalCallback<EOS_Auth_OnLoginStatusChangedCallback, EOS_Auth_LoginStatusChangedCallbackInfo> FLoginStatusChangedCallback;
#endif
void FUserManagerEOS::FullLoginCallback(int32 LocalUserNum, EOS_EpicAccountId AccountId, EOS_ProductUserId UserId)
{
	// Add our login status changed callback if not already set
	if (LoginNotificationId == 0)
	{
#if ENGINE_MAJOR_VERSION == 5
		FLoginStatusChangedCallback* CallbackObj = new FLoginStatusChangedCallback(AsWeak());
#else
		FLoginStatusChangedCallback* CallbackObj = new FLoginStatusChangedCallback();
#endif
		LoginNotificationCallback = CallbackObj;
		CallbackObj->CallbackLambda = [this](const EOS_Auth_LoginStatusChangedCallbackInfo* Data)
		{
			LoginStatusChanged(Data);
		};

		EOS_Auth_AddNotifyLoginStatusChangedOptions Options = { };
		Options.ApiVersion = EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST;
		LoginNotificationId = EOS_Auth_AddNotifyLoginStatusChanged(EOSSubsystem->AuthHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	// Register for friends updates if not set yet
	if (FriendsNotificationId == 0)
	{
#if ENGINE_MAJOR_VERSION == 5
		FFriendsStatusUpdateCallback* CallbackObj = new FFriendsStatusUpdateCallback(AsWeak());
#else
		FFriendsStatusUpdateCallback* CallbackObj = new FFriendsStatusUpdateCallback();
#endif
		FriendsNotificationCallback = CallbackObj;
		CallbackObj->CallbackLambda = [LocalUserNum, this](const EOS_Friends_OnFriendsUpdateInfo* Data)
		{
			FriendStatusChanged(Data);
		};

		EOS_Friends_AddNotifyFriendsUpdateOptions Options = { };
		Options.ApiVersion = EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST;
		FriendsNotificationId = EOS_Friends_AddNotifyFriendsUpdate(EOSSubsystem->FriendsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	// Register for presence updates if not set yet
	if (PresenceNotificationId == 0)
	{
#if ENGINE_MAJOR_VERSION == 5
		FPresenceChangedCallback* CallbackObj = new FPresenceChangedCallback(AsWeak());
#else
		FPresenceChangedCallback* CallbackObj = new FPresenceChangedCallback();
#endif
		PresenceNotificationCallback = CallbackObj;
		CallbackObj->CallbackLambda = [LocalUserNum, this](const EOS_Presence_PresenceChangedCallbackInfo* Data)
		{
			if (EpicAccountIdToOnlineUserMap.Contains(Data->PresenceUserId))
			{
				// Update the presence data to the most recent
				UpdatePresence(Data->PresenceUserId);
				return;
			}
		};

		EOS_Presence_AddNotifyOnPresenceChangedOptions Options = { };
		Options.ApiVersion = EOS_PRESENCE_ADDNOTIFYONPRESENCECHANGED_API_LATEST;
		PresenceNotificationId = EOS_Presence_AddNotifyOnPresenceChanged(EOSSubsystem->PresenceHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	// Add auth refresh notification if not set for this user yet
	if (!LocalUserNumToConnectLoginNotifcationMap.Contains(LocalUserNum))
	{
		FNotificationIdCallbackPair* NotificationPair = new FNotificationIdCallbackPair();
		LocalUserNumToConnectLoginNotifcationMap.Emplace(LocalUserNum, NotificationPair);
#if ENGINE_MAJOR_VERSION == 5
		FRefreshAuthCallback* CallbackObj = new FRefreshAuthCallback(AsWeak());
#else
		FRefreshAuthCallback* CallbackObj = new FRefreshAuthCallback();
#endif
		NotificationPair->Callback = CallbackObj;
		CallbackObj->CallbackLambda = [LocalUserNum, this](const EOS_Connect_AuthExpirationCallbackInfo* Data)
		{
			RefreshConnectLogin(LocalUserNum);
		};

		EOS_Connect_AddNotifyAuthExpirationOptions Options = { };
		Options.ApiVersion = EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST;
		NotificationPair->NotificationId = EOS_Connect_AddNotifyAuthExpiration(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}

	AddLocalUser(LocalUserNum, AccountId, UserId);
	FUniqueNetIdEOSPtr UserNetId = GetLocalUniqueNetIdEOS(LocalUserNum);
	check(UserNetId.IsValid());

	TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserNetId, FString());
	TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::NotLoggedIn, ELoginStatus::LoggedIn, *UserNetId);
}

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Auth_OnLogoutCallback, EOS_Auth_LogoutCallbackInfo, FUserManagerEOS> FLogoutCallback;
#else
typedef TEOSCallback<EOS_Auth_OnLogoutCallback, EOS_Auth_LogoutCallbackInfo> FLogoutCallback;
#endif
bool FUserManagerEOS::Logout(int32 LocalUserNum)
{
	FUniqueNetIdEOSPtr UserId = GetLocalUniqueNetIdEOS(LocalUserNum);
	if (!UserId.IsValid())
	{
		UE_LOG_ONLINE(Warning, TEXT("No logged in user found for LocalUserNum=%d."),
			LocalUserNum);
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
		return false;
	}
	if(UserId.Get()->GetEpicAccountId())
	{
#if ENGINE_MAJOR_VERSION == 5
		FLogoutCallback* CallbackObj = new FLogoutCallback(AsWeak());
CallbackObj->CallbackLambda = [LocalUserNum, this](const EOS_Auth_LogoutCallbackInfo* Data)
{
	FDeletePersistentAuthCallback* DeleteAuthCallbackObj = new FDeletePersistentAuthCallback(AsWeak());
#else
		FLogoutCallback* CallbackObj = new FLogoutCallback();
		CallbackObj->CallbackLambda = [LocalUserNum, this](const EOS_Auth_LogoutCallbackInfo* Data)
		{
			FDeletePersistentAuthCallback* DeleteAuthCallbackObj = new FDeletePersistentAuthCallback();
#endif
			DeleteAuthCallbackObj->CallbackLambda = [this, LocalUserNum, LogoutResultCode = Data->ResultCode](const EOS_Auth_DeletePersistentAuthCallbackInfo* Data)
			{
				if (LogoutResultCode == EOS_EResult::EOS_Success)
				{
					RemoveLocalUser(LocalUserNum);

					TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
				}
				else
				{
					TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
				}
			};

			EOS_Auth_DeletePersistentAuthOptions DeletePersistentAuthOptions;
			DeletePersistentAuthOptions.ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;
			DeletePersistentAuthOptions.RefreshToken = nullptr;
			EOS_Auth_DeletePersistentAuth(EOSSubsystem->AuthHandle, &DeletePersistentAuthOptions, (void*)DeleteAuthCallbackObj, DeleteAuthCallbackObj->GetCallbackPtr());
		};
		EOS_Auth_LogoutOptions LogoutOptions = { };
		LogoutOptions.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
		LogoutOptions.LocalUserId = UserId->GetEpicAccountId();

		EOS_Auth_Logout(EOSSubsystem->AuthHandle, &LogoutOptions, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	else
	{
		RemoveLocalUser(LocalUserNum);
		TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
	}
	LocalUserNumToLastLoginCredentials.Remove(LocalUserNum);

	return true;
}

void FUserManagerEOS::Tick(float DeltaTime)
{
	UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/OnlineSubsystemEIK.EEIK_EExternalCredentialType"));
	UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>();
	if(EnumPtr && EIKSettings)
	{
		if(!bAutoLoginAttempted)
		{
			AutoLogin(0);
			UWorld* World;
			if(GEngine)
			{
				UE_LOG(LogEIK, Verbose, TEXT("AutoLaunchDevTool: GEngine is valid"));
				World = GEngine->GetWorldContexts()[0].World();
				if(World)
				{
					if(World->WorldType == EWorldType::Editor)
					{
						if(EIKSettings->bAutoLaunchDevTool)
						{
							UE_LOG(LogEIK, Log, TEXT("World is a PIE world. Launching DevTool as auto launch is enabled"));
							LaunchDevTool();
						}
					}
				}
				else
				{
					UE_LOG(LogEIK, Warning, TEXT("World is not valid. Skipping auto launch of DevTool"));
				}
			}
		}
	}
}

bool FUserManagerEOS::AutoLogin(int32 LocalUserNum)
{
	FString LoginId;
	FString Password;
	FString AuthType;

	FParse::Value(FCommandLine::Get(), TEXT("AUTH_LOGIN="), LoginId);
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_PASSWORD="), Password);
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

	FEOSSettings Settings = UEIKSettings::GetSettings();

	if (EOSSubsystem->bIsDefaultOSS && AuthType.IsEmpty())
	{
		UE_LOG(LogEIK, Log, TEXT("AutoLogin: Unable to AutoLogin user (%d) due to missing auth command line args, attempting to auto login with settings"), LocalUserNum);
		return AutoLoginUsingSettings(LocalUserNum);
	}
	UE_LOG(LogEIK, Log, TEXT("AutoLogin: Attempting to auto login user (%d) with auth command line args"), LocalUserNum);
	FOnlineAccountCredentials Creds(AuthType, LoginId, Password);
	if(AuthType == "exchangecode")
	{
		Creds.Type = "eas_+_EIK_LCT_ExchangeCode_+_EIK_ECT_EPIC";
		Creds.Token = Password;
		Creds.Id = LoginId;
	}
	else if(AuthType == "deviceid")
	{
		Creds.Id = "deviceid";
		Creds.Type = "noeas_+_EIK_ECT_DEVICEID_ACCESS_TOKEN";
	}
	else if(AuthType == "persistentauth")
	{
		Creds.Type = "eas_+_EIK_LCT_PersistentAuth_+_EIK_ECT_EPIC";
		Creds.Token = Password;
		Creds.Id = LoginId;
	}
	else if(AuthType == "accountportal")
	{
		Creds.Type = "eas_+_EIK_LCT_AccountPortal+_EIK_ECT_EPIC";
		Creds.Token = Password;
		Creds.Id = LoginId;
	}
	LocalUserNumToLastLoginCredentials.Emplace(LocalUserNum, MakeShared<FOnlineAccountCredentials>(Creds));
	bAutoLoginAttempted = true;
	bAutoLoginInProgress = false;
	return Login(LocalUserNum, Creds);
}

void FUserManagerEOS::LaunchDevTool()
{
	IPluginManager& PluginManager = IPluginManager::Get();
	TSharedPtr<IPlugin> EOSPlugin = PluginManager.FindPlugin(TEXT("EOSIntegrationKit"));
	if(!EOSPlugin.IsValid())
	{
		UE_LOG(LogEIK, Error, TEXT("EOSIntegrationKit plugin not found"));
		return;
	}
	FString PluginRoot = EOSPlugin->GetBaseDir();
	auto MainModulePath = FPaths::Combine(*PluginRoot, TEXT("Source/ThirdParty/EIKSDK/Tools/EOS_DevAuthTool-win32-x64-1.2.0"));
	FString DevToolPath = FPaths::Combine(*MainModulePath, TEXT("EOS_DevAuthTool.exe"));
	FString DevToolArgs = TEXT("");
	FPlatformProcess::CreateProc(*DevToolPath, *DevToolArgs, true, false, false, nullptr, 0, nullptr, nullptr);
}

bool FUserManagerEOS::AutoLoginUsingSettings(int32 LocalUserNum)
{
	if(bAutoLoginAttempted)
	{
		UE_LOG(LogEIK, Warning, TEXT("AutoLoginUsingSettings: Auto login has already been attempted. Skipping auto login"));
		return false;
	}
	UE_LOG(LogEIK, Verbose, TEXT("Checking if the user should be auto logged in"));
	UWorld* World;
	if(GEngine)
	{
		World = GEngine->GetWorldContexts()[0].World();
		if(World)
		{
			if(World->WorldType != EWorldType::Game)
			{
				UE_LOG(LogEIK, Display, TEXT("World is not a game world. Skipping auto login"));
				bAutoLoginAttempted = true;
				return false;
			}
		}
		else
		{
			UE_LOG(LogEIK, Warning, TEXT("World is not valid. Skipping auto login"));
			bAutoLoginAttempted = true;
			return false;
		}
	}
	else
	{
		UE_LOG(LogEIK, Warning, TEXT("GEngine is not valid. Skipping auto login"));
		bAutoLoginAttempted = true;
		return false;
	}
	UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/OnlineSubsystemEIK.EEIK_EExternalCredentialType"));
	UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>();
	if(EnumPtr && EIKSettings)
	{
		UE_LOG(LogEIK, Verbose, TEXT("AutoLogin: Enum found for EEIK_EExternalCredentialType. Starting auto login"));
		if(EIKSettings->AutoLoginType == EEIK_AutoLoginType::AutoLogin_None)
		{
			UE_LOG(LogEIK, Display, TEXT("AutoLogin: AutoLoginType is set to None. Skipping auto login"));
			bAutoLoginAttempted = true;
			return false;
		}
		OnLoginCompleteDelegates->AddLambda([this, EIKSettings](int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
		{
			if(!bWasSuccessful && bAutoLoginInProgress)
			{
				UE_LOG(LogEIK, Warning, TEXT("AutoLogin: Login failed for user (%d). Attempting to auto login with fallback"), LocalUserNum);
				ClearOnLoginCompleteDelegates(0,this);
				AutoLoginWithFallback(LocalUserNum);
				return;
			}
			ClearOnLoginCompleteDelegates(0,this);
		});
		FOnlineAccountCredentials TempDetails;
		switch (EIKSettings->AutoLoginType)
		{
		case AutoLogin_None:
			UE_LOG(LogEIK, Warning, TEXT("AutoLogin: AutoLoginType is set to None. Skipping auto login"));
			return true;
			break;
		case AutoLogin_PersistentAuth:
			TempDetails.Type = "eas_+_EIK_LCT_PersistentAuth_+_EIK_ECT_EPIC";
			Login(0,TempDetails);
			bAutoLoginAttempted = true;
			bAutoLoginInProgress = true;
			return true;
			break;
		case AutoLogin_DeviceIdLogin:
			TempDetails.Type = "noeas_+_EIK_ECT_DEVICEID_ACCESS_TOKEN";
			TempDetails.Id = "deviceid";
			bAutoLoginInProgress = true;
			Login(0,TempDetails);
			bAutoLoginAttempted = true;
			return true;
			break;
		case AutoLogin_AccountPortalLogin:
			TempDetails.Type = "eas_+_EIK_LCT_AccountPortal+_EIK_ECT_EPIC";
			bAutoLoginInProgress = true;
			Login(0,TempDetails);
			bAutoLoginAttempted = true;
			return true;
			break;
		case AutoLogin_PlatformLogin:
			bAutoLoginAttempted = true;
			bAutoLoginInProgress = false;
			break;
/*		case AutoLogin_OculusLogin:
#if SUPPORTOCULUSPLATFORM
{
    OvrPlatform_User_GetLoggedInUser_Delegate GetLoggedInUser_Delegate;

    GetLoggedInUser_Delegate.BindLambda([&](bool bSuccess, FOvrUserPtr StringPtrParam, FString StringParam)
    {
        if(!bSuccess)
        {
            UE_LOG(LogEIK, Warning, TEXT("AutoLogin: Failed to get logged in user. Skipping auto login"));
            bAutoLoginAttempted = true;
            bAutoLoginInProgress = false;
            AutoLoginWithFallback(0);
            return;
        }
        OvrPlatform_User_GetUserProof_Delegate GetUserProof_Delegate;
        GetUserProof_Delegate.BindLambda([&](bool bSuccess2, FOvrUserProofPtr StringVal, FString StringParam2)
        {
            if(!bSuccess2)
            {
                UE_LOG(LogEIK, Warning, TEXT("AutoLogin: Failed to get user proof. Skipping auto login"));
                bAutoLoginAttempted = true;
                bAutoLoginInProgress = false;
                AutoLoginWithFallback(0);
                return;
            }
            FOnlineAccountCredentials TempDetails;
            if(EIKSettings->bUse_EAS_ForAutoLogin)
            {
                TempDetails.Type = "eas_+_EIK_LCT_ExternalAuth_+_EIK_ECT_OCULUS_USERID_NONCE";
            }
            else
            {
                TempDetails.Type = "noeas_+_EIK_ECT_OCULUS_USERID_NONCE";
            }
            TempDetails.Token = "{" + StringPtrParam.Get()->OculusID + "}|{" + *StringVal.Get()->Nonce + "}";
            Login(0, TempDetails);
            bAutoLoginAttempted = true;
            bAutoLoginInProgress = true;
        });
        //OvrPlatform_User_GetUserProof(UGameplayStatics::GetGameInstance(World), MoveTemp(GetUserProof_Delegate));
    });
   OvrPlatform_User_GetLoggedInUser(UGameplayStatics::GetGameInstance(World), std::move(GetLoggedInUser_Delegate));
}
#else
UE_LOG(LogEIK, Warning, TEXT("AutoLogin: Oculus login is not supported in this build. Skipping auto login"));
bAutoLoginAttempted = true;
bAutoLoginInProgress = false;
AutoLoginWithFallback(0);
#endif
			break;*/
		case AutoLogin_SteamLogin:
#if ENGINE_MAJOR_VERSION == 5
			GetPlatformAuthToken(0,FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this,EIKSettings, WeakThis = AsWeak()](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
{
	if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
#else
			GetPlatformAuthToken(0,FOnGetLinkedAccountAuthTokenCompleteDelegate::CreateLambda([this,EIKSettings](int32 LocalUserNum, bool bWasSuccessful, const FExternalAuthToken& AuthToken)
			{
				if (FUserManagerEOSPtr StrongThis = AsShared())
#endif
					{
					if (!bWasSuccessful || !AuthToken.IsValid())
					{
						UE_LOG(LogEIK, Warning, TEXT("Unable to AutoLogin user (%d) due to an empty platform auth token"), LocalUserNum);
						AutoLoginWithFallback(LocalUserNum);
						bAutoLoginAttempted = true;
						return;
					}
					if(!AuthToken.HasTokenString())
					{
						UE_LOG(LogEIK, Warning, TEXT("Unable to AutoLogin user (%d) due to an empty platform auth token"), LocalUserNum);
						AutoLoginWithFallback(LocalUserNum);
						bAutoLoginAttempted = true;
						return;
					}
					FOnlineAccountCredentials TempDetails;
					if(EIKSettings->bUse_EAS_ForAutoLogin)
					{
						TempDetails.Type = "eas_+_EIK_LCT_ExternalAuth_+_EIK_ECT_STEAM_SESSION_TICKET";
					}
					else
					{
						TempDetails.Type = "noeas_+_EIK_ECT_STEAM_SESSION_TICKET";
						TempDetails.Token = AuthToken.TokenString;
					}
					Login(0,TempDetails);
				}
			}));
			bAutoLoginAttempted = true;
			bAutoLoginInProgress = true;
			return true;
			break;
		case AutoLogin_PSNLogin:
			break;
		case AutoLogin_GoogleLogin:
			break;
		case AutoLogin_AppleLogin:
			break;
		case AutoLogin_DeveloperTool:
#if WITH_EDITOR
			{
				FString CommandLine = FString(FCommandLine::Get());
				FString SearchString = TEXT("GameUserSettingsINI=PIEGameUserSettings");

				int32 FoundIndex;
				if (CommandLine.FindChar('=', FoundIndex))
				{
					int32 StartIndex = CommandLine.Find(SearchString);
					if (StartIndex != INDEX_NONE)
					{
						StartIndex += SearchString.Len();
						int32 EndIndex = CommandLine.Find(TEXT(" "), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
						if (EndIndex == INDEX_NONE)
						{
							EndIndex = CommandLine.Len();
						}
						FString NumberString = CommandLine.Mid(StartIndex, EndIndex - StartIndex);
						UE_LOG(LogEIK, Log, TEXT("AutoLogin: Found GameUserSettingsINI=PIEGameUserSettings. Using LocalUserNum: %d"), FCString::Atoi(*NumberString));
						TempDetails.Type = "eas_+_EIK_LCT_Developer_+_EIK_ECT_EPIC";
						TempDetails.Id = EIKSettings->DeveloperToolUrl;
						int32 TempUserNum = FCString::Atoi(*NumberString) + 1;
						UE_LOG(LogEIK, Log, TEXT("AutoLogin: LocalUserNum: %d"), TempUserNum);
						TempDetails.Token = "Context_" + FString::FromInt(TempUserNum);
						Login(0,TempDetails);
						bAutoLoginAttempted = true;
						bAutoLoginInProgress = true;
						return true;
					}
				}
				UE_LOG(LogEIK, Warning, TEXT("AutoLogin: DeveloperTool is enabled but GameUserSettingsINI=PIEGameUserSettings is not found. Skipping auto login"));
				bAutoLoginAttempted = true;
				bAutoLoginInProgress = false;
				return AutoLoginWithFallback(0);
			}
#endif
			default:
			UE_LOG(LogEIK, Warning, TEXT("AutoLogin: Unknown AutoLoginType. Skipping auto login"));
			bAutoLoginAttempted = true;
			return false;
		}
	}
	else
	{
		UE_LOG(LogEIK, Log, TEXT("AutoLogin: Enum not found for EEIK_EExternalCredentialType. Will retry auto login"));
		bAutoLoginAttempted = false;
		return false;
	}
	return true;
}

bool FUserManagerEOS::AutoLoginWithFallback(int32 LocalUserNum)
{
	UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>();
	switch(EIKSettings->FallbackForAutoLoginType)
	{
	case Fallback_None:
		UE_LOG(LogEIK, Warning, TEXT("No fallback for AutoLogin, stopping autolog process"));
		break;
	case Fallback_DeviceIdLogin:
		{
			FOnlineAccountCredentials TempDetails;
			TempDetails.Type = "noeas_+_EIK_ECT_DEVICEID_ACCESS_TOKEN";
			TempDetails.Id = "deviceid";
			Login(0,TempDetails);
		}
		break;
	case Fallback_AccountPortalLogin:
		{
			FOnlineAccountCredentials TempDetails;
			TempDetails.Type = "eas_+_EIK_LCT_AccountPortal+_EIK_ECT_EPIC";
			Login(0,TempDetails);
		}
		break;
	default: ;
	}
	return true;
}

void FUserManagerEOS::AddLocalUser(int32 LocalUserNum, EOS_EpicAccountId EpicAccountId, EOS_ProductUserId UserId)
{
	// Set the default user to the first one that logs in
	if (DefaultLocalUser == -1)
	{
		DefaultLocalUser = LocalUserNum;
	}

	FUniqueNetIdEOSRef UserNetId = FUniqueNetIdEOSRegistry::FindOrAdd(EpicAccountId, UserId).ToSharedRef();
	const FString& NetId = UserNetId->ToString();
	FUserOnlineAccountEOSRef UserAccountRef(new FUserOnlineAccountEOS(UserNetId));

	UserNumToNetIdMap.Emplace(LocalUserNum, UserNetId);
	UserNumToAccountIdMap.Emplace(LocalUserNum, EpicAccountId);
	AccountIdToUserNumMap.Emplace(EpicAccountId, LocalUserNum);
	NetIdStringToOnlineUserMap.Emplace(*NetId, UserAccountRef);
	StringToUserAccountMap.Emplace(NetId, UserAccountRef);
	AccountIdToStringMap.Emplace(EpicAccountId, NetId);
	ProductUserIdToStringMap.Emplace(UserId, *NetId);
	EpicAccountIdToAttributeAccessMap.Emplace(EpicAccountId, UserAccountRef);
	UserNumToProductUserIdMap.Emplace(LocalUserNum, UserId);
	ProductUserIdToUserNumMap.Emplace(UserId, LocalUserNum);

	// Init player lists
	FFriendsListEOSRef FriendsList = MakeShareable(new FFriendsListEOS(LocalUserNum, UserNetId));
	LocalUserNumToFriendsListMap.Emplace(LocalUserNum, FriendsList);
	NetIdStringToFriendsListMap.Emplace(NetId, FriendsList);
	ReadFriendsList(LocalUserNum, FString());

	FBlockedPlayersListEOSRef BlockedPlayersList = MakeShareable(new FBlockedPlayersListEOS(LocalUserNum, UserNetId));
	LocalUserNumToBlockedPlayerListMap.Emplace(LocalUserNum, BlockedPlayersList);
	NetIdStringToBlockedPlayerListMap.Emplace(NetId, BlockedPlayersList);
	QueryBlockedPlayers(*UserNetId);

	FRecentPlayersListEOSRef RecentPlayersList = MakeShareable(new FRecentPlayersListEOS(LocalUserNum, UserNetId));
	LocalUserNumToRecentPlayerListMap.Emplace(LocalUserNum, RecentPlayersList);
	NetIdStringToRecentPlayerListMap.Emplace(NetId, RecentPlayersList);

	// Get auth token info
	EOS_Auth_Token* AuthToken = nullptr;
	EOS_Auth_CopyUserAuthTokenOptions Options = { };
	Options.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

	EOS_EResult CopyResult = EOS_Auth_CopyUserAuthToken(EOSSubsystem->AuthHandle, &Options, EpicAccountId, &AuthToken);
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		UserAccountRef->SetAuthAttribute(AUTH_ATTR_ID_TOKEN, AuthToken->AccessToken);
		EOS_Auth_Token_Release(AuthToken);

		UpdateUserInfo(UserAccountRef, EpicAccountId, EpicAccountId);
	}
}

void FUserManagerEOS::UpdateUserInfo(IAttributeAccessInterfaceRef AttributeAccessRef, EOS_EpicAccountId LocalId, EOS_EpicAccountId AccountId)
{
	EOS_UserInfo_CopyUserInfoOptions Options = { };
	Options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
	Options.LocalUserId = LocalId;
	Options.TargetUserId = AccountId;

	EOS_UserInfo* UserInfo = nullptr;

	EOS_EResult CopyResult = EOS_UserInfo_CopyUserInfo(EOSSubsystem->UserInfoHandle, &Options, &UserInfo);
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		AttributeAccessRef->SetInternalAttribute(USER_ATTR_DISPLAY_NAME, UTF8_TO_TCHAR(UserInfo->DisplayName));
		AttributeAccessRef->SetInternalAttribute(USER_ATTR_COUNTRY, UTF8_TO_TCHAR(UserInfo->Country));
		AttributeAccessRef->SetInternalAttribute(USER_ATTR_LANG, UTF8_TO_TCHAR(UserInfo->PreferredLanguage));
		EOS_UserInfo_Release(UserInfo);
	}
}

bool FUserManagerEOS::IsFriendQueryUserInfoOngoing(int32 LocalUserNum)
{
	// If we have an entry for this user and the corresponding array has any element, users are still being processed
	if (IsFriendQueryUserInfoOngoingForLocalUserMap.Contains(LocalUserNum))
	{
		if(IsFriendQueryUserInfoOngoingForLocalUserMap[LocalUserNum].Num() > 0)
		{
			return true;
		}
	}
	
	if (IsPlayerQueryExternalMappingsOngoingForLocalUserMap.Contains(LocalUserNum))
	{
		if(IsPlayerQueryExternalMappingsOngoingForLocalUserMap[LocalUserNum].Num() > 0)
		{
			return true;
		}
	}

	return false;
}

TSharedPtr<FUserOnlineAccount> FUserManagerEOS::GetUserAccount(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> Result;

	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(UserId);
	const FUserOnlineAccountEOSRef* FoundUserAccount = StringToUserAccountMap.Find(EOSID.ToString());
	if (FoundUserAccount != nullptr)
	{
		return *FoundUserAccount;
	}

	return nullptr;
}

TArray<TSharedPtr<FUserOnlineAccount>> FUserManagerEOS::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount>> Result;

	for (TMap<FString, FUserOnlineAccountEOSRef>::TConstIterator It(StringToUserAccountMap); It; ++It)
	{
		Result.Add(It.Value());
	}
	return Result;
}

FUniqueNetIdPtr FUserManagerEOS::GetUniquePlayerId(int32 LocalUserNum) const
{
	return GetLocalUniqueNetIdEOS(LocalUserNum);
}

int32 FUserManagerEOS::GetLocalUserNumFromUniqueNetId(const FUniqueNetId& NetId) const
{
	const FUniqueNetIdEOS& EosId = FUniqueNetIdEOS::Cast(NetId);

	const EOS_EpicAccountId AccountId = EosId.GetEpicAccountId();
	if (const int32* UserNum = AccountIdToUserNumMap.Find(AccountId))
	{
		return *UserNum;
	}

	const EOS_ProductUserId ProductUserId = EosId.GetProductUserId();
	if (const int32* UserNum = ProductUserIdToUserNumMap.Find(ProductUserId))
	{
		return *UserNum;
	}

	// Use the default user if we can't find the person that they want
	return DefaultLocalUser;
}

bool FUserManagerEOS::IsLocalUser(const FUniqueNetId& NetId) const
{
	const FUniqueNetIdEOS& EosId = FUniqueNetIdEOS::Cast(NetId);
	return AccountIdToUserNumMap.Contains(EosId.GetEpicAccountId()) || ProductUserIdToUserNumMap.Contains(EosId.GetProductUserId());
}

FUniqueNetIdEOSPtr FUserManagerEOS::GetLocalUniqueNetIdEOS(int32 LocalUserNum) const
{
	const FUniqueNetIdEOSPtr* FoundId = UserNumToNetIdMap.Find(LocalUserNum);
	if (FoundId != nullptr)
	{
		return *FoundId;
	}
	return nullptr;
}

FUniqueNetIdEOSPtr FUserManagerEOS::GetLocalUniqueNetIdEOS(EOS_ProductUserId UserId) const
{
	if (ProductUserIdToUserNumMap.Contains(UserId))
	{
		return GetLocalUniqueNetIdEOS(ProductUserIdToUserNumMap[UserId]);
	}
	return nullptr;
}

FUniqueNetIdEOSPtr FUserManagerEOS::GetLocalUniqueNetIdEOS(EOS_EpicAccountId AccountId) const
{
	if (AccountIdToUserNumMap.Contains(AccountId))
	{
		return GetLocalUniqueNetIdEOS(AccountIdToUserNumMap[AccountId]);
	}
	return nullptr;
}

EOS_EpicAccountId FUserManagerEOS::GetLocalEpicAccountId(int32 LocalUserNum) const
{
	if (UserNumToAccountIdMap.Contains(LocalUserNum))
	{
		return UserNumToAccountIdMap[LocalUserNum];
	}
	return nullptr;
}

EOS_EpicAccountId FUserManagerEOS::GetLocalEpicAccountId() const
{
	return GetLocalEpicAccountId(DefaultLocalUser);
}

EOS_ProductUserId FUserManagerEOS::GetLocalProductUserId(int32 LocalUserNum) const
{
	if (UserNumToProductUserIdMap.Contains(LocalUserNum))
	{
		return UserNumToProductUserIdMap[LocalUserNum];
	}
	return nullptr;
}

EOS_ProductUserId FUserManagerEOS::GetLocalProductUserId() const
{
	return GetLocalProductUserId(DefaultLocalUser);
}

EOS_EpicAccountId FUserManagerEOS::GetLocalEpicAccountId(EOS_ProductUserId UserId) const
{
	if (ProductUserIdToUserNumMap.Contains(UserId))
	{
		return GetLocalEpicAccountId(ProductUserIdToUserNumMap[UserId]);
	}
	return nullptr;
}

EOS_ProductUserId FUserManagerEOS::GetLocalProductUserId(EOS_EpicAccountId AccountId) const
{
	if (AccountIdToUserNumMap.Contains(AccountId))
	{
		return GetLocalProductUserId(AccountIdToUserNumMap[AccountId]);
	}
	return nullptr;
}

#if ENGINE_MAJOR_VERSION == 5
		typedef TEOSCallback<EOS_Connect_OnQueryProductUserIdMappingsCallback, EOS_Connect_QueryProductUserIdMappingsCallbackInfo, FUserManagerEOS> FConnectQueryProductUserIdMappingsCallback;
#else
typedef TEOSCallback<EOS_Connect_OnQueryProductUserIdMappingsCallback, EOS_Connect_QueryProductUserIdMappingsCallbackInfo> FConnectQueryProductUserIdMappingsCallback;
#endif
/**
 * Uses the Connect API to retrieve the EOS_EpicAccountId for a given EOS_ProductUserId
 *
 * @param ProductUserId the product user id we want to query
 * @Param OutEpicAccountId the epic account id we will assign if the query is successful
 *
 * @return true if the operation was successful, false otherwise
 */
bool FUserManagerEOS::GetEpicAccountIdFromProductUserId(const EOS_ProductUserId& ProductUserId, EOS_EpicAccountId& OutEpicAccountId) const
{
	bool bResult = false;

	char EpicIdStr[EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH+1];
	int32 EpicIdStrSize = sizeof(EpicIdStr);

	EOS_Connect_GetProductUserIdMappingOptions Options = { };
	Options.ApiVersion = EOS_CONNECT_GETPRODUCTUSERIDMAPPING_API_LATEST;
	Options.AccountIdType = EOS_EExternalAccountType::EOS_EAT_EPIC;
	Options.LocalUserId = GetLocalProductUserId();
	Options.TargetProductUserId = ProductUserId;

	EOS_EResult Result = EOS_Connect_GetProductUserIdMapping(EOSSubsystem->ConnectHandle, &Options, EpicIdStr, &EpicIdStrSize);
	if (Result == EOS_EResult::EOS_Success)
	{
		OutEpicAccountId = EOS_EpicAccountId_FromString(EpicIdStr);
		bResult = true;
	}
	else
	{
		UE_LOG_ONLINE(Verbose, TEXT("[FUserManagerEOS::GetEpicAccountIdFromProductUserId] EOS_Connect_GetProductUserIdMapping not successful for ProductUserId (%s). Finished with EOS_EResult %s"), *EIK_LexToString(ProductUserId), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
	}

	return bResult;
}

void FUserManagerEOS::ResolveUniqueNetId(const EOS_ProductUserId& ProductUserId, const FResolveUniqueNetIdCallback& Callback) const
{
	TArray<EOS_ProductUserId> ProductUserIds = { const_cast<EOS_ProductUserId>(ProductUserId) };

	FResolveUniqueNetIdsCallback GroupCallback = [ProductUserId, OriginalCallback = Callback](TMap<EOS_ProductUserId, FUniqueNetIdEOSRef> ResolvedUniqueNetIds) {
		OriginalCallback(ResolvedUniqueNetIds[ProductUserId]);
	};

	ResolveUniqueNetIds(ProductUserIds, GroupCallback);
}

void FUserManagerEOS::ResolveUniqueNetIds(const TArray<EOS_ProductUserId>& ProductUserIds, const FResolveUniqueNetIdsCallback& Callback) const
{
	TMap<EOS_ProductUserId, FUniqueNetIdEOSRef> ResolvedUniqueNetIds;
	TArray<EOS_ProductUserId> ProductUserIdsToResolve;

	for (const EOS_ProductUserId& ProductUserId : ProductUserIds)
	{
		EOS_EpicAccountId EpicAccountId;
		// We check first if the Product User Id has already been queried, which would allow us to retrieve its Epic Account Id directly
		if (GetEpicAccountIdFromProductUserId(ProductUserId, EpicAccountId))
		{
			const FUniqueNetIdEOSRef UniqueNetId = FUniqueNetIdEOSRegistry::FindOrAdd(EpicAccountId, ProductUserId).ToSharedRef();
			ResolvedUniqueNetIds.Add(ProductUserId, UniqueNetId);
		}
		else
		{
			// If that's not the case, we'll have to query them first
			ProductUserIdsToResolve.Add(ProductUserId);
		}
	}
	

	if (ProductUserIdsToResolve.Num() > 0)
	{
		EOS_Connect_QueryProductUserIdMappingsOptions QueryProductUserIdMappingsOptions = {};
		QueryProductUserIdMappingsOptions.ApiVersion = EOS_CONNECT_QUERYPRODUCTUSERIDMAPPINGS_API_LATEST;
		QueryProductUserIdMappingsOptions.LocalUserId = EOSSubsystem->UserManager->GetLocalProductUserId(0);
		QueryProductUserIdMappingsOptions.ProductUserIds = ProductUserIdsToResolve.GetData();
		QueryProductUserIdMappingsOptions.ProductUserIdCount = ProductUserIdsToResolve.Num();
#if ENGINE_MAJOR_VERSION == 5
		FConnectQueryProductUserIdMappingsCallback* CallbackObj = new FConnectQueryProductUserIdMappingsCallback(FUserManagerEOSConstWeakPtr(AsShared()));
#else
		FConnectQueryProductUserIdMappingsCallback* CallbackObj = new FConnectQueryProductUserIdMappingsCallback();
#endif
		CallbackObj->CallbackLambda = [this, ProductUserIdsToResolve, ResolvedUniqueNetIds = MoveTemp(ResolvedUniqueNetIds), Callback](const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* Data) mutable
		{
			if (Data->ResultCode != EOS_EResult::EOS_Success)
			{
				UE_LOG_ONLINE(Verbose, TEXT("[FUserManagerEOS::ResolveUniqueNetIds] EOS_Connect_QueryProductUserIdMappings not successful for user (%s). Finished with EOS_EResult %s."), *EIK_LexToString(Data->LocalUserId), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			}

			for (const EOS_ProductUserId& ProductUserId : ProductUserIdsToResolve)
			{
				if(ProductUserId == nullptr)
				{
					continue;
				}
				EOS_EpicAccountId EpicAccountId = nullptr;

				GetEpicAccountIdFromProductUserId(ProductUserId, EpicAccountId);

				const FUniqueNetIdEOSRef UniqueNetId = FUniqueNetIdEOSRegistry::FindOrAdd(EpicAccountId, ProductUserId).ToSharedRef();

				ResolvedUniqueNetIds.Add(ProductUserId, UniqueNetId);
			}

			Callback(ResolvedUniqueNetIds);
		};
		
		EOS_Connect_QueryProductUserIdMappings(EOSSubsystem->ConnectHandle, &QueryProductUserIdMappingsOptions, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	else
	{
		Callback(ResolvedUniqueNetIds);
	}
}

FOnlineUserPtr FUserManagerEOS::GetLocalOnlineUser(int32 LocalUserNum) const
{
	FOnlineUserPtr OnlineUser;
	if (UserNumToNetIdMap.Contains(LocalUserNum))
	{
		const FUniqueNetIdEOSPtr NetId = UserNumToNetIdMap.FindRef(LocalUserNum);
		if (NetIdStringToOnlineUserMap.Contains(NetId->ToString()))
		{
			OnlineUser = NetIdStringToOnlineUserMap.FindRef(NetId->ToString());
		}
	}
	return OnlineUser;
}

FOnlineUserPtr FUserManagerEOS::GetOnlineUser(EOS_ProductUserId UserId) const
{
	FOnlineUserPtr OnlineUser;
	if (ProductUserIdToStringMap.Contains(UserId))
	{
		const FString& NetId = ProductUserIdToStringMap.FindRef(UserId);
		if (NetIdStringToOnlineUserMap.Contains(NetId))
		{
			OnlineUser = NetIdStringToOnlineUserMap.FindRef(*NetId);
		}
	}
	return OnlineUser;
}

FOnlineUserPtr FUserManagerEOS::GetOnlineUser(EOS_EpicAccountId AccountId) const
{
	FOnlineUserPtr OnlineUser;
	if (AccountIdToStringMap.Contains(AccountId))
	{
		const FString& NetId = AccountIdToStringMap.FindRef(AccountId);
		if (NetIdStringToOnlineUserMap.Contains(NetId))
		{
			OnlineUser = NetIdStringToOnlineUserMap.FindRef(NetId);
		}
	}
	return OnlineUser;
}

void FUserManagerEOS::RemoveLocalUser(int32 LocalUserNum)
{
	const FUniqueNetIdEOSPtr* FoundId = UserNumToNetIdMap.Find(LocalUserNum);
	if (FoundId != nullptr)
	{
		EOSSubsystem->ReleaseVoiceChatUserInterface(**FoundId);
		LocalUserNumToFriendsListMap.Remove(LocalUserNum);
		const FString& NetId = (*FoundId)->ToString();
		const EOS_EpicAccountId AccountId = (*FoundId)->GetEpicAccountId();
		AccountIdToStringMap.Remove(AccountId);
		AccountIdToUserNumMap.Remove(AccountId);
		NetIdStringToOnlineUserMap.Remove(NetId);
		StringToUserAccountMap.Remove(NetId);
		UserNumToNetIdMap.Remove(LocalUserNum);
		UserNumToAccountIdMap.Remove(LocalUserNum);
		EOS_ProductUserId UserId = UserNumToProductUserIdMap[LocalUserNum];
		ProductUserIdToUserNumMap.Remove(UserId);
		ProductUserIdToStringMap.Remove(UserId);
		UserNumToProductUserIdMap.Remove(LocalUserNum);
	}
	// Reset this for the next user login
	if (LocalUserNum == DefaultLocalUser)
	{
		DefaultLocalUser = -1;
	}
}

FUniqueNetIdPtr FUserManagerEOS::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	// If we're passed an EOSPlus id, the first EOS_ID_BYTE_SIZE bytes are the EAS|EOS part we care about.
	Size = FMath::Min(Size, EOS_ID_BYTE_SIZE);
	return FUniqueNetIdEOSRegistry::FindOrAdd(Bytes, Size);
}

FUniqueNetIdPtr FUserManagerEOS::CreateUniquePlayerId(const FString& InStr)
{
	FString NetIdStr = InStr;
	// If we're passed an EOSPlus id, remove the platform id and separator.
	NetIdStr.Split(EOSPLUS_ID_SEPARATOR, nullptr, &NetIdStr);
	return FUniqueNetIdEOSRegistry::FindOrAdd(NetIdStr);
}

ELoginStatus::Type FUserManagerEOS::GetLoginStatus(int32 LocalUserNum) const
{
	FUniqueNetIdEOSPtr UserId = GetLocalUniqueNetIdEOS(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetLoginStatus(*UserId);
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FUserManagerEOS::GetLoginStatus(const FUniqueNetIdEOS& UserId) const
{
	FEOSSettings Settings = UEIKSettings::GetSettings();
	// If the user isn't using EAS, then only check for a product user id
	if (!Settings.bUseEAS)
	{
		const EOS_ProductUserId ProductUserId = UserId.GetProductUserId();
		if (ProductUserId != nullptr)
		{
			return ELoginStatus::LoggedIn;
		}
		return ELoginStatus::NotLoggedIn;
	}

	const EOS_EpicAccountId AccountId = UserId.GetEpicAccountId();
	if (AccountId == nullptr)
	{
		return ELoginStatus::NotLoggedIn;
	}

	EOS_ELoginStatus LoginStatus = EOS_Auth_GetLoginStatus(EOSSubsystem->AuthHandle, AccountId);
	switch (LoginStatus)
	{
		case EOS_ELoginStatus::EOS_LS_LoggedIn:
		{
			return ELoginStatus::LoggedIn;
		}
		case EOS_ELoginStatus::EOS_LS_UsingLocalProfile:
		{
			return ELoginStatus::UsingLocalProfile;
		}
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FUserManagerEOS::GetLoginStatus(const FUniqueNetId& UserId) const
{
	const FUniqueNetIdEOS& EosId = FUniqueNetIdEOS::Cast(UserId);
	return GetLoginStatus(EosId);
}

FString FUserManagerEOS::GetPlayerNickname(int32 LocalUserNum) const
{
	FUniqueNetIdEOSPtr UserId = GetLocalUniqueNetIdEOS(LocalUserNum);
	if (UserId.IsValid())
	{
		TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(*UserId);
		if (UserAccount.IsValid())
		{
			return UserAccount->GetDisplayName();
		}
	}
	return FString();
}

FString FUserManagerEOS::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid())
	{
		return UserAccount->GetDisplayName();
	}
	return FString();
}

FString FUserManagerEOS::GetAuthToken(int32 LocalUserNum) const
{
	FUniqueNetIdPtr UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(*UserId);
		if (UserAccount.IsValid())
		{
			return UserAccount->GetAccessToken();
		}
	}
	return FString();
}
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
void FUserManagerEOS::GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege,
	const FOnGetUserPrivilegeCompleteDelegate& Delegate, EShowPrivilegeResolveUI ShowResolveUI)
{
	Delegate.ExecuteIfBound(LocalUserId, Privilege, static_cast<unsigned>(EPrivilegeResults::NoFailures));
}
#else
void FUserManagerEOS::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege,
	const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(UserId, Privilege, static_cast<unsigned>(EPrivilegeResults::NoFailures));
}
#endif

void FUserManagerEOS::RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(LocalUserId, FOnlineError(EOnlineErrorResult::NotImplemented));
}

FPlatformUserId FUserManagerEOS::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const
{
#if ENGINE_MAJOR_VERSION == 5
	return GetPlatformUserIdFromLocalUserNum(GetLocalUserNumFromUniqueNetId(UniqueNetId));
#else
	return GetLocalUserNumFromUniqueNetId(UniqueNetId);
#endif
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
void FUserManagerEOS::GetLinkedAccountAuthToken(int32 LocalUserNum, const FString& TokenType, const FOnGetLinkedAccountAuthTokenCompleteDelegate& Delegate) const
#else
void FUserManagerEOS::GetLinkedAccountAuthToken(int32 LocalUserNum, const FOnGetLinkedAccountAuthTokenCompleteDelegate& Delegate) const
#endif
{
	FExternalAuthToken ExternalToken;
	ExternalToken.TokenString = GetAuthToken(LocalUserNum);
	Delegate.ExecuteIfBound(LocalUserNum, ExternalToken.IsValid(), ExternalToken);
}

int32 FUserManagerEOS::GetLocalUserNumFromPlatformUserId(FPlatformUserId PlatformUserId) const
{
	return 0;
}

FString FUserManagerEOS::GetAuthType() const
{
	return TEXT("epic");
}

// IOnlineExternalUI Interface

bool FUserManagerEOS::ShowLoginUI(const int ControllerIndex, bool bShowOnlineOnly, bool bShowSkipButton, const FOnLoginUIClosedDelegate& Delegate)
{
	FPlatformEOSHelpersPtr EOSHelpers = EOSSubsystem->GetEOSHelpers();
	EOSHelpers->PlatformTriggerLoginUI(EOSSubsystem, ControllerIndex, bShowOnlineOnly, bShowSkipButton, Delegate);

	return true;
}

bool FUserManagerEOS::ShowAccountCreationUI(const int ControllerIndex, const FOnAccountCreationUIClosedDelegate& Delegate)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowAccountCreationUI] This method is not implemented."));
	
	EOSSubsystem->ExecuteNextTick([ControllerIndex, Delegate]()
		{
			Delegate.ExecuteIfBound(ControllerIndex, FOnlineAccountCredentials(), FOnlineError(EOnlineErrorResult::NotImplemented));
		});

	return true;
}

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_UI_OnShowFriendsCallback, EOS_UI_ShowFriendsCallbackInfo, FUserManagerEOS> FOnShowFriendsCallback;
#else
typedef TEOSCallback<EOS_UI_OnShowFriendsCallback, EOS_UI_ShowFriendsCallbackInfo> FOnShowFriendsCallback;
#endif

bool FUserManagerEOS::ShowFriendsUI(int32 LocalUserNum)
{
	EOS_UI_ShowFriendsOptions Options = {};
	Options.ApiVersion = EOS_UI_SHOWFRIENDS_API_LATEST;
	Options.LocalUserId = GetLocalEpicAccountId(LocalUserNum);
#if ENGINE_MAJOR_VERSION == 5
	FOnShowFriendsCallback* CallbackObj = new FOnShowFriendsCallback(AsWeak());
#else
	FOnShowFriendsCallback* CallbackObj = new FOnShowFriendsCallback();
#endif
	CallbackObj->CallbackLambda = [](const EOS_UI_ShowFriendsCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			UE_LOG_ONLINE_EXTERNALUI(VeryVerbose, TEXT("[FUserManagerEOS::ShowFriendsUI] EOS_UI_ShowFriends was successful."));
		}
		else
		{
			UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowFriendsUI] EOS_UI_ShowFriends was not successful. Finished with error %s"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
	};

	EOS_UI_ShowFriends(EOSSubsystem->UIHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return true;
}


bool FUserManagerEOS::ShowInviteUI(int32 LocalUserNum, FName SessionName)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowInviteUI] This method is not implemented."));

	return false;
}

bool FUserManagerEOS::ShowAchievementsUI(int32 LocalUserNum)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowAchievementsUI] This method is not implemented."));

	return false;
}

bool FUserManagerEOS::ShowLeaderboardUI(const FString& LeaderboardName)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowLeaderboardUI] This method is not implemented."));

	return false;
}

bool FUserManagerEOS::ShowWebURL(const FString& Url, const FShowWebUrlParams& ShowParams, const FOnShowWebUrlClosedDelegate& Delegate)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowWebURL] This method is not implemented."));

	if (Delegate.IsBound())
	{
		EOSSubsystem->ExecuteNextTick([Delegate]()
			{
				Delegate.ExecuteIfBound(FString());
			});
		return true;
	}
	return false;
}

bool FUserManagerEOS::CloseWebURL()
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::CloseWebURL] This method is not implemented."));

	return false;
}

bool FUserManagerEOS::ShowProfileUI(const FUniqueNetId& Requestor, const FUniqueNetId& Requestee, const FOnProfileUIClosedDelegate& Delegate)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowProfileUI] This method is not implemented."));

	if (Delegate.IsBound())
	{
		EOSSubsystem->ExecuteNextTick([Delegate]()
			{
				Delegate.ExecuteIfBound();
			});
		return true;
	}
	return false;
}

bool FUserManagerEOS::ShowAccountUpgradeUI(const FUniqueNetId& UniqueId)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowAccountUpgradeUI] This method is not implemented."));

	return false;
}

bool FUserManagerEOS::ShowStoreUI(int32 LocalUserNum, const FShowStoreParams& ShowParams, const FOnShowStoreUIClosedDelegate& Delegate)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowStoreUI] This method is not implemented."));

	if (Delegate.IsBound())
	{
		EOSSubsystem->ExecuteNextTick([Delegate]()
			{
				Delegate.ExecuteIfBound(false);
			});
		return true;
	}
	return false;
}

bool FUserManagerEOS::ShowSendMessageUI(int32 LocalUserNum, const FShowSendMessageParams& ShowParams, const FOnShowSendMessageUIClosedDelegate& Delegate)
{
	UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("[FUserManagerEOS::ShowSendMessageUI] This method is not implemented."));

	if (Delegate.IsBound())
	{
		EOSSubsystem->ExecuteNextTick([Delegate]()
			{
				Delegate.ExecuteIfBound(false);
			});
		return true;
	}
	return false;
}

// ~IOnlineExternalUI Interface

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Friends_OnQueryFriendsCallback, EOS_Friends_QueryFriendsCallbackInfo, FUserManagerEOS> FReadFriendsCallback;
#else
typedef TEOSCallback<EOS_Friends_OnQueryFriendsCallback, EOS_Friends_QueryFriendsCallbackInfo> FReadFriendsCallback;
#endif

void FUserManagerEOS::FriendStatusChanged(const EOS_Friends_OnFriendsUpdateInfo* Data)
{
	// This seems to happen due to the SDK's local cache going from empty to filled, so ignore it
	// It's not really a valid transition since there should have been a pending invite inbetween
	if (Data->PreviousStatus == EOS_EFriendsStatus::EOS_FS_NotFriends && Data->CurrentStatus == EOS_EFriendsStatus::EOS_FS_Friends)
	{
		return;
	}

	// Get the local user information
	if (AccountIdToUserNumMap.Contains(Data->LocalUserId))
	{
		int32 LocalUserNum = AccountIdToUserNumMap[Data->LocalUserId];
		FUniqueNetIdEOSPtr LocalEOSID = UserNumToNetIdMap[LocalUserNum];
		// If we don't know them yet, then add them to kick off the reads
		if (!AccountIdToStringMap.Contains(Data->TargetUserId))
		{
			AddFriend(LocalUserNum, Data->TargetUserId);
		}
		// They are in our list now
		FOnlineUserPtr OnlineUser = EpicAccountIdToOnlineUserMap[Data->TargetUserId];
		FOnlineFriendEOSPtr Friend = LocalUserNumToFriendsListMap[LocalUserNum]->GetByNetIdString(AccountIdToStringMap[Data->TargetUserId]);
		// Figure out which notification to fire
		if (Data->CurrentStatus == EOS_EFriendsStatus::EOS_FS_Friends)
		{
			Friend->SetInviteStatus(EInviteStatus::Accepted);
			TriggerOnInviteAcceptedDelegates(*LocalEOSID, *OnlineUser->GetUserId());
		}
		else if (Data->PreviousStatus == EOS_EFriendsStatus::EOS_FS_Friends && Data->CurrentStatus == EOS_EFriendsStatus::EOS_FS_NotFriends)
		{
			LocalUserNumToFriendsListMap[LocalUserNum]->Remove(AccountIdToStringMap[Data->TargetUserId], Friend.ToSharedRef());
			Friend->SetInviteStatus(EInviteStatus::Unknown);
			TriggerOnFriendRemovedDelegates(*LocalEOSID, *OnlineUser->GetUserId());
		}
		else if (Data->PreviousStatus < EOS_EFriendsStatus::EOS_FS_Friends && Data->CurrentStatus == EOS_EFriendsStatus::EOS_FS_NotFriends)
		{
			if (!Friend.IsValid())
			{
				return; // Early return to avoid crash
			}
			if (!AccountIdToStringMap.Contains(Data->TargetUserId))
			{
				return;
			}
			if (!LocalUserNumToFriendsListMap.Contains(LocalUserNum))
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid LocalUserNum or FriendsListMap entry for LocalUserNum: %d"), LocalUserNum);
				return;
			}
			LocalUserNumToFriendsListMap[LocalUserNum]->Remove(AccountIdToStringMap[Data->TargetUserId], Friend.ToSharedRef());
			Friend->SetInviteStatus(EInviteStatus::Unknown);
			TriggerOnInviteRejectedDelegates(*LocalEOSID, *OnlineUser->GetUserId());
		}
		else if (Data->CurrentStatus == EOS_EFriendsStatus::EOS_FS_InviteReceived)
		{
			Friend->SetInviteStatus(EInviteStatus::PendingInbound);
			TriggerOnInviteReceivedDelegates(*LocalEOSID, *OnlineUser->GetUserId());
		}
		TriggerOnFriendsChangeDelegates(LocalUserNum);
	}
}

void FUserManagerEOS::AddFriend(int32 LocalUserNum, EOS_EpicAccountId EpicAccountId)
{
	FUniqueNetIdEOSRef FriendNetId = FUniqueNetIdEOSRegistry::FindOrAdd(EpicAccountId, nullptr).ToSharedRef();
	const FString NetId = FriendNetId->ToString();
	FOnlineFriendEOSRef FriendRef = MakeShareable(new FOnlineFriendEOS(FriendNetId));
	LocalUserNumToFriendsListMap[LocalUserNum]->Add(NetId, FriendRef);

	EOS_Friends_GetStatusOptions Options = { };
	Options.ApiVersion = EOS_FRIENDS_GETSTATUS_API_LATEST;
	Options.LocalUserId = UserNumToAccountIdMap[LocalUserNum];
	Options.TargetUserId = EpicAccountId;
	EOS_EFriendsStatus Status = EOS_Friends_GetStatus(EOSSubsystem->FriendsHandle, &Options);
	
	FriendRef->SetInviteStatus(ToEInviteStatus(Status));

	// Add this friend as a remote player (this will grab user info)
	AddRemotePlayer(LocalUserNum, NetId, EpicAccountId, FriendNetId, FriendRef, FriendRef);

	// Querying the presence of a non-friend would cause an SDK error.
	// Players that sent/recieved a friend invitation from us still count as "friends", so check
	// our friend relationship here.
	if(Status == EOS_EFriendsStatus::EOS_FS_Friends)
	{
		QueryPresence(*FriendNetId, IgnoredPresenceDelegate);
	}
}

void FUserManagerEOS::AddRemotePlayer(int32 LocalUserNum, const FString& NetId, EOS_EpicAccountId EpicAccountId)
{
	FUniqueNetIdEOSRef EOSID = FUniqueNetIdEOSRegistry::FindOrAdd(NetId).ToSharedRef();
	FOnlineUserEOSRef UserRef = MakeShareable(new FOnlineUserEOS(EOSID));
	// Add this user as a remote (this will grab presence & user info)
	AddRemotePlayer(LocalUserNum, NetId, EpicAccountId, EOSID, UserRef, UserRef);
}

void FUserManagerEOS::AddRemotePlayer(int32 LocalUserNum, const FString& NetId, EOS_EpicAccountId EpicAccountId, FUniqueNetIdEOSPtr UniqueNetId, FOnlineUserPtr OnlineUser, IAttributeAccessInterfaceRef AttributeRef)
{
	NetIdStringToOnlineUserMap.Emplace(NetId, OnlineUser);
	EpicAccountIdToOnlineUserMap.Emplace(EpicAccountId, OnlineUser);
	NetIdStringToAttributeAccessMap.Emplace(NetId, AttributeRef);
	EpicAccountIdToAttributeAccessMap.Emplace(EpicAccountId, AttributeRef);

	AccountIdToStringMap.Emplace(EpicAccountId, NetId);

	// Read the user info for this player
	ReadUserInfo(LocalUserNum, EpicAccountId);
}

void FUserManagerEOS::UpdateRemotePlayerProductUserId(EOS_EpicAccountId EpicAccountId, EOS_ProductUserId ProductUserId)
{
	// See if the net ids have changed for this user and bail if they are the same
	const FString PrevNetIdStr = AccountIdToStringMap[EpicAccountId];
	// Note that calling FindOrAdd with previously invalid EAS/EOS components will update the net id in place.
	const FString NewNetIdStr = FUniqueNetIdEOSRegistry::FindOrAdd(EpicAccountId, ProductUserId)->ToString();
	if (PrevNetIdStr == NewNetIdStr)
	{
		// No change, so skip any work
		return;
	}

	const FString ProductUserIdStr = EIK_LexToString(ProductUserId);

	// Get the unique net id and rebuild the string for it
	IAttributeAccessInterfaceRef AttrAccess = NetIdStringToAttributeAccessMap[PrevNetIdStr];
	FUniqueNetIdEOSPtr NetIdEOS = AttrAccess->GetUniqueNetIdEOS();
	// Update any old friends entries with the new net id key
	for (TMap<int32, FFriendsListEOSRef>::TConstIterator It(LocalUserNumToFriendsListMap); It; ++It)
	{
		FFriendsListEOSRef FriendsList = It.Value();
		FOnlineFriendEOSPtr FoundFriend = FriendsList->GetByNetIdString(PrevNetIdStr);
		if (FoundFriend.IsValid())
		{
			FriendsList->UpdateNetIdStr(PrevNetIdStr, NewNetIdStr);
		}
	}
	// Update all of the other net id to X mappings
	AccountIdToStringMap.Remove(EpicAccountId);
	AccountIdToStringMap.Emplace(EpicAccountId, NewNetIdStr);
	ProductUserIdToStringMap.Remove(ProductUserId);
	ProductUserIdToStringMap.Emplace(ProductUserId, *NewNetIdStr);
	FOnlineUserPtr OnlineUser = NetIdStringToOnlineUserMap[PrevNetIdStr];
	NetIdStringToOnlineUserMap.Remove(PrevNetIdStr);
	NetIdStringToOnlineUserMap.Emplace(NewNetIdStr, OnlineUser);
	NetIdStringToAttributeAccessMap.Remove(PrevNetIdStr);
	NetIdStringToAttributeAccessMap.Emplace(NewNetIdStr, AttrAccess);
	// Presence may not be available for all online users
	if (NetIdStringToOnlineUserPresenceMap.Contains(PrevNetIdStr))
	{
		FOnlineUserPresenceRef UserPresence = NetIdStringToOnlineUserPresenceMap[PrevNetIdStr];
		NetIdStringToOnlineUserPresenceMap.Remove(PrevNetIdStr);
		NetIdStringToOnlineUserPresenceMap.Emplace(NewNetIdStr, UserPresence);
	}
}

// IOnlineFriends Interface

bool FUserManagerEOS::ReadFriendsList(int32 LocalUserNum, const FString& ListName, const FOnReadFriendsListComplete& Delegate)
{
	if (!UserNumToNetIdMap.Contains(LocalUserNum))
	{
		const FString ErrorStr = FString::Printf(TEXT("Can't ReadFriendsList() for user (%d) since they are not logged in"), LocalUserNum);
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("%s"), *ErrorStr);
		Delegate.ExecuteIfBound(LocalUserNum, false, ListName, ErrorStr);
		return false;
	}

	// We save the information for this call even if it won't be automatically processed
	const bool bIsReadFriendsListOngoing = CachedReadUserListInfoForLocalUserMap.Contains(LocalUserNum);
	if (!bIsReadFriendsListOngoing)
	{
		CachedReadUserListInfoForLocalUserMap.Emplace(LocalUserNum);
	}
	CachedReadUserListInfoForLocalUserMap[LocalUserNum].Add(ReadUserListInfo(LocalUserNum, ListName, Delegate));

	if (bIsReadFriendsListOngoing)
	{
		UE_LOG_ONLINE_FRIEND(Verbose, TEXT("A ReadFriendsList() operation for user (%d) is already running, we'll save its information and launch it automatically later."), LocalUserNum);
		return true;
	}

	EOS_Friends_QueryFriendsOptions Options = { };
	Options.ApiVersion = EOS_FRIENDS_QUERYFRIENDS_API_LATEST;
	Options.LocalUserId = UserNumToAccountIdMap[LocalUserNum];

#if ENGINE_MAJOR_VERSION == 5
	FReadFriendsCallback* CallbackObj = new FReadFriendsCallback(AsWeak());
#else
	FReadFriendsCallback* CallbackObj = new FReadFriendsCallback();
#endif
	CallbackObj->CallbackLambda = [this, LocalUserNum, ListName, Delegate](const EOS_Friends_QueryFriendsCallbackInfo* Data)
	{
		EOS_EResult Result = Data->ResultCode;
		if (GetLoginStatus(LocalUserNum) != ELoginStatus::LoggedIn)
		{
			// Handle the user logging out while a read is in progress
			Result = EOS_EResult::EOS_InvalidUser;
		}

		bool bWasSuccessful = Result == EOS_EResult::EOS_Success;
		if (bWasSuccessful)
		{
			EOS_Friends_GetFriendsCountOptions Options = { };
			Options.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
			Options.LocalUserId = UserNumToAccountIdMap[LocalUserNum];
			int32 FriendCount = EOS_Friends_GetFriendsCount(EOSSubsystem->FriendsHandle, &Options);

			LocalUserNumToFriendsListMap[LocalUserNum]->Empty(FriendCount);

			TArray<FString> FriendEasIds;
			FriendEasIds.Reserve(FriendCount);
			// Process each friend returned
			for (int32 Index = 0; Index < FriendCount; Index++)
			{
				EOS_Friends_GetFriendAtIndexOptions FriendIndexOptions = { };
				FriendIndexOptions.ApiVersion = EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST;
				FriendIndexOptions.Index = Index;
				FriendIndexOptions.LocalUserId = Options.LocalUserId;
				EOS_EpicAccountId FriendEpicAccountId = EOS_Friends_GetFriendAtIndex(EOSSubsystem->FriendsHandle, &FriendIndexOptions);
				if (FriendEpicAccountId != nullptr)
				{
					AddFriend(LocalUserNum, FriendEpicAccountId);
					FriendEasIds.Add(EIK_LexToString(FriendEpicAccountId));
				}
			}

			const TFunction<FOnQueryExternalIdMappingsComplete::TFuncType>& OnExternalIdMappingsQueriedLambda =
#if ENGINE_MAJOR_VERSION == 5
				[this, WeakThis = AsWeak(), LocalUserNum](bool bWasSuccessful, const FUniqueNetId& UserId, const FExternalIdQueryOptions& QueryOptions, const TArray<FString>& ExternalIds, const FString& Error)
{
if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
#else
				[this, LocalUserNum](bool bWasSuccessful, const FUniqueNetId& UserId, const FExternalIdQueryOptions& QueryOptions, const TArray<FString>& ExternalIds, const FString& Error)
			{
				if (FUserManagerEOSPtr StrongThis = AsShared())
#endif
				{
					ProcessReadFriendsListComplete(LocalUserNum, bWasSuccessful, Error);
				}
			};

			const auto& ExternalMappingsCallback = OSSInternalCallback::Create<FOnQueryExternalIdMappingsComplete>(EOSSubsystem->UserManager, OnExternalIdMappingsQueriedLambda);

			QueryExternalIdMappings(*GetLocalUniqueNetIdEOS(DefaultLocalUser), FExternalIdQueryOptions(), FriendEasIds, ExternalMappingsCallback);

		}
		else
		{
			const FString ErrorString = FString::Printf(TEXT("ReadFriendsList(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
			ProcessReadFriendsListComplete(LocalUserNum, false, ErrorString);
		}
	};
	EOS_Friends_QueryFriends(EOSSubsystem->FriendsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return true;
}

void FUserManagerEOS::ProcessReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ErrorStr)
{
	// If we started any user info queries for friends, we'll just wait until they finish
	if (!IsFriendQueryUserInfoOngoing(LocalUserNum))
	{
		// If not, we'll just trigger the delegates for all cached calls
		TArray<ReadUserListInfo> CachedInfoList;
		if (CachedReadUserListInfoForLocalUserMap.RemoveAndCopyValue(LocalUserNum, CachedInfoList))
		{
			for (const ReadUserListInfo& CachedInfo : CachedInfoList)
			{
				CachedInfo.ExecuteDelegateIfBound(bWasSuccessful, ErrorStr);
			}
		}

		TriggerOnFriendsChangeDelegates(LocalUserNum);

		IsFriendQueryUserInfoOngoingForLocalUserMap.Remove(LocalUserNum);
		IsPlayerQueryExternalMappingsOngoingForLocalUserMap.Remove(LocalUserNum);
	}
}

void FUserManagerEOS::SetFriendAlias(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName, const FString& Alias, const FOnSetFriendAliasComplete& Delegate)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::SetFriendAlias] This method is not supported."));

	EOSSubsystem->ExecuteNextTick([LocalUserNum, FriendId = FriendId.AsShared(), ListName, Delegate]()
		{
			Delegate.ExecuteIfBound(LocalUserNum, *FriendId, ListName, FOnlineError(EOnlineErrorResult::NotImplemented));
		});
}

void FUserManagerEOS::DeleteFriendAlias(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName, const FOnDeleteFriendAliasComplete& Delegate)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::DeleteFriendAlias] This method is not supported."));

	EOSSubsystem->ExecuteNextTick([LocalUserNum, FriendId = FriendId.AsShared(), ListName, Delegate]()
		{
			Delegate.ExecuteIfBound(LocalUserNum, *FriendId, ListName, FOnlineError(EOnlineErrorResult::NotImplemented));
		});
}

bool FUserManagerEOS::DeleteFriendsList(int32 LocalUserNum, const FString& ListName, const FOnDeleteFriendsListComplete& Delegate)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::DeleteFriendsList] This method is not supported."));

	EOSSubsystem->ExecuteNextTick([LocalUserNum, ListName, Delegate]()
		{
			Delegate.ExecuteIfBound(LocalUserNum, false, ListName, TEXT("This method is not supported."));
		});

	return true;
}

#if ENGINE_MAJOR_VERSION == 5
	typedef TEOSCallback<EOS_Friends_OnSendInviteCallback, EOS_Friends_SendInviteCallbackInfo, FUserManagerEOS> FSendInviteCallback;
#else
typedef TEOSCallback<EOS_Friends_OnSendInviteCallback, EOS_Friends_SendInviteCallbackInfo> FSendInviteCallback;
#endif
	
bool FUserManagerEOS::SendInvite(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName, const FOnSendInviteComplete& Delegate)
{
	if (!UserNumToNetIdMap.Contains(LocalUserNum))
	{
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("Can't SendInvite() for user (%d) since they are not logged in"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FriendId, ListName, FString(TEXT("Can't SendInvite() for user (%d) since they are not logged in"), LocalUserNum));
		return false;
	}

	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(FriendId);
	const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_FALSE)
	{
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("Can't SendInvite() for user (%d) since the potential player id is unknown"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FriendId, ListName, FString(TEXT("Can't SendInvite() for user (%d) since the player id is unknown"), LocalUserNum));
		return false;
	}
#if ENGINE_MAJOR_VERSION == 5
	FSendInviteCallback* CallbackObj = new FSendInviteCallback(AsWeak());
#else
	FSendInviteCallback* CallbackObj = new FSendInviteCallback();
#endif
	CallbackObj->CallbackLambda = [LocalUserNum, ListName, this, Delegate](const EOS_Friends_SendInviteCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			const FString& NetId = AccountIdToStringMap[Data->TargetUserId];

			FString ErrorString;
			bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
			if (!bWasSuccessful)
			{
				ErrorString = FString::Printf(TEXT("Failed to send invite for user (%d) to player (%s) with result code (%s)"), LocalUserNum, *NetId, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			}
			Delegate.ExecuteIfBound(LocalUserNum, bWasSuccessful, *FUniqueNetIdEOSRegistry::FindOrAdd(NetId), ListName, ErrorString);
		}
	};

	EOS_Friends_SendInviteOptions Options = { };
	Options.ApiVersion = EOS_FRIENDS_SENDINVITE_API_LATEST;
	Options.LocalUserId = UserNumToAccountIdMap[LocalUserNum];
	Options.TargetUserId = AccountId;
	EOS_Friends_SendInvite(EOSSubsystem->FriendsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return true;
}

#if ENGINE_MAJOR_VERSION == 5
	typedef TEOSCallback<EOS_Friends_OnAcceptInviteCallback, EOS_Friends_AcceptInviteCallbackInfo, FUserManagerEOS> FAcceptInviteCallback;
#else
typedef TEOSCallback<EOS_Friends_OnAcceptInviteCallback, EOS_Friends_AcceptInviteCallbackInfo> FAcceptInviteCallback;
#endif
bool FUserManagerEOS::AcceptInvite(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName, const FOnAcceptInviteComplete& Delegate)
{
	if (!UserNumToNetIdMap.Contains(LocalUserNum))
	{
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("Can't AcceptInvite() for user (%d) since they are not logged in"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FriendId, ListName, FString(TEXT("Can't AcceptInvite() for user (%d) since they are not logged in"), LocalUserNum));
		return false;
	}

	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(FriendId);
	const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_FALSE)
	{
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("Can't AcceptInvite() for user (%d) since the friend is not in their list"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FriendId, ListName, FString(TEXT("Can't AcceptInvite() for user (%d) since the friend is not in their list"), LocalUserNum));
		return false;
	}
#if ENGINE_MAJOR_VERSION == 5
	FAcceptInviteCallback* CallbackObj = new FAcceptInviteCallback(AsWeak());
#else
	FAcceptInviteCallback* CallbackObj = new FAcceptInviteCallback();
#endif
	CallbackObj->CallbackLambda = [LocalUserNum, ListName, this, Delegate](const EOS_Friends_AcceptInviteCallbackInfo* Data)
	{
		const FString& NetId = AccountIdToStringMap[Data->TargetUserId];

		FString ErrorString;
		bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			ErrorString = FString::Printf(TEXT("Failed to accept invite for user (%d) from friend (%s) with result code (%s)"), LocalUserNum, *NetId, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		Delegate.ExecuteIfBound(LocalUserNum, bWasSuccessful, *FUniqueNetIdEOSRegistry::FindOrAdd(NetId), ListName, ErrorString);
	};

	EOS_Friends_AcceptInviteOptions Options = { };
	Options.ApiVersion = EOS_FRIENDS_ACCEPTINVITE_API_LATEST;
	Options.LocalUserId = UserNumToAccountIdMap[LocalUserNum];
	Options.TargetUserId = AccountId;
	EOS_Friends_AcceptInvite(EOSSubsystem->FriendsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	return true;
}

void EOS_CALL EOSRejectInviteCallback(const EOS_Friends_RejectInviteCallbackInfo* Data)
{
	// We don't need to notify anyone so ignore
}

bool FUserManagerEOS::RejectInvite(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName)
{
	if (!UserNumToNetIdMap.Contains(LocalUserNum))
	{
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("Can't RejectInvite() for user (%d) since they are not logged in"), LocalUserNum);
		return false;
	}

	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(FriendId);
	const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_FALSE)
	{
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("Can't RejectInvite() for user (%d) since the friend is not in their list"), LocalUserNum);
		return false;
	}

	EOS_Friends_RejectInviteOptions Options{ 0 };
	Options.ApiVersion = EOS_FRIENDS_REJECTINVITE_API_LATEST;
	Options.LocalUserId = UserNumToAccountIdMap[LocalUserNum];
	Options.TargetUserId = AccountId;
	EOS_Friends_RejectInvite(EOSSubsystem->FriendsHandle, &Options, nullptr, &EOSRejectInviteCallback);
	return true;
}

bool FUserManagerEOS::DeleteFriend(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::DeleteFriend] Friends may only be deleted via the Epic Games Launcher."));
#if ENGINE_MAJOR_VERSION == 5
	EOSSubsystem->ExecuteNextTick([this, WeakThis = AsWeak(), LocalUserNum, FriendId = FriendId.AsShared(), ListName]()
	{
		if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
#else
	EOSSubsystem->ExecuteNextTick([this, LocalUserNum, FriendId = FriendId.AsShared(), ListName]()
		{
			if (FUserManagerEOSPtr StrongThis = AsShared())
#endif
				{
				TriggerOnDeleteFriendCompleteDelegates(LocalUserNum, false, *FriendId, ListName, TEXT("[FUserManagerEOS::DeleteFriend] Friends may only be deleted via the Epic Games Launcher."));
			}
		});

	return true;
}

bool FUserManagerEOS::GetFriendsList(int32 LocalUserNum, const FString& ListName, TArray<TSharedRef<FOnlineFriend>>& OutFriends)
{
	OutFriends.Reset();
	if (LocalUserNumToFriendsListMap.Contains(LocalUserNum))
	{
		FFriendsListEOSRef FriendsList = LocalUserNumToFriendsListMap[LocalUserNum];
		for (FOnlineFriendEOSRef Friend : FriendsList->GetList())
		{
			const FOnlineUserPresence& Presence = Friend->GetPresence();
			// See if they only want online only
			if (ListName == EFriendsLists::ToString(EFriendsLists::OnlinePlayers) && !Presence.bIsOnline)
			{
				continue;
			}
			// Of if they only want friends playing this game
			else if (ListName == EFriendsLists::ToString(EFriendsLists::InGamePlayers) && !Presence.bIsPlayingThisGame)
			{
				continue;
			}
			// If the service hasn't returned the info yet, skip them
			else if (Friend->GetDisplayName().IsEmpty())
			{
				continue;
			}
			OutFriends.Add(Friend);
		}
		// Sort these by those playing the game first, alphabetically, then not playing, then not online
		OutFriends.Sort([](TSharedRef<FOnlineFriend> A, TSharedRef<FOnlineFriend> B)
		{
			const FOnlineUserPresence& APres = A->GetPresence();
			const FOnlineUserPresence& BPres = B->GetPresence();
			// If they are the same, then check playing this game
			if (APres.bIsOnline == BPres.bIsOnline)
			{
				// If they are the same, then sort by name
				if (APres.bIsPlayingThisGame == BPres.bIsPlayingThisGame)
				{
					const EInviteStatus::Type AFriendStatus = A->GetInviteStatus();
					const EInviteStatus::Type BFriendStatus = B->GetInviteStatus();
					// Sort pending friends below accepted friends
					if (AFriendStatus == BFriendStatus && AFriendStatus == EInviteStatus::Accepted)
					{
						const FString& AName = A->GetDisplayName();
						const FString& BName = B->GetDisplayName();
						return AName < BName;
					}
				}
			}
			return false;
		});
		return true;
	}
	return false;
}

TSharedPtr<FOnlineFriend> FUserManagerEOS::GetFriend(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName)
{
	if (LocalUserNumToFriendsListMap.Contains(LocalUserNum))
	{
		FFriendsListEOSRef FriendsList = LocalUserNumToFriendsListMap[LocalUserNum];
		const FUniqueNetIdEOS& EosId = FUniqueNetIdEOS::Cast(FriendId);
		FOnlineFriendEOSPtr FoundFriend = FriendsList->GetByNetIdString(EosId.ToString());
		if (FoundFriend.IsValid())
		{
			const FOnlineUserPresence& Presence = FoundFriend->GetPresence();
			// See if they only want online only
			if (ListName == EFriendsLists::ToString(EFriendsLists::OnlinePlayers) && !Presence.bIsOnline)
			{
				return TSharedPtr<FOnlineFriend>();
			}
			// Of if they only want friends playing this game
			else if (ListName == EFriendsLists::ToString(EFriendsLists::InGamePlayers) && !Presence.bIsPlayingThisGame)
			{
				return TSharedPtr<FOnlineFriend>();
			}
			return FoundFriend;
		}
	}
	return TSharedPtr<FOnlineFriend>();
}

bool FUserManagerEOS::IsFriend(int32 LocalUserNum, const FUniqueNetId& FriendId, const FString& ListName)
{
	return GetFriend(LocalUserNum, FriendId, ListName).IsValid();
}

bool FUserManagerEOS::QueryRecentPlayers(const FUniqueNetId& UserId, const FString& Namespace)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::QueryRecentPlayers] This method is not supported."));

#if ENGINE_MAJOR_VERSION == 5
		EOSSubsystem->ExecuteNextTick([this, WeakThis = AsWeak(), UserId = UserId.AsShared(), Namespace]()
	{
		if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
#else
	EOSSubsystem->ExecuteNextTick([this, UserId = UserId.AsShared(), Namespace]()
		{
			if (FUserManagerEOSPtr StrongThis = AsShared())
#endif
				{
				TriggerOnQueryRecentPlayersCompleteDelegates(*UserId, Namespace, false, TEXT("This method is not supported."));
			}
		});

	return true;
}

bool FUserManagerEOS::GetRecentPlayers(const FUniqueNetId& UserId, const FString& Namespace, TArray<TSharedRef<FOnlineRecentPlayer>>& OutRecentPlayers)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::GetRecentPlayers] This method is not supported."));

	return false;
}

bool FUserManagerEOS::BlockPlayer(int32 LocalUserNum, const FUniqueNetId& PlayerId)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::BlockPlayer] This method is not supported."));

#if ENGINE_MAJOR_VERSION == 5
			EOSSubsystem->ExecuteNextTick([this, WeakThis = AsWeak(), LocalUserNum, PlayerId = PlayerId.AsShared()]()
	{
		if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
#else
	EOSSubsystem->ExecuteNextTick([this, LocalUserNum, PlayerId = PlayerId.AsShared()]()
		{
			if (FUserManagerEOSPtr StrongThis =	AsShared())
#endif
				{
				TriggerOnBlockedPlayerCompleteDelegates(LocalUserNum, false, *PlayerId, TEXT(""), TEXT("This method is not supported"));
			}
		});

	return true;
}

bool FUserManagerEOS::UnblockPlayer(int32 LocalUserNum, const FUniqueNetId& PlayerId)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::UnblockPlayer] This method is not supported."));

#if ENGINE_MAJOR_VERSION == 5
				EOSSubsystem->ExecuteNextTick([this, WeakThis = AsWeak(), LocalUserNum, PlayerId = PlayerId.AsShared()]()
	{
		if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
#else
	EOSSubsystem->ExecuteNextTick([this, LocalUserNum, PlayerId = PlayerId.AsShared()]()
		{
			if (FUserManagerEOSPtr StrongThis = AsShared())
#endif
				{
				TriggerOnUnblockedPlayerCompleteDelegates(LocalUserNum, false, *PlayerId, TEXT(""), TEXT("This method is not supported"));
			}
		});

	return true;
}

bool FUserManagerEOS::QueryBlockedPlayers(const FUniqueNetId& UserId)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::QueryBlockedPlayers] This method is not supported."));

#if ENGINE_MAJOR_VERSION == 5
					EOSSubsystem->ExecuteNextTick([this, WeakThis = AsWeak(), UserId = UserId.AsShared()]()
	{
		if (FUserManagerEOSPtr StrongThis = WeakThis.Pin())
#else
	EOSSubsystem->ExecuteNextTick([this, UserId = UserId.AsShared()]()
		{
			if (FUserManagerEOSPtr StrongThis = AsShared())
#endif
				{
				TriggerOnQueryBlockedPlayersCompleteDelegates(*UserId, false, TEXT("This method is not supported"));
			}
		});

	return true;
}

bool FUserManagerEOS::GetBlockedPlayers(const FUniqueNetId& UserId, TArray<TSharedRef<FOnlineBlockedPlayer>>& OutBlockedPlayers)
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::GetBlockedPlayers] This method is not supported."));

	return false;
}

void FUserManagerEOS::DumpBlockedPlayers() const
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::DumpBlockedPlayers] This method is not supported."));
}

void FUserManagerEOS::DumpRecentPlayers() const
{
	UE_LOG_ONLINE_FRIEND(Warning, TEXT("[FUserManagerEOS::DumpRecentPlayers] This method is not supported."));
}

bool FUserManagerEOS::HandleFriendsExec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
#if !UE_BUILD_SHIPPING

	bool bWasHandled = true;
	if (FParse::Command(&Cmd, TEXT("ReadFriendsList"))) /* ONLINE (EOS if using EOSPlus) FRIENDS ReadFriendsList 0 default/onlinePlayers/inGamePlayers/inGameAndSessionPlayers */
	{
		int LocalUserNum = FCString::Atoi(*FParse::Token(Cmd, false));

		FString FriendsList = FParse::Token(Cmd, false);

		ReadFriendsList(LocalUserNum, FriendsList, FOnReadFriendsListComplete());
	}
	else if (FParse::Command(&Cmd, TEXT("GetFriendsList"))) /* ONLINE (EOS if using EOSPlus) FRIENDS GetFriendsList 0 default/onlinePlayers/inGamePlayers/inGameAndSessionPlayers */
	{
		int LocalUserNum = FCString::Atoi(*FParse::Token(Cmd, false));

		FString FriendsList = FParse::Token(Cmd, false);

		TArray< TSharedRef<FOnlineFriend> > Friends;
		// Grab the friends data so we can print it out
		if (GetFriendsList(LocalUserNum, FriendsList, Friends))
		{
			UE_LOG_ONLINE_FRIEND(Log, TEXT("FUserManagerEOS::GetFriendsList returned %d friends"), Friends.Num());

			// Log each friend's data out
			for (int32 Index = 0; Index < Friends.Num(); Index++)
			{
				const FOnlineFriend& Friend = *Friends[Index];
				const FOnlineUserPresence& Presence = Friend.GetPresence();
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t%s has unique id (%s)"), *Friend.GetDisplayName(), *Friend.GetUserId()->ToDebugString());
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t Invite status (%s)"), EInviteStatus::ToString(Friend.GetInviteStatus()));
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t Presence: %s"), *Presence.Status.StatusStr);
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t State: %s"), EOnlinePresenceState::ToString(Presence.Status.State));
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsOnline (%s)"), Presence.bIsOnline ? TEXT("true") : TEXT("false"));
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsPlaying (%s)"), Presence.bIsPlaying ? TEXT("true") : TEXT("false"));
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsPlayingThisGame (%s)"), Presence.bIsPlayingThisGame ? TEXT("true") : TEXT("false"));
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsJoinable (%s)"), Presence.bIsJoinable ? TEXT("true") : TEXT("false"));
				UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bHasVoiceSupport (%s)"), Presence.bHasVoiceSupport ? TEXT("true") : TEXT("false"));
			}
		}
	}
	else if (FParse::Command(&Cmd, TEXT("GetFriend"))) /* ONLINE (EOS if using EOSPlus) FRIENDS GetFriend 0 "FriendUserId|FullStr" default/onlinePlayers/inGamePlayers/inGameAndSessionPlayers */
	{
		int LocalUserNum = FCString::Atoi(*FParse::Token(Cmd, false));

		FString FriendUserIdStr = FParse::Token(Cmd, false);
		FUniqueNetIdEOSRef FriendEosId = FUniqueNetIdEOSRegistry::FindOrAdd(FriendUserIdStr).ToSharedRef();

		FString FriendsList = FParse::Token(Cmd, false);

		TSharedPtr<FOnlineFriend> Friend = GetFriend(LocalUserNum, *FriendEosId, FriendsList);
		if (Friend.IsValid())
		{
			const FOnlineUserPresence& Presence = Friend->GetPresence();
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t%s has unique id (%s)"), *Friend->GetDisplayName(), *Friend->GetUserId()->ToDebugString());
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t Invite status (%s)"), EInviteStatus::ToString(Friend->GetInviteStatus()));
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t Presence: %s"), *Presence.Status.StatusStr);
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t State: %s"), EOnlinePresenceState::ToString(Presence.Status.State));
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsOnline (%s)"), Presence.bIsOnline ? TEXT("true") : TEXT("false"));
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsPlaying (%s)"), Presence.bIsPlaying ? TEXT("true") : TEXT("false"));
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsPlayingThisGame (%s)"), Presence.bIsPlayingThisGame ? TEXT("true") : TEXT("false"));
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bIsJoinable (%s)"), Presence.bIsJoinable ? TEXT("true") : TEXT("false"));
			UE_LOG_ONLINE_FRIEND(Log, TEXT("\t\t bHasVoiceSupport (%s)"), Presence.bHasVoiceSupport ? TEXT("true") : TEXT("false"));
		}
	}
	else if (FParse::Command(&Cmd, TEXT("IsFriend"))) /* ONLINE (EOS if using EOSPlus) FRIENDS IsFriend 0 "FriendUserId|FullStr" default/onlinePlayers/inGamePlayers/inGameAndSessionPlayers */
	{
		int LocalUserNum = FCString::Atoi(*FParse::Token(Cmd, false));

		FString FriendUserIdStr = FParse::Token(Cmd, false);
		FUniqueNetIdEOSRef FriendEosId = FUniqueNetIdEOSRegistry::FindOrAdd(FriendUserIdStr).ToSharedRef();

		FString FriendsList = FParse::Token(Cmd, false);

		bool bIsFriend = IsFriend(LocalUserNum, *FriendEosId, FriendsList);
		UE_LOG_ONLINE_FRIEND(Log, TEXT("UserId=%s bIsFriend=%s"), *FriendUserIdStr, *LexToString(bIsFriend));
	}
	else
	{
		UE_LOG_ONLINE_FRIEND(Warning, TEXT("Unknown FRIENDS command: %s"), *FParse::Token(Cmd, true));
		bWasHandled = false;
	}

	return bWasHandled;
#else
	return false;
#endif // !UE_BUILD_SHIPPING
}

// ~IOnlineFriends Interface

struct FPresenceStrings
{
	FPresenceStrings(const FString& InKey, const FString& InValue)
		: Key(*InKey), Value(*InValue)
	{
	}
	FTCHARToUTF8 Key;
	FTCHARToUTF8 Value;
};

struct FRichTextOptions :
	public EOS_PresenceModification_SetRawRichTextOptions
{
	FRichTextOptions() :
		EOS_PresenceModification_SetRawRichTextOptions()
	{
		ApiVersion = EOS_PRESENCE_SETRAWRICHTEXT_API_LATEST;
		RichText = RichTextAnsi;
	}
	char RichTextAnsi[EOS_PRESENCE_RICH_TEXT_MAX_VALUE_LENGTH];
};

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Presence_SetPresenceCompleteCallback, EOS_Presence_SetPresenceCallbackInfo, FUserManagerEOS> FSetPresenceCallback;
#else
typedef TEOSCallback<EOS_Presence_SetPresenceCompleteCallback, EOS_Presence_SetPresenceCallbackInfo> FSetPresenceCallback;
#endif
void FUserManagerEOS::SetPresence(const FUniqueNetId& UserId, const FOnlineUserPresenceStatus& Status, const FOnPresenceTaskCompleteDelegate& Delegate)
{
	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(UserId);
	const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_FALSE)
	{
		UE_LOG_ONLINE(Error, TEXT("Can't SetPresence() for user (%s) since they are not logged in"), *EOSID.ToDebugString());
		return;
	}

	EOS_HPresenceModification ChangeHandle = nullptr;
	EOS_Presence_CreatePresenceModificationOptions Options = { };
	Options.ApiVersion = EOS_PRESENCE_CREATEPRESENCEMODIFICATION_API_LATEST;
	Options.LocalUserId = AccountId;
	EOS_Presence_CreatePresenceModification(EOSSubsystem->PresenceHandle, &Options, &ChangeHandle);
	if (ChangeHandle == nullptr)
	{
		UE_LOG_ONLINE(Error, TEXT("Failed to create a modification handle for setting presence"));
		return;
	}

	EOS_PresenceModification_SetStatusOptions StatusOptions = { };
	StatusOptions.ApiVersion = EOS_PRESENCE_SETSTATUS_API_LATEST;
	StatusOptions.Status = ToEOS_Presence_EStatus(Status.State);
	EOS_EResult SetStatusResult = EOS_PresenceModification_SetStatus(ChangeHandle, &StatusOptions);
	if (SetStatusResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Error, TEXT("EOS_PresenceModification_SetStatus() failed with result code (%d)"), (int32)SetStatusResult);
	}

	// Convert the status string as the rich text string
	FRichTextOptions TextOptions;
	FCStringAnsi::Strncpy(TextOptions.RichTextAnsi, TCHAR_TO_UTF8(*Status.StatusStr), EOS_PRESENCE_RICH_TEXT_MAX_VALUE_LENGTH);
	EOS_EResult SetRichTextResult = EOS_PresenceModification_SetRawRichText(ChangeHandle, &TextOptions);
	if (SetRichTextResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Error, TEXT("EOS_PresenceModification_SetRawRichText() failed with result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(SetRichTextResult)));
	}

	TArray<FPresenceStrings, TInlineAllocator<EOS_PRESENCE_DATA_MAX_KEYS>> RawStrings;
	TArray<EOS_Presence_DataRecord, TInlineAllocator<EOS_PRESENCE_DATA_MAX_KEYS>> Records;
	int32 CurrentIndex = 0;
	// Loop through the properties building records
	for (FPresenceProperties::TConstIterator It(Status.Properties); It && CurrentIndex < EOS_PRESENCE_DATA_MAX_KEYS; ++It, ++CurrentIndex)
	{
		const FPresenceStrings& RawString = RawStrings.Emplace_GetRef(It.Key(), It.Value().ToString());

		EOS_Presence_DataRecord& Record = Records.Emplace_GetRef();
		Record.ApiVersion = EOS_PRESENCE_DATARECORD_API_LATEST;
		Record.Key = RawString.Key.Get();
		Record.Value = RawString.Value.Get();
	}
	EOS_PresenceModification_SetDataOptions DataOptions = { };
	DataOptions.ApiVersion = EOS_PRESENCE_SETDATA_API_LATEST;
	DataOptions.RecordsCount = Records.Num();
	DataOptions.Records = Records.GetData();
	EOS_EResult SetDataResult = EOS_PresenceModification_SetData(ChangeHandle, &DataOptions);
	if (SetDataResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Error, TEXT("EOS_PresenceModification_SetData() failed with result code (%s)"), *EIK_LexToString(SetDataResult));
	}
#if ENGINE_MAJOR_VERSION == 5
	FSetPresenceCallback* CallbackObj = new FSetPresenceCallback(AsWeak());
#else
	FSetPresenceCallback* CallbackObj = new FSetPresenceCallback();
#endif
	CallbackObj->CallbackLambda = [this, Delegate](const EOS_Presence_SetPresenceCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success && AccountIdToStringMap.Contains(Data->LocalUserId))
		{
			FUniqueNetIdEOSRef EOSID = FUniqueNetIdEOSRegistry::FindOrAdd(AccountIdToStringMap[Data->LocalUserId]).ToSharedRef();
			Delegate.ExecuteIfBound(*EOSID, true);
			return;
		}
		UE_LOG_ONLINE(Error, TEXT("SetPresence() failed with result code (%s)"), *EIK_LexToString(Data->ResultCode));
		Delegate.ExecuteIfBound(*FUniqueNetIdEOS::EmptyId(), false);
	};

	EOS_Presence_SetPresenceOptions PresOptions = { };
	PresOptions.ApiVersion = EOS_PRESENCE_SETPRESENCE_API_LATEST;
	PresOptions.LocalUserId = AccountId;
	PresOptions.PresenceModificationHandle = ChangeHandle;
	// Last step commit the changes
	EOS_Presence_SetPresence(EOSSubsystem->PresenceHandle, &PresOptions, CallbackObj, CallbackObj->GetCallbackPtr());
	EOS_PresenceModification_Release(ChangeHandle);
}

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Presence_OnQueryPresenceCompleteCallback, EOS_Presence_QueryPresenceCallbackInfo, FUserManagerEOS> FQueryPresenceCallback;
#else
typedef TEOSCallback<EOS_Presence_OnQueryPresenceCompleteCallback, EOS_Presence_QueryPresenceCallbackInfo> FQueryPresenceCallback;
#endif
void FUserManagerEOS::QueryPresence(const FUniqueNetId& UserId, const FOnPresenceTaskCompleteDelegate& Delegate)
{
	if (DefaultLocalUser < 0)
	{
		UE_LOG_ONLINE(Error, TEXT("Can't QueryPresence() due to no users being signed in"));
		Delegate.ExecuteIfBound(UserId, false);
		return;
	}

	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(UserId);
	const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_FALSE)
	{
		UE_LOG_ONLINE(Error, TEXT("Can't QueryPresence(%s) for unknown unique net id"), *EOSID.ToDebugString());
		Delegate.ExecuteIfBound(UserId, false);
		return;
	}

	EOS_Presence_HasPresenceOptions HasOptions = { };
	HasOptions.ApiVersion = EOS_PRESENCE_HASPRESENCE_API_LATEST;
	HasOptions.LocalUserId = UserNumToAccountIdMap[DefaultLocalUser];
	HasOptions.TargetUserId = AccountId;
	EOS_Bool bHasPresence = EOS_Presence_HasPresence(EOSSubsystem->PresenceHandle, &HasOptions);
	if (bHasPresence == EOS_FALSE)
	{
#if ENGINE_MAJOR_VERSION == 5
		FQueryPresenceCallback* CallbackObj = new FQueryPresenceCallback(AsWeak());
#else
		FQueryPresenceCallback* CallbackObj = new FQueryPresenceCallback();
#endif
		CallbackObj->CallbackLambda = [this, Delegate](const EOS_Presence_QueryPresenceCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success && EpicAccountIdToOnlineUserMap.Contains(Data->TargetUserId))
			{
				// Update the presence data to the most recent
				UpdatePresence(Data->TargetUserId);
				FOnlineUserPtr OnlineUser = EpicAccountIdToOnlineUserMap[Data->TargetUserId];
				Delegate.ExecuteIfBound(*OnlineUser->GetUserId(), true);
				return;
			}
			const FString& TargetUser = FUniqueNetIdEOSRegistry::FindOrAdd(Data->TargetUserId, nullptr)->ToString();
			UE_LOG_ONLINE(Error, TEXT("QueryPresence() for user (%s) failed with result code (%s)"), *TargetUser, *EIK_LexToString(Data->ResultCode));
			Delegate.ExecuteIfBound(*FUniqueNetIdEOS::EmptyId(), false);
		};

		// Query for updated presence
		EOS_Presence_QueryPresenceOptions Options = { };
		Options.ApiVersion = EOS_PRESENCE_QUERYPRESENCE_API_LATEST;
		Options.LocalUserId = HasOptions.LocalUserId;
		Options.TargetUserId = HasOptions.TargetUserId;
		EOS_Presence_QueryPresence(EOSSubsystem->PresenceHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
		return;
	}

	// Update the presence data to the most recent
	UpdatePresence(HasOptions.TargetUserId);
	// It's already present so trigger that it's done
	Delegate.ExecuteIfBound(UserId, true);
}

void FUserManagerEOS::UpdatePresence(EOS_EpicAccountId AccountId)
{
	EOS_Presence_Info* PresenceInfo = nullptr;
	EOS_Presence_CopyPresenceOptions Options = { };
	Options.ApiVersion = EOS_PRESENCE_COPYPRESENCE_API_LATEST;
	Options.LocalUserId = UserNumToAccountIdMap[DefaultLocalUser];
	Options.TargetUserId = AccountId;
	EOS_EResult CopyResult = EOS_Presence_CopyPresence(EOSSubsystem->PresenceHandle, &Options, &PresenceInfo);
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		const FString& NetId = AccountIdToStringMap[AccountId];
		// Create it on demand if we don't have one yet
		if (!NetIdStringToOnlineUserPresenceMap.Contains(NetId))
		{
			FOnlineUserPresenceRef PresenceRef = MakeShareable(new FOnlineUserPresence());
			NetIdStringToOnlineUserPresenceMap.Emplace(NetId, PresenceRef);
		}

		FOnlineUserPresenceRef PresenceRef = NetIdStringToOnlineUserPresenceMap[NetId];
		const FString ProductId(UTF8_TO_TCHAR(PresenceInfo->ProductId));
		const FString ProdVersion(UTF8_TO_TCHAR(PresenceInfo->ProductVersion));
		const FString Platform(UTF8_TO_TCHAR(PresenceInfo->Platform));
		// Convert the presence data to our format
		PresenceRef->Status.State = ToEOnlinePresenceState(PresenceInfo->Status);
		PresenceRef->Status.StatusStr = UTF8_TO_TCHAR(PresenceInfo->RichText);
		PresenceRef->bIsOnline = PresenceRef->Status.State == EOnlinePresenceState::Online;
		PresenceRef->bIsPlaying = !ProductId.IsEmpty();
		PresenceRef->bIsPlayingThisGame = ProductId == EOSSubsystem->ProductId && ProdVersion == EOSSubsystem->EOSSDKManager->GetProductVersion();
//		PresenceRef->bIsJoinable = ???;
//		PresenceRef->bHasVoiceSupport = ???;
		PresenceRef->Status.Properties.Add(TEXT("ProductId"), ProductId);
		PresenceRef->Status.Properties.Add(TEXT("ProductVersion"), ProdVersion);
		PresenceRef->Status.Properties.Add(TEXT("Platform"), Platform);
		for (int32 Index = 0; Index < PresenceInfo->RecordsCount; Index++)
		{
			const EOS_Presence_DataRecord& Record = PresenceInfo->Records[Index];
			PresenceRef->Status.Properties.Add(Record.Key, UTF8_TO_TCHAR(Record.Value));
		}

		// Copy the presence if this is a friend that was updated, so that their data is in sync
		UpdateFriendPresence(NetId, PresenceRef);

		EOS_Presence_Info_Release(PresenceInfo);
	}
	else
	{
		UE_LOG_ONLINE(Error, TEXT("Failed to copy presence data with error code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(CopyResult)));
	}
}

void FUserManagerEOS::UpdateFriendPresence(const FString& FriendId, FOnlineUserPresenceRef Presence)
{
	for (TMap<int32, FFriendsListEOSRef>::TConstIterator It(LocalUserNumToFriendsListMap); It; ++It)
	{
		FFriendsListEOSRef FriendsList = It.Value();
		FOnlineFriendEOSPtr Friend = FriendsList->GetByNetIdString(FriendId);
		if (Friend.IsValid())
		{
			Friend->SetPresence(Presence);
		}
	}
}

EOnlineCachedResult::Type FUserManagerEOS::GetCachedPresence(const FUniqueNetId& UserId, TSharedPtr<FOnlineUserPresence>& OutPresence)
{
	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(UserId);
	if (NetIdStringToOnlineUserPresenceMap.Contains(EOSID.ToString()))
	{
		OutPresence = NetIdStringToOnlineUserPresenceMap[EOSID.ToString()];
		return EOnlineCachedResult::Success;
	}
	return EOnlineCachedResult::NotFound;
}

EOnlineCachedResult::Type FUserManagerEOS::GetCachedPresenceForApp(const FUniqueNetId&, const FUniqueNetId& UserId, const FString&, TSharedPtr<FOnlineUserPresence>& OutPresence)
{
	return GetCachedPresence(UserId, OutPresence);
}

bool FUserManagerEOS::QueryUserInfo(int32 LocalUserNum, const TArray<FUniqueNetIdRef>& UserIds)
{
	TArray<FString> UserEasIdsNeedingExternalMappings;
	UserEasIdsNeedingExternalMappings.Reserve(UserIds.Num());
	
	// Trigger a query for each user in the list
	for (const FUniqueNetIdRef& NetId : UserIds)
	{
		const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(*NetId);
		// Skip querying for local users since we already have that data
		if (StringToUserAccountMap.Contains(EOSID.ToString()))
		{
			continue;
		}
		// Check to see if we know about this user or not
		const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
		if (EOS_EpicAccountId_IsValid(AccountId) == EOS_TRUE)
		{
			// If the user is already registered, we'll update their user info
			if (EpicAccountIdToAttributeAccessMap.Contains(AccountId))
			{
				ReadUserInfo(LocalUserNum, AccountId);
			}
			else
			{
				// If the user is not registered, we'll add it and query their user info
				UserEasIdsNeedingExternalMappings.Add(EIK_LexToString(AccountId));

				// Registering the player will also query the user info data
				AddRemotePlayer(LocalUserNum, EOSID.ToString(), AccountId);
			}
		}
	}

	const FUniqueNetIdEOSPtr LocalId = GetLocalUniqueNetIdEOS(LocalUserNum);
	QueryExternalIdMappings(*LocalId, FExternalIdQueryOptions(), UserEasIdsNeedingExternalMappings, IgnoredMappingDelegate);
	
	return true;
}

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_UserInfo_OnQueryUserInfoCallback, EOS_UserInfo_QueryUserInfoCallbackInfo, FUserManagerEOS> FReadUserInfoCallback;
#else
typedef TEOSCallback<EOS_UserInfo_OnQueryUserInfoCallback, EOS_UserInfo_QueryUserInfoCallbackInfo> FReadUserInfoCallback;
#endif
void FUserManagerEOS::ReadUserInfo(int32 LocalUserNum, EOS_EpicAccountId EpicAccountId)
{
#if ENGINE_MAJOR_VERSION == 5
	FReadUserInfoCallback* CallbackObj = new FReadUserInfoCallback(AsWeak());
#else
	FReadUserInfoCallback* CallbackObj = new FReadUserInfoCallback();
#endif
	CallbackObj->CallbackLambda = [this, LocalUserNum, EpicAccountId](const EOS_UserInfo_QueryUserInfoCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			IAttributeAccessInterfaceRef AttributeAccessRef = EpicAccountIdToAttributeAccessMap[Data->TargetUserId];
			UpdateUserInfo(AttributeAccessRef, Data->LocalUserId, Data->TargetUserId);
		}

		// We mark this player as processed
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
		IsFriendQueryUserInfoOngoingForLocalUserMap[LocalUserNum].RemoveSwap(EpicAccountId, EAllowShrinking::No);
#else
		IsFriendQueryUserInfoOngoingForLocalUserMap[LocalUserNum].RemoveSwap(EpicAccountId, false);
#endif
		ProcessReadFriendsListComplete(LocalUserNum, true, TEXT(""));
	};

	EOS_UserInfo_QueryUserInfoOptions Options = { };
	Options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
	Options.LocalUserId = UserNumToAccountIdMap[DefaultLocalUser];
	Options.TargetUserId = EpicAccountId;
	EOS_UserInfo_QueryUserInfo(EOSSubsystem->UserInfoHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	// We mark this player as pending for processing
	IsFriendQueryUserInfoOngoingForLocalUserMap.FindOrAdd(LocalUserNum).Add(EpicAccountId);
}

bool FUserManagerEOS::GetAllUserInfo(int32 LocalUserNum, TArray<TSharedRef<FOnlineUser>>& OutUsers)
{
	OutUsers.Reset();
	// Get remote users
	for (TMap<FString, FOnlineUserPtr>::TConstIterator It(NetIdStringToOnlineUserMap); It; ++It)
	{
		if (It.Value().IsValid())
		{
			OutUsers.Add(It.Value().ToSharedRef());
		}
	}
	// Get local users
	for (TMap<FString, FUserOnlineAccountEOSRef>::TConstIterator It(StringToUserAccountMap); It; ++It)
	{
		OutUsers.Add(It.Value());
	}
	return true;
}

TSharedPtr<FOnlineUser> FUserManagerEOS::GetUserInfo(int32 LocalUserNum, const FUniqueNetId& UserId)
{
	TSharedPtr<FOnlineUser> OnlineUser;
	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(UserId);
	if (NetIdStringToOnlineUserMap.Contains(EOSID.ToString()))
	{
		OnlineUser = NetIdStringToOnlineUserMap[EOSID.ToString()];
	}
	return OnlineUser;
}

struct FQueryByDisplayNameOptions :
	public EOS_UserInfo_QueryUserInfoByDisplayNameOptions
{
	FQueryByDisplayNameOptions() :
		EOS_UserInfo_QueryUserInfoByDisplayNameOptions()
	{
		ApiVersion = EOS_USERINFO_QUERYUSERINFOBYDISPLAYNAME_API_LATEST;
		DisplayName = DisplayNameAnsi;
	}
	char DisplayNameAnsi[EOS_OSS_STRING_BUFFER_LENGTH];
};

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback, EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo, FUserManagerEOS> FQueryInfoByNameCallback;
#else
typedef TEOSCallback<EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback, EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo> FQueryInfoByNameCallback;
#endif

bool FUserManagerEOS::QueryUserIdMapping(const FUniqueNetId& UserId, const FString& DisplayNameOrEmail, const FOnQueryUserMappingComplete& Delegate)
{
	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(UserId);
	const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_FALSE)
	{
		UE_LOG_ONLINE(Error, TEXT("Specified local user (%s) is not known"), *EOSID.ToDebugString());
		Delegate.ExecuteIfBound(false, UserId, DisplayNameOrEmail, *FUniqueNetIdEOS::EmptyId(), FString::Printf(TEXT("Specified local user (%s) is not known"), *EOSID.ToDebugString()));
		return false;
	}
	int32 LocalUserNum = GetLocalUserNumFromUniqueNetId(UserId);

#if ENGINE_MAJOR_VERSION == 5
	FQueryInfoByNameCallback* CallbackObj = new FQueryInfoByNameCallback(AsWeak());
#else
	FQueryInfoByNameCallback* CallbackObj = new FQueryInfoByNameCallback();
#endif
	CallbackObj->CallbackLambda = [LocalUserNum, DisplayNameOrEmail, this, Delegate](const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data)
	{
		EOS_EResult Result = Data->ResultCode;
		if (GetLoginStatus(LocalUserNum) != ELoginStatus::LoggedIn)
		{
			// Handle the user logging out while a read is in progress
			Result = EOS_EResult::EOS_InvalidUser;
		}

		FString ErrorString;
		bool bWasSuccessful = Result == EOS_EResult::EOS_Success;
		if (bWasSuccessful)
		{
			const FString NetIdStr = EIK_LexToString(Data->TargetUserId);
			FUniqueNetIdEOSPtr LocalUserId = UserNumToNetIdMap[DefaultLocalUser];
			if (!EpicAccountIdToOnlineUserMap.Contains(Data->TargetUserId))
			{
				// Registering the player will also query the presence/user info data
				AddRemotePlayer(LocalUserNum, NetIdStr, Data->TargetUserId);
			}

			Delegate.ExecuteIfBound(true, *LocalUserId, DisplayNameOrEmail, *FUniqueNetIdEOSRegistry::FindOrAdd(NetIdStr), ErrorString);
		}
		else
		{
			ErrorString = FString::Printf(TEXT("QueryUserIdMapping(%d, '%s') failed with EOS result code (%s)"), DefaultLocalUser, *DisplayNameOrEmail, ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
		}
		Delegate.ExecuteIfBound(false, *FUniqueNetIdEOS::EmptyId(), DisplayNameOrEmail, *FUniqueNetIdEOS::EmptyId(), ErrorString);
	};

	FQueryByDisplayNameOptions Options;
	FCStringAnsi::Strncpy(Options.DisplayNameAnsi, TCHAR_TO_UTF8(*DisplayNameOrEmail), EOS_OSS_STRING_BUFFER_LENGTH);
	Options.LocalUserId = AccountId;
	EOS_UserInfo_QueryUserInfoByDisplayName(EOSSubsystem->UserInfoHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return true;
}

struct FQueryByStringIdsOptions :
	public EOS_Connect_QueryExternalAccountMappingsOptions
{
	FQueryByStringIdsOptions(const uint32 InNumStringIds, EOS_ProductUserId InLocalUserId) :
		EOS_Connect_QueryExternalAccountMappingsOptions()
	{
		PointerArray.AddZeroed(InNumStringIds);
		for (int32 Index = 0; Index < PointerArray.Num(); Index++)
		{
			PointerArray[Index] = new char[EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH+1];
		}
		ApiVersion = EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_API_LATEST;
		AccountIdType = EOS_EExternalAccountType::EOS_EAT_EPIC;
		ExternalAccountIds = (const char**)PointerArray.GetData();
		ExternalAccountIdCount = InNumStringIds;
		LocalUserId = InLocalUserId;
	}

	~FQueryByStringIdsOptions()
	{
		for (int32 Index = 0; Index < PointerArray.Num(); Index++)
		{
			delete [] PointerArray[Index];
		}
	}
	TArray<char*> PointerArray;
};

struct FGetAccountMappingOptions :
	public EOS_Connect_GetExternalAccountMappingsOptions
{
	FGetAccountMappingOptions() :
		EOS_Connect_GetExternalAccountMappingsOptions()
	{
		ApiVersion = EOS_CONNECT_GETEXTERNALACCOUNTMAPPINGS_API_LATEST;
		AccountIdType = EOS_EExternalAccountType::EOS_EAT_EPIC;
		TargetExternalUserId = AccountId;
	}
	char AccountId[EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH+1];
};

#if ENGINE_MAJOR_VERSION == 5
typedef TEOSCallback<EOS_Connect_OnQueryExternalAccountMappingsCallback, EOS_Connect_QueryExternalAccountMappingsCallbackInfo, FUserManagerEOS> FQueryByStringIdsCallback;
#else
typedef TEOSCallback<EOS_Connect_OnQueryExternalAccountMappingsCallback, EOS_Connect_QueryExternalAccountMappingsCallbackInfo> FQueryByStringIdsCallback;
#endif

bool FUserManagerEOS::QueryExternalIdMappings(const FUniqueNetId& UserId, const FExternalIdQueryOptions& QueryOptions, const TArray<FString>& ExternalIds, const FOnQueryExternalIdMappingsComplete& Delegate)
{
	const FUniqueNetIdEOS& EOSID = FUniqueNetIdEOS::Cast(UserId);
	const EOS_EpicAccountId AccountId = EOSID.GetEpicAccountId();
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_FALSE)
	{
		Delegate.ExecuteIfBound(false, UserId, QueryOptions, ExternalIds, FString::Printf(TEXT("User (%s) is not logged in, so can't query external account ids"), *EOSID.ToDebugString()));
		return false;
	}

	if (ExternalIds.Num() == 0)
	{
		Delegate.ExecuteIfBound(false, UserId, QueryOptions, ExternalIds, FString::Printf(TEXT("List of ids to query is empty for User (%s), so can't query external account ids"), *EOSID.ToDebugString()));
		return false;
	}

	int32 LocalUserNum = GetLocalUserNumFromUniqueNetId(UserId);

	// Mark the queries as in progress
	IsPlayerQueryExternalMappingsOngoingForLocalUserMap.FindOrAdd(LocalUserNum).Append(ExternalIds);

	const EOS_ProductUserId LocalUserId = EOSID.GetProductUserId();
	const int32 NumBatches = (ExternalIds.Num() / EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_MAX_ACCOUNT_IDS) + 1;
	int32 QueryStart = 0;
	// Process queries in batches since there's a max that can be done at once
	for (int32 BatchCount = 0; BatchCount < NumBatches; BatchCount++)
	{
		const uint32 AmountToProcess = FMath::Min(ExternalIds.Num() - QueryStart, EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_MAX_ACCOUNT_IDS);
		TArray<FString> BatchIds;
		BatchIds.Empty(AmountToProcess);
		FQueryByStringIdsOptions Options(AmountToProcess, LocalUserId);
		// Build an options up per batch
		for (uint32 ProcessedCount = 0; ProcessedCount < AmountToProcess; ProcessedCount++, QueryStart++)
		{
			FCStringAnsi::Strncpy(Options.PointerArray[ProcessedCount], TCHAR_TO_UTF8(*ExternalIds[ProcessedCount]), EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH+1);
			BatchIds.Add(ExternalIds[ProcessedCount]);
		}
#if ENGINE_MAJOR_VERSION == 5
		FQueryByStringIdsCallback* CallbackObj = new FQueryByStringIdsCallback(AsWeak());
#else
		FQueryByStringIdsCallback* CallbackObj = new FQueryByStringIdsCallback();
#endif
		CallbackObj->CallbackLambda = [LocalUserNum, QueryOptions, BatchIds, this, Delegate](const EOS_Connect_QueryExternalAccountMappingsCallbackInfo* Data)
		{
			EOS_EResult Result = Data->ResultCode;
			if (GetLoginStatus(LocalUserNum) != ELoginStatus::LoggedIn)
			{
				// Handle the user logging out while a read is in progress
				Result = EOS_EResult::EOS_InvalidUser;
			}

			FString ErrorString;
			FUniqueNetIdEOSPtr EOSID = FUniqueNetIdEOS::EmptyId();
			if (Result == EOS_EResult::EOS_Success)
			{
				EOSID = UserNumToNetIdMap[LocalUserNum];

				FGetAccountMappingOptions Options;
				Options.LocalUserId = UserNumToProductUserIdMap[DefaultLocalUser];
				// Get the product id for each epic account passed in
				for (const FString& StringId : BatchIds)
				{
					FCStringAnsi::Strncpy(Options.AccountId, TCHAR_TO_UTF8(*StringId), EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH + 1);
					EOS_ProductUserId ProductUserId = EOS_Connect_GetExternalAccountMapping(EOSSubsystem->ConnectHandle, &Options);
					if (EOS_ProductUserId_IsValid(ProductUserId) == EOS_TRUE)
					{
						EOS_EpicAccountId AccountId = EOS_EpicAccountId_FromString(Options.AccountId);
						UpdateRemotePlayerProductUserId(AccountId, ProductUserId);
					}
				}
			}
			else
			{
				ErrorString = FString::Printf(TEXT("EOS_Connect_QueryExternalAccountMappings() failed with result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
			}

			// Mark all queries as complete
			TArray<FString>& OngoingQueries = IsPlayerQueryExternalMappingsOngoingForLocalUserMap[LocalUserNum];
			for (const FString& StringId : BatchIds)
			{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
				OngoingQueries.RemoveSwap(StringId, EAllowShrinking::No);
#else
				OngoingQueries.RemoveSwap(StringId, false);
#endif
			}

			const bool bWasSuccessful = Result == EOS_EResult::EOS_Success;
			Delegate.ExecuteIfBound(bWasSuccessful, *EOSID, QueryOptions, BatchIds, ErrorString);
		};

		EOS_Connect_QueryExternalAccountMappings(EOSSubsystem->ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	return true;
}

void FUserManagerEOS::GetExternalIdMappings(const FExternalIdQueryOptions& QueryOptions, const TArray<FString>& ExternalIds, TArray<FUniqueNetIdPtr>& OutIds)
{
	OutIds.Reset();
	for (const FString& AccountIdStr : ExternalIds)
	{
		OutIds.Add(GetExternalIdMapping(QueryOptions, AccountIdStr));
	}
}

FUniqueNetIdPtr FUserManagerEOS::GetExternalIdMapping(const FExternalIdQueryOptions& QueryOptions, const FString& ExternalId)
{
	FUniqueNetIdPtr NetId;
	EOS_EpicAccountId AccountId = EOS_EpicAccountId_FromString(TCHAR_TO_UTF8(*ExternalId));
	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_TRUE && AccountIdToStringMap.Contains(AccountId))
	{
		const FString& NetIdStr = AccountIdToStringMap[AccountId];
		NetId = NetIdStringToOnlineUserMap[NetIdStr]->GetUserId();
	}
	return NetId;
}

#endif