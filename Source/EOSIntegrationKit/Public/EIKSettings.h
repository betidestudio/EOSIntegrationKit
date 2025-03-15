// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/ObjectMacros.h"
#include "Engine/RuntimeOptionsBase.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/MessageDialog.h"
#include "Engine/DataAsset.h"
#include "Runtime/Launch/Resources/Version.h"
#include "EIKSettings.generated.h"

/** Native version of the UObject based config data */
struct FEOSArtifactSettings
{
	FString ArtifactName;
	FString ClientId;
	FString ClientSecret;
	FString ProductId;
	FString SandboxId;
	FString DeploymentId;
	FString EncryptionKey;

	void ParseRawArrayEntry(const FString& RawLine);
};

UCLASS(Deprecated)
class UDEPRECATED_EIKArtifactSettings :
	public UDataAsset
{
	GENERATED_BODY()

public:
	UDEPRECATED_EIKArtifactSettings()
	{
	}
};

USTRUCT(BlueprintType)
struct FEArtifactSettings
{
	GENERATED_BODY()

public:
	/** This needs to match what the launcher passes in the -epicapp command line arg */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	FString ArtifactName;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Artifact Settings")
	FString ClientId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Artifact Settings")
	FString ClientSecret;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Artifact Settings")
	FString ProductId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Artifact Settings")
	FString SandboxId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Artifact Settings")
	FString DeploymentId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Artifact Settings")
	FString EncryptionKey = TEXT("9B86F278E855EE69855A5CAE0B1AD04166A143FD98CF96EC71AA4409F9B301C3");

	FEOSArtifactSettings ToNative() const;
};

/** Native version of the UObject based config data */
struct FEOSSettings
{
	FString ApiKey;
	FString ReturnLevelName;
	FString CacheDir;
	FString DefaultArtifactName;
	FString VoiceArtifactName;
	FString DedicatedServerArtifactName;
	int32 TickBudgetInMilliseconds;
	int32 TitleStorageReadChunkLength;
	bool bEnableOverlay;
	bool bEnableSocialOverlay;
	bool bEnableEditorOverlay;
	bool bUseLauncherChecks;
	bool bUseEAS;
	bool bUseEOSConnect;
	bool bUseEOSSessions;
	bool bMirrorStatsToEOS;
	bool bMirrorAchievementsToEOS;
	bool bMirrorPresenceToEAS;
	TArray<FEOSArtifactSettings> Artifacts;
	TArray<FString> TitleStorageTags;
};

UENUM(BlueprintType)
enum EEIK_AutoLoginType {
	AutoLogin_None 			UMETA(DisplayName="None"),
	/** Developer Token login will automatically log the user in with the developer token depending upon the standalone number */
	AutoLogin_DeveloperTool 	UMETA(DisplayName="Developer Tool"),
	/** Persistent Auth will automatically log the user in with EAS credentials if they have previously logged in */
	AutoLogin_PersistentAuth 	UMETA(DisplayName="Persistent Auth"),
	/** Device ID login will automatically log the user in with the device ID */
	AutoLogin_DeviceIdLogin 	UMETA(DisplayName="Device ID Login"),
	/** Account Portal login will automatically log the user in with the Epic Account Portal */
	AutoLogin_AccountPortalLogin 	UMETA(DisplayName="Account Portal Login"),
	/** Platform login will automatically log the user in with the platform's login system */
	AutoLogin_PlatformLogin 	UMETA(DisplayName="Platform Login"),
	/** Steam login will automatically log the user in with the Steam login system */
	AutoLogin_SteamLogin 	UMETA(DisplayName="Steam Login"),
	/** (NOT ACTIVE ATM) PSN login will automatically log the user in with the PSN login system */
	AutoLogin_PSNLogin 	UMETA(DisplayName="PSN Login"),
	/** Google login will automatically log the user in with the Google login system but only works on Android */
	AutoLogin_GoogleLogin 	UMETA(DisplayName="Google Login"),
	/** Apple login will automatically log the user in with the Apple login system but only works on iOS */
	AutoLogin_AppleLogin 	UMETA(DisplayName="Apple Login"),
	
};

UENUM(BlueprintType)
enum EEIK_FallbackForAutoLoginType
{
	/** No fallback */
	Fallback_None UMETA(DisplayName="None"),
	/** Use the device id login */
	Fallback_DeviceIdLogin UMETA(DisplayName="Device ID Login"),
	/** Use the Epic Account Portal login */
	Fallback_AccountPortalLogin UMETA(DisplayName="Account Portal Login")
};

