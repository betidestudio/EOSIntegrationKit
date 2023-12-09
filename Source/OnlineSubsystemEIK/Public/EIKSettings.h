//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "UObject/ObjectMacros.h"
#include "Engine/RuntimeOptionsBase.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/MessageDialog.h"
#include "Engine/DataAsset.h"
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
	FString EncryptionKey;

	FEOSArtifactSettings ToNative() const;
};

/** Native version of the UObject based config data */
struct FEOSSettings
{
	FString CacheDir;
	FString DefaultArtifactName;
	int32 TickBudgetInMilliseconds;
	int32 TitleStorageReadChunkLength;
	bool bEnableOverlay;
	bool bEnableSocialOverlay;
	bool bEnableEditorOverlay;
	bool bShouldEnforceBeingLaunchedByEGS;
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
enum class EAutoLoginTypes : uint8 {
	None 			UMETA(DisplayName="None"),
	AccountPortal       UMETA(DisplayName="Account Portal"),
	PersistentAuth              UMETA(DisplayName="Persistent Auth"),
	DeviceID        UMETA(DisplayName="Device ID"),
};

UCLASS(Config=Engine, DefaultConfig)
class ONLINESUBSYSTEMEIK_API UEIKSettings :
	public URuntimeOptionsBase
{
	GENERATED_BODY()

public:

	/** This will automatically setup EOS Integration Kit.
	 *
	 * Restart the editor after changing this value.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK  Settings")
	bool bAutomaticallySetupEIK;
	/** An organization is the highest level in the Epic Online Services (EOS) product management ecosystem. It encompasses all the products and the associated members, such as a publisher or game studio. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Specific Settings")
	FString OrganizationName;
	/** Products are games or other software projects that contain sandboxes and deployments within EOS. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Specific Settings")
	FString ProductName;
	/** Auto-Logins the player into the game. Can be used for testing or games with only 1 type of login */
	EAutoLoginTypes AutoLoginType;
	/** This will request the country code from the EOS SDK and login */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Specific Settings")
	bool bUseCountryScope;	
	/** This will show the advanced logs for the EOS SDK and EOS Integration Kit functions*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Specific Settings")
	bool bShowAdvancedLogs;


	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Android/IOS Settings")
	FString ClientId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Android/IOS Settings")
	FString ClientSecret;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Android/IOS Settings")
	FString ProductId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Android/IOS Settings")
	FString SandboxId;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EIK Android/IOS Settings")
	FString DeploymentId;
	/**
	 * The directory any PDS/TDS files are cached into. This is per artifact e.g.:
	 *
	 * <UserDir>/<ArtifactId>/<CacheDir>
	*/

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	FString CacheDir = TEXT("CacheDir");

	/** Used when launched from a store other than EGS or when the specified artifact name was not present */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	FString DefaultArtifactName;

	/** Used to throttle how much time EOS ticking can take */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	int32 TickBudgetInMilliseconds = 0;

	/** Set to true to enable the overlay (ecom features) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	bool bEnableOverlay = false;

	/** Set to true to enable the social overlay (friends, invites, etc.) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	bool bEnableSocialOverlay = false;

	/** Set to true to enable the overlay when running in the editor */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "EOS Settings")
	bool bEnableEditorOverlay = false;

	/** Set to true to enable the social overlay (friends, invites, etc.) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings", DisplayName="Require Being Launched by the Epic Games Store")
	bool bShouldEnforceBeingLaunchedByEGS = false;

	/** Tag combinations for paged queries in title file enumerations, separate tags within groups using `+` */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	TArray<FString> TitleStorageTags;

	/** Chunk size used when reading a title file */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	int32 TitleStorageReadChunkLength = 0;

	/** Per artifact SDK settings. A game might have a FooStaging, FooQA, and public Foo artifact */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOS Settings")
	TArray<FEArtifactSettings> Artifacts;

	/** Set to true to have Epic Accounts used (friends list will be unified with the default platform) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOSPlus Login Settings", DisplayName="Use Epic Account for EOS login (requires account linking)")
	bool bUseEAS = false;

	/** Set to true to have EOS Connect APIs used to link accounts for crossplay */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="EOSPlus Login Settings", DisplayName="Use Crossplatform User IDs for EOS Login (doesn't use Epic Account)")
	bool bUseEOSConnect = false;

	/** Set to true to write stats to EOS as well as the default platform */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Crossplay Settings")
	bool bMirrorStatsToEOS = false;

	/** Set to true to write achievement data to EOS as well as the default platform */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Crossplay Settings")
	bool bMirrorAchievementsToEOS = false;

	/** Set to true to use EOS for session registration with data mirrored to the default platform */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Crossplay Settings", DisplayName="Use Crossplay Sessions")
	bool bUseEOSSessions = false;

	/** Set to true to have Epic Accounts presence information updated when the default platform is updated */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Crossplay Settings")
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