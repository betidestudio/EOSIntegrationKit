// Copyright Epic Games, Inc. All Rights Reserved.

#include "EIKSettings.h"

#include "EOSIntegrationKit.h"
#include "Algo/Transform.h"
#include "Misc/CommandLine.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"

#if ENGINE_MAJOR_VERSION >= 5
#include UE_INLINE_GENERATED_CPP_BY_NAME(EIKSettings)
#endif

#if WITH_EDITOR
	#include "Misc/MessageDialog.h"
	#include "UnrealEdMisc.h"
#endif

#define LOCTEXT_NAMESPACE "EOS"

#define INI_SECTION TEXT("/Script/EOSIntegrationKit.EIKSettings")

inline bool IsAnsi(const FString& Source)
{
	for (const TCHAR& IterChar : Source)
	{
		if (!FChar::IsPrint(IterChar))
		{
			return false;
		}
	}
	return true;
}

inline bool IsHex(const FString& Source)
{
	for (const TCHAR& IterChar : Source)
	{
		if (!FChar::IsHexDigit(IterChar))
		{
			return false;
		}
	}
	return true;
}

inline bool ContainsWhitespace(const FString& Source)
{
	for (const TCHAR& IterChar : Source)
	{
		if (FChar::IsWhitespace(IterChar))
		{
			return true;
		}
	}
	return false;
}

FEOSArtifactSettings FEArtifactSettings::ToNative() const
{
	FEOSArtifactSettings Native;

	Native.ArtifactName = ArtifactName;
	Native.ClientId = ClientId;
	Native.ClientSecret = ClientSecret;
	Native.DeploymentId = DeploymentId;
	Native.EncryptionKey = EncryptionKey;
	Native.ProductId = ProductId;
	Native.SandboxId = SandboxId;

	return Native;
}

inline FString StripQuotes(const FString& Source)
{
	if (Source.StartsWith(TEXT("\"")))
	{
		return Source.Mid(1, Source.Len() - 2);
	}
	return Source;
}

void FEOSArtifactSettings::ParseRawArrayEntry(const FString& RawLine)
{
	const TCHAR* Delims[4] = { TEXT("("), TEXT(")"), TEXT("="), TEXT(",") };
	TArray<FString> Values;
	RawLine.ParseIntoArray(Values, Delims, 4, false);
	for (int32 ValueIndex = 0; ValueIndex < Values.Num(); ValueIndex++)
	{
		if (Values[ValueIndex].IsEmpty())
		{
			continue;
		}

		// Parse which struct field
		if (Values[ValueIndex] == TEXT("ArtifactName"))
		{
			ArtifactName = StripQuotes(Values[ValueIndex + 1]);
		}
		else if (Values[ValueIndex] == TEXT("ClientId"))
		{
			ClientId = StripQuotes(Values[ValueIndex + 1]);
		}
		else if (Values[ValueIndex] == TEXT("ClientSecret"))
		{
			ClientSecret = StripQuotes(Values[ValueIndex + 1]);
		}
		else if (Values[ValueIndex] == TEXT("ProductId"))
		{
			ProductId = StripQuotes(Values[ValueIndex + 1]);
		}
		else if (Values[ValueIndex] == TEXT("SandboxId"))
		{
			SandboxId = StripQuotes(Values[ValueIndex + 1]);
		}
		else if (Values[ValueIndex] == TEXT("DeploymentId"))
		{
			DeploymentId = StripQuotes(Values[ValueIndex + 1]);
		}
		else if (Values[ValueIndex] == TEXT("EncryptionKey"))
		{
			EncryptionKey = StripQuotes(Values[ValueIndex + 1]);
		}
		ValueIndex++;
	}
}

FEOSSettings UEIKSettings::GetSettings()
{
	if (UObjectInitialized())
	{
		return UEIKSettings::AutoGetSettings();
	}

	return UEIKSettings::ManualGetSettings();
}

FEOSSettings UEIKSettings::AutoGetSettings()
{
	return GetDefault<UEIKSettings>()->ToNative();
}

const FEOSSettings& UEIKSettings::ManualGetSettings()
{
	static TOptional<FEOSSettings> CachedSettings;

	if (!CachedSettings.IsSet())
	{
		CachedSettings.Emplace();

		GConfig->GetString(INI_SECTION, TEXT("ApiKey"), CachedSettings->ApiKey, GEngineIni);
		GConfig->GetString(INI_SECTION, TEXT("ReturnLevelName"), CachedSettings->ReturnLevelName, GEngineIni);
		GConfig->GetString(INI_SECTION, TEXT("CacheDir"), CachedSettings->CacheDir, GEngineIni);
		GConfig->GetString(INI_SECTION, TEXT("DefaultArtifactName"), CachedSettings->DefaultArtifactName, GEngineIni);
		GConfig->GetInt(INI_SECTION, TEXT("TickBudgetInMilliseconds"), CachedSettings->TickBudgetInMilliseconds, GEngineIni);
		GConfig->GetInt(INI_SECTION, TEXT("TitleStorageReadChunkLength"), CachedSettings->TitleStorageReadChunkLength, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bEnableOverlay"), CachedSettings->bEnableOverlay, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bEnableSocialOverlay"), CachedSettings->bEnableSocialOverlay, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bEnableEditorOverlay"), CachedSettings->bEnableEditorOverlay, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bUseLauncherChecks"), CachedSettings->bUseLauncherChecks, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bUseEAS"), CachedSettings->bUseEAS, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bUseEOSConnect"), CachedSettings->bUseEOSConnect, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bUseEOSSessions"), CachedSettings->bUseEOSSessions, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bMirrorStatsToEOS"), CachedSettings->bMirrorStatsToEOS, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bMirrorAchievementsToEOS"), CachedSettings->bMirrorAchievementsToEOS, GEngineIni);
		GConfig->GetBool(INI_SECTION, TEXT("bMirrorPresenceToEAS"), CachedSettings->bMirrorPresenceToEAS, GEngineIni);
		// Artifacts explicitly skipped
		GConfig->GetArray(INI_SECTION, TEXT("TitleStorageTags"), CachedSettings->TitleStorageTags, GEngineIni);
	}

	return *CachedSettings;
}

FEOSSettings UEIKSettings::ToNative() const
{
	FEOSSettings Native;

	Native.ApiKey = ApiKey;
	Native.ReturnLevelName = ReturnLevelName;
	Native.CacheDir = CacheDir;
	Native.DefaultArtifactName = DefaultArtifactName;
	Native.TickBudgetInMilliseconds = TickBudgetInMilliseconds;
	Native.TitleStorageReadChunkLength = TitleStorageReadChunkLength;
	Native.bEnableOverlay = bEnableOverlay;
	Native.DedicatedServerArtifactName = DedicatedServerArtifactName;
	Native.VoiceArtifactName = VoiceArtifactName;
	Native.bEnableSocialOverlay = bEnableSocialOverlay;
	Native.bEnableEditorOverlay = bEnableEditorOverlay;
	Native.bUseLauncherChecks = bUseLauncherChecks;
	Native.bUseEAS = bUseEAS;
	Native.bUseEOSConnect = bUseEOSConnect;
	Native.bUseEOSSessions = bUseEOSSessions;
	Native.bMirrorStatsToEOS = bMirrorStatsToEOS;
	Native.bMirrorAchievementsToEOS = bMirrorAchievementsToEOS;
	Native.bMirrorPresenceToEAS = bMirrorPresenceToEAS;
	Algo::Transform(Artifacts, Native.Artifacts, &FEArtifactSettings::ToNative);
	Native.TitleStorageTags = TitleStorageTags;

	return Native;
}


#if WITH_EDITOR
EAppReturnType::Type UEIKSettings::ShowRestartWarning(const FText& Title)
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >=3
	return FMessageDialog::Open(EAppMsgType::OkCancel, LOCTEXT("ActionRestartMsg", "Imported settings won't be applied until the editor is restarted. Do you wish to restart now (you will be prompted to save any changes)?"), Title);
#else
	return FMessageDialog::Open(EAppMsgType::OkCancel, LOCTEXT("ActionRestartMsg", "Imported settings won't be applied until the editor is restarted. Do you wish to restart now (you will be prompted to save any changes)?"), &Title);
#endif
}
#endif

bool UEIKSettings::GetSettingsForArtifact(const FString& ArtifactName, FEOSArtifactSettings& OutSettings)
{
	if (UObjectInitialized())
	{
		return UEIKSettings::AutoGetSettingsForArtifact(ArtifactName, OutSettings);
	}
	return UEIKSettings::ManualGetSettingsForArtifact(ArtifactName, OutSettings);
}

bool UEIKSettings::ManualGetSettingsForArtifact(const FString& ArtifactName, FEOSArtifactSettings& OutSettings)
{
    static TOptional<FString> CachedDefaultArtifactName;
    static TOptional<TArray<FEOSArtifactSettings>> CachedArtifactSettings;

    if (!CachedDefaultArtifactName.IsSet())
    {
        CachedDefaultArtifactName.Emplace();

        GConfig->GetString(INI_SECTION, TEXT("DefaultArtifactName"), *CachedDefaultArtifactName, GEngineIni);
    }

    if (!CachedArtifactSettings.IsSet())
    {
        CachedArtifactSettings.Emplace();

        TArray<FString> Artifacts;
        GConfig->GetArray(INI_SECTION, TEXT("Artifacts"), Artifacts, GEngineIni);
        for (const FString& Line : Artifacts)
        {
            FEOSArtifactSettings Artifact;
            Artifact.ParseRawArrayEntry(Line);
            CachedArtifactSettings->Add(Artifact);
        }
    }

    FString ArtifactNameOverride;
    FString DeploymentIdOverride;
    FString SandboxIdOverride;

    // Figure out which config object we are loading
    FParse::Value(FCommandLine::Get(), TEXT("EOSArtifactNameOverride="), ArtifactNameOverride);
    if (ArtifactNameOverride.IsEmpty())
    {
        ArtifactNameOverride = ArtifactName;
    }
	if(IsRunningDedicatedServer() && ArtifactNameOverride.IsEmpty())
	{
		FString DedicatedServerArtifactName;
		GConfig->GetString(INI_SECTION, TEXT("DedicatedServerArtifactName"), DedicatedServerArtifactName, GEngineIni);
		if(!DedicatedServerArtifactName.IsEmpty())
		{
			ArtifactNameOverride = DedicatedServerArtifactName;
		}
	}

    // Override DeploymentId and SandboxId
    if(FParse::Value(FCommandLine::Get(), TEXT("epicdeploymentid="), DeploymentIdOverride))
	{
    	UE_LOG(LogEOSIntegrationKit, Log, TEXT("Overriding DeploymentId with %s"), *DeploymentIdOverride);
	}
    if(FParse::Value(FCommandLine::Get(), TEXT("epicsandboxid="), SandboxIdOverride))
    {
	    UE_LOG(LogEOSIntegrationKit, Log, TEXT("Overriding SandboxId with %s"), *SandboxIdOverride);
    }

    // Search by name and then default if not found
    for (const FEOSArtifactSettings& Artifact : *CachedArtifactSettings)
    {
        if (Artifact.ArtifactName == ArtifactNameOverride)
        {
            OutSettings = Artifact;
            if (!DeploymentIdOverride.IsEmpty())
            {
                OutSettings.DeploymentId = DeploymentIdOverride;
            }
            if (!SandboxIdOverride.IsEmpty())
            {
                OutSettings.SandboxId = SandboxIdOverride;
            }
            return true;
        }
    }

    for (const FEOSArtifactSettings& Artifact : *CachedArtifactSettings)
    {
        if (Artifact.ArtifactName == *CachedDefaultArtifactName)
        {
            OutSettings = Artifact;
            if (!DeploymentIdOverride.IsEmpty())
            {
                OutSettings.DeploymentId = DeploymentIdOverride;
            }
            if (!SandboxIdOverride.IsEmpty())
            {
                OutSettings.SandboxId = SandboxIdOverride;
            }
            return true;
        }
    }

    return false;
}

bool UEIKSettings::AutoGetSettingsForArtifact(const FString& ArtifactName, FEOSArtifactSettings& OutSettings)
{
	const UEIKSettings* This = GetDefault<UEIKSettings>();
	FString DeploymentIdOverride;
	FString SandboxIdOverride;
	if(FParse::Value(FCommandLine::Get(), TEXT("epicdeploymentid="), DeploymentIdOverride))
	{
		UE_LOG(LogEOSIntegrationKit, Log, TEXT("Overriding DeploymentId with %s"), *DeploymentIdOverride);
	}
	if(FParse::Value(FCommandLine::Get(), TEXT("epicsandboxid="), SandboxIdOverride))
	{
		UE_LOG(LogEOSIntegrationKit, Log, TEXT("Overriding SandboxId with %s"), *SandboxIdOverride);
	}
	FString ArtifactNameOverride;
	// Figure out which config object we are loading
	FParse::Value(FCommandLine::Get(), TEXT("EOSArtifactNameOverride="), ArtifactNameOverride);
	if (ArtifactNameOverride.IsEmpty())
	{
		if(IsRunningDedicatedServer())
		{
			if(!This->DedicatedServerArtifactName.IsEmpty())
			{
				ArtifactNameOverride = This->DedicatedServerArtifactName;
			}
			else
			{
				ArtifactNameOverride = ArtifactName;
			}
		}
		else
		{
			ArtifactNameOverride = ArtifactName;
		}
	}
	for (const FEArtifactSettings& Artifact : This->Artifacts)
	{
		if (Artifact.ArtifactName == ArtifactNameOverride)
		{
			OutSettings = Artifact.ToNative();
			if(!DeploymentIdOverride.IsEmpty())
			{
				OutSettings.DeploymentId = DeploymentIdOverride;
			}
			if(!SandboxIdOverride.IsEmpty())
			{
				OutSettings.SandboxId = SandboxIdOverride;
			}
			return true;
		}
	}
	for (const FEArtifactSettings& Artifact : This->Artifacts)
	{
		if (Artifact.ArtifactName == This->DefaultArtifactName)
		{
			OutSettings = Artifact.ToNative();
			if(!DeploymentIdOverride.IsEmpty())
			{
				OutSettings.DeploymentId = DeploymentIdOverride;
			}
			if(!SandboxIdOverride.IsEmpty())
			{
				OutSettings.SandboxId = SandboxIdOverride;
			}
			return true;
		}
	}
	if(!ArtifactNameOverride.IsEmpty())
	{
		UE_LOG(LogEOSIntegrationKit, Error, TEXT("Failed to find artifact settings for %s"), *ArtifactNameOverride);
	}
	else
	{
		UE_LOG(LogEOSIntegrationKit, Error, TEXT("Failed to find artifact settings or the default artifact because the name was empty. Please check project settings under EOS Integration Kit"));
	}
	return false;
}

#if WITH_EDITOR
void UEIKSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FString EngineIniPath = FPaths::ProjectConfigDir() / TEXT("DefaultEngine.ini");
	{
		/*FEOSArtifactSettings OutSettings;
		if(!PlatformSpecificArtifactName.IsEmpty() && GetSettingsForArtifact(PlatformSpecificArtifactName, OutSettings))
		{
			ClientId = OutSettings.ClientId;
			ClientSecret = OutSettings.ClientSecret;
			ProductId = OutSettings.ProductId;
			SandboxId = OutSettings.SandboxId;
			DeploymentId = OutSettings.DeploymentId;
			SaveConfig();
			if(GConfig)
			{
				GConfig->Flush(false, EngineIniPath);
			}
		}*/
	}
	if (PropertyChangedEvent.Property == nullptr)
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		return;
	}	

	// Turning off the overlay in general turns off the social overlay too
	if (PropertyChangedEvent.Property->GetFName() == FName(TEXT("bEnableOverlay")))
	{
		if (!bEnableOverlay)
		{
			bEnableSocialOverlay = false;
			bEnableEditorOverlay = false;
		}
	}

	// Turning on the social overlay requires the base overlay too
	if (PropertyChangedEvent.Property->GetFName() == FName(TEXT("bEnableSocialOverlay")))
	{
		if (bEnableSocialOverlay)
		{
			bEnableOverlay = true;
		}
	}

	if (PropertyChangedEvent.MemberProperty != nullptr &&
		PropertyChangedEvent.MemberProperty->GetFName() == FName(TEXT("Artifacts")) &&
		(PropertyChangedEvent.ChangeType & EPropertyChangeType::ValueSet))
	{
		// Loop through all entries validating them
		for (FEArtifactSettings& Artifact : Artifacts)
		{
			if (!Artifact.ClientId.IsEmpty())
			{
				if (!Artifact.ClientId.StartsWith(TEXT("xyz")))
				{
					FMessageDialog::Open(EAppMsgType::Ok,
						LOCTEXT("ClientIdInvalidMsg", "Client ids created after SDK version 1.5 start with xyz. Double check that you did not use your BPT Client Id instead."));
				}
				if (!IsAnsi(Artifact.ClientId) || ContainsWhitespace(Artifact.ClientId))
				{
					FMessageDialog::Open(EAppMsgType::Ok,
						LOCTEXT("ClientIdNotAnsiMsg", "Client ids must contain ANSI printable characters only with no whitespace"));
					Artifact.ClientId.Empty();
				}
			}

			if (!Artifact.ClientSecret.IsEmpty())
			{
				if (!IsAnsi(Artifact.ClientSecret) || ContainsWhitespace(Artifact.ClientSecret))
				{
					FMessageDialog::Open(EAppMsgType::Ok,
						LOCTEXT("ClientSecretNotAnsiMsg", "ClientSecret must contain ANSI printable characters only with no whitespace"));
					Artifact.ClientSecret.Empty();
				}
			}

			if (!Artifact.EncryptionKey.IsEmpty())
			{
				if (!IsHex(Artifact.EncryptionKey) || Artifact.EncryptionKey.Len() != 64)
				{
					FMessageDialog::Open(EAppMsgType::Ok,
						LOCTEXT("EncryptionKeyNotHexMsg", "EncryptionKey must contain 64 hex characters"));
					Artifact.EncryptionKey.Empty();
				}
			}
		}
	}

	// Turning off EAS disables presence mirroring too
	if (PropertyChangedEvent.Property->GetFName() == FName(TEXT("bUseEAS")))
	{
		if (!bUseEAS)
		{
			bMirrorPresenceToEAS = false;
		}
	}

	if(PropertyChangedEvent.Property->GetFName()==FName(TEXT("bAutomaticallySetupEIK")))
	{
		if(bAutomaticallySetupEIK)
		{
			FString EngineIniText;
			if (FFileHelper::LoadFileToString(EngineIniText, *EngineIniPath))
			{
				bool bConfigChanged = false;

				// Check if [OnlineSubsystemEIK] section exists and add it if not
				if (!EngineIniText.Contains(TEXT("[OnlineSubsystemEIK]")))
				{
					EngineIniText += TEXT("\n[OnlineSubsystemEIK]\nbEnabled=true\n");
					bConfigChanged = true;
				}

				// Update [OnlineSubsystem] section
				if (!EngineIniText.Contains(TEXT("[OnlineSubsystem]")))
				{
					EngineIniText += TEXT("\n[OnlineSubsystem]\nDefaultPlatformService=EIK\n");
					bConfigChanged = true;
				}

				// Update [/Script/OnlineSubsystemEIK.NetDriverEIK] section
				if (!EngineIniText.Contains(TEXT("[/Script/OnlineSubsystemEIK.NetDriverEIK]")))
				{
					FString Comment = TEXT("\n;EIK Comment: You do not need to worry about this setting as we dynamically set it in Travel URL depending upon if we are using Listen Server or Dedicated Server\n");
					EngineIniText += Comment;
					EngineIniText += TEXT("[/Script/OnlineSubsystemEIK.NetDriverEIK]\nbIsUsingP2PSockets=true\n");
					bConfigChanged = true;
				}

				// Update [/Script/Engine.GameEngine] section
				if (!EngineIniText.Contains(TEXT("[/Script/Engine.GameEngine]")))
				{
					EngineIniText += TEXT("\n[/Script/Engine.GameEngine]\n");

					// Update NetDriverDefinitions in [/Script/Engine.GameEngine] section
					FString NetDriverDefinitions = FString::Printf(
						TEXT("!NetDriverDefinitions=ClearArray\n+NetDriverDefinitions=(DefName=\"GameNetDriver\",DriverClassName=\"OnlineSubsystemEIK.NetDriverEIK\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")\n")
					);
					EngineIniText += NetDriverDefinitions;
            
					bConfigChanged = true;
				}

				// Save the modified text back to the DefaultEngine.ini file if any changes were made
				if (bConfigChanged)
				{
					if (FFileHelper::SaveStringToFile(EngineIniText, *EngineIniPath))
					{
						UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystemEIK configuration added/updated in DefaultEngine.ini"));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to save modified DefaultEngine.ini"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystemEIK configuration already exists in DefaultEngine.ini"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load DefaultEngine.ini"));
			}
		}

		if(EAppReturnType::Ok == ShowRestartWarning(FText::FromString("Restart Editor")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Restart Editor"));
			FUnrealEdMisc::Get().RestartEditor(true);
		}
	}

	// Turning on presence requires EAS
	if (PropertyChangedEvent.Property->GetFName() == FName(TEXT("bMirrorPresenceToEAS")))
	{
		if (bMirrorPresenceToEAS)
		{
			bUseEAS = true;
		}
	}

	// Turning off EAS disables presence mirroring too
	if (PropertyChangedEvent.Property->GetFName() == FName(TEXT("bUseEOSConnect")))
	{
		if (!bUseEOSConnect)
		{
			bMirrorAchievementsToEOS = false;
			bMirrorStatsToEOS = false;
			bUseEOSSessions = false;
		}
	}

	// These all require EOS turned on if they are on
	if (PropertyChangedEvent.Property->GetFName() == FName(TEXT("bMirrorAchievementsToEOS")) ||
		PropertyChangedEvent.Property->GetFName() == FName(TEXT("bMirrorStatsToEOS")) ||
		PropertyChangedEvent.Property->GetFName() == FName(TEXT("bUseEOSSessions")))
	{
		if (bMirrorAchievementsToEOS || bMirrorStatsToEOS || bUseEOSSessions)
		{
			bUseEOSConnect = true;
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

#undef LOCTEXT_NAMESPACE