UENUM(BlueprintType)
enum EEIK_LoginFlags_LocalForSettings
{
	T_EOS_AS_NoFlags = 0 UMETA(DisplayName = "No Flags"),
	/** Permissions to see your account ID, display name, and language */
	EOS_AS_BasicProfile = 0x1 UMETA(DisplayName = "Basic Profile"),
	/** Permissions to see a list of your friends who use this application */
	EOS_AS_FriendsList = 0x2 UMETA(DisplayName = "Friends List"),
	/** Permissions to set your online presence and see presence of your friends */
	EOS_AS_Presence = 0x4 UMETA(DisplayName = "Presence"),
	/** Permissions to manage the Epic friends list. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures. */
	EOS_AS_FriendsManagement = 0x8 UMETA(DisplayName = "Friends Management"),
	/** Permissions to see email in the response when fetching information for a user. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures. */
	EOS_AS_Email = 0x10 UMETA(DisplayName = "Email"),
	/** Permissions to see your country */
	EOS_AS_Country = 0x20 UMETA(DisplayName = "Country"),
};

UENUM(BlueprintType)
enum EEIK_BuildConfiguration
{
	/** Unknown build configuration. */
	EIK_UnknownBuild = 0 UMETA(DisplayName = "Unknown"),

	/** Debug build. */
	EIK_Debug = 1 UMETA(DisplayName = "Debug"),

	/** DebugGame build. */
	EIK_DebugGame = 2 UMETA(DisplayName = "DebugGame"),

	/** Development build. */
	EIK_Development = 3 UMETA(DisplayName = "Development"),

	/** Shipping build. */
	EIK_Shipping = 4 UMETA(DisplayName = "Shipping"),

	/** Test build. */
	EIK_Test = 5 UMETA(DisplayName = "Test"),
};

UCLASS(Config=Engine, DefaultConfig)
class EOSINTEGRATIONKIT_API UEIKSettings :
	public URuntimeOptionsBase
{
	GENERATED_BODY()

public:


	/** This will automatically setup EOS Integration Kit.
	 *
	 * Restart the editor after changing this value.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Integration Kit Settings")
	bool bAutomaticallySetupEIK;
	/** If set to true, on every launch of the editor, the developer tool will be launched. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Integration Kit Settings")
	bool bAutoLaunchDevTool = false;
	/** An organization is the highest level in the Epic Online Services (EOS) product management ecosystem. It encompasses all the products and the associated members, such as a publisher or game studio. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Integration Kit Settings")
	FString OrganizationName;
	/** Products are games or other software projects that contain sandboxes and deployments within EOS. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Integration Kit Settings")
	FString ProductName;

	/** Should the Google One Tap be enabled? Should be off for Quest builds */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Integration Kit Settings")
	bool bEnableGoogleOneTap = false;

	/** Should Google Play Games be enabled? Works only on Android. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Integration Kit Settings")
	bool bEnableGooglePlayGames = false;

	/** Google Play Games App ID */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Integration Kit Settings", meta = (EditCondition = "bEnableGooglePlayGames"))
	FString GooglePlayGamesAppID = FString("");
	
	/** Auto-Logins the player into the game. Can be used for testing or games with only 1 type of login */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Login Settings|Auto Login")
	TEnumAsByte<EEIK_AutoLoginType> AutoLoginType;

	/** If the AutoLoginType fails, this will be used as a fallback */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Login Settings|Auto Login")
	TEnumAsByte<EEIK_FallbackForAutoLoginType> FallbackForAutoLoginType;

	/** If true, the Auth Interface will be used to login the user, inshort, the user will be logged in using the Epic Account Services */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Login Settings|Auto Login")
	bool bUse_EAS_ForAutoLogin = false;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Login Settings|Auto Login")
	FString DeveloperToolUrl = TEXT("localhost:6300");
	
	/** LoginFlags help define what permissions the user has when they login. */ 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Login Settings")
	TArray<TEnumAsByte<EEIK_LoginFlags_LocalForSettings>> LoginFlags = {EEIK_LoginFlags_LocalForSettings::EOS_AS_BasicProfile, EEIK_LoginFlags_LocalForSettings::EOS_AS_FriendsList, EEIK_LoginFlags_LocalForSettings::EOS_AS_Presence};

	/*Your api key found in Game Services -> Player Ticketing -> [Show Api Key]*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "EOS Settings|Player Ticketing Settings")
	FString ApiKey;
	
	/**
	 * The directory any PDS/TDS files are cached into. This is per artifact e.g.:
	 *
	 * <UserDir>/<ArtifactId>/<CacheDir>
	*/

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Title Storage Settings")
	FString CacheDir = TEXT("CacheDir");

	/** Used to throttle how much time EOS ticking can take */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	int32 TickBudgetInMilliseconds = 0;

	/** Set to true to enable the overlay (ecom features) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Overlay Settings")
	bool bEnableOverlay = false;

	/** Set to true to enable the social overlay (friends, invites, etc.) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Overlay Settings")
	bool bEnableSocialOverlay = false;

	/** Set to true to enable the overlay when running in the editor */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "EOS Settings|Overlay Settings")
	bool bEnableEditorOverlay = false;
	
	/** Select which level which leave party button in social overlay will return to. Leave empty to return to defult game map. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "EOS Settings|Overlay Settings")
	FString ReturnLevelName;


	/** Set to true to enable the social overlay (friends, invites, etc.) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings", DisplayName="Require Being Launched by the Epic Games Store")
	bool bUseLauncherChecks = false;

	/** Tag combinations for paged queries in title file enumerations, separate tags within groups using `+` */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Title Storage Settings")
	TArray<FString> TitleStorageTags;

	/** Chunk size used when reading a title file */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Title Storage Settings")
	int32 TitleStorageReadChunkLength = 0;

	/** Used when launched from a store other than EGS or when the specified artifact name was not present */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings")
	FString DefaultArtifactName = TEXT("DefaultArtifact");

	/** Artifact settings to be used for voice operations */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings")
	FString VoiceArtifactName = TEXT("DefaultArtifact");

	/** Artifact settings to be used for Dedicated Servers. If empty, the default artifact will be used  but Dedicated Server usually require the policy to not have User Required */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings")
	FString DedicatedServerArtifactName = TEXT("DefaultArtifact");

	/** Artifact settings to be used for IOS */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings|Platform Specific")
	FString PlatformSpecificArtifactName = TEXT("DefaultArtifact");

	/** Per artifact SDK settings. A game might have a FooStaging, FooQA, and public Foo artifact */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings")
	TArray<FEArtifactSettings> Artifacts;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings|Platform Specific|Android")
	FString ClientId;

	/** GoogleSignIn iOS Settings **/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings|Platform Specific|iOS|GoogleSignIn")
	FString Google_ClientId;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings|Artifact Settings|Platform Specific|iOS|GoogleSignIn")
	FString Google_ReverseClientId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy", DisplayName="Build Configuration")
	TEnumAsByte<EEIK_BuildConfiguration> OneClick_BuildConfiguration;

	/** Use the Organization ID string that was provided along with your credentials. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Credentials", DisplayName="Organization Id")
	FString OneClick_OrganizationId;

	/** Use the Product ID string that was provided along with your credentials. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Credentials", DisplayName="Product Id")
	FString OneClick_ProductId;

	/** Use the Artifact ID string that was provided along with your credentials. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Credentials", DisplayName="Artifact Id")
	FString OneClick_ArtifactId;
	
	/*BuildPatchTool uses a unique Client ID and Client Secret, separate from any EOS Client IDs your game may use. Refer to the Build Patch Tool Credentials section of your Product Settings in Dev Portal to obtain the correct Client ID and Secret. Clients listed under SDK Credentials will not function with the BuildPatchTool.*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Credentials", DisplayName="Client Id")
	FString OneClick_ClientId;
	
	/*BuildPatchTool uses a unique Client ID and Client Secret, separate from any EOS Client IDs your game may use. Refer to the Build Patch Tool Credentials section of your Product Settings in Dev Portal to obtain the correct Client ID and Secret. Clients listed under SDK Credentials will not function with the BuildPatchTool.*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Credentials", DisplayName="Client Secret")
	FString OneClick_ClientSecret;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Parameters", DisplayName="Cloud Dir")
	FString OneClick_CloudDirOverride;
	/** The commandline to send to the app on launch. This can be set to “” when no additional arguments are needed. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Parameters", DisplayName="Args Override")
	FString OneClick_ArgsOverride;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Parameters", DisplayName="App Launch Override")
	FString OneClick_AppLaunchOverride;
	

	/** Please leave this empty unless you really want to override the build version. We assign the build version automatically based on the timestamp of the build and the game version. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Parameters", DisplayName="Build Version Override")
	FString OneClick_BuildVersionOverride;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Parameters", DisplayName="App Args Override")
	FString OneClick_AppArgsOverride;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="One Click Deploy|Build Patch Tool Parameters", DisplayName="Build Root Override")
	FString OneClick_BuildRootOverride;

	
	/** Set to true to have Epic Accounts used (friends list will be unified with the default platform) */
	UPROPERTY()
	bool bUseEAS = false;

	/** Set to true to have EOS Connect APIs used to link accounts for crossplay */
	UPROPERTY()
	bool bUseEOSConnect = false;

	/** Set to true to write stats to EOS as well as the default platform */
	UPROPERTY()
	bool bMirrorStatsToEOS = false;

	/** Set to true to write achievement data to EOS as well as the default platform */
	UPROPERTY()
	bool bMirrorAchievementsToEOS = false;

	/** Set to true to use EOS for session registration with data mirrored to the default platform */
	UPROPERTY()
	bool bUseEOSSessions = false;

	/** Set to true to have Epic Accounts presence information updated when the default platform is updated */
	UPROPERTY()
	bool bMirrorPresenceToEAS = false;

	/** Find the Settings for an artifact by name */
	static bool GetSettingsForArtifact(const FString& ArtifactName, FEOSArtifactSettings& OutSettings);

	static FEOSSettings GetSettings();
	FEOSSettings ToNative() const;

private:
#if WITH_EDITOR
	EAppReturnType::Type ShowRestartWarning(const FText& Title);
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	static bool AutoGetSettingsForArtifact(const FString& ArtifactName, FEOSArtifactSettings& OutSettings);
	static bool ManualGetSettingsForArtifact(const FString& ArtifactName, FEOSArtifactSettings& OutSettings);

	static FEOSSettings AutoGetSettings();
	static const FEOSSettings& ManualGetSettings();
};