// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OnlineSubsystemEOS/Public/EOSSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEOSSettings() {}
// Cross Module References
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_NoRegister();
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings();
	ENGINE_API UClass* Z_Construct_UClass_UDataAsset();
	UPackage* Z_Construct_UPackage__Script_OnlineSubsystemEOS();
	ONLINESUBSYSTEMEOS_API UScriptStruct* Z_Construct_UScriptStruct_FArtifactSettings();
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UEOSSettings_NoRegister();
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UEOSSettings();
	ENGINE_API UClass* Z_Construct_UClass_URuntimeOptionsBase();
// End Cross Module References
	void UDEPRECATED_EOSArtifactSettings::StaticRegisterNativesUDEPRECATED_EOSArtifactSettings()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UDEPRECATED_EOSArtifactSettings);
	UClass* Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_NoRegister()
	{
		return UDEPRECATED_EOSArtifactSettings::StaticClass();
	}
	struct Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UDataAsset,
		(UObject* (*)())Z_Construct_UPackage__Script_OnlineSubsystemEOS,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "EOSSettings.h" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDEPRECATED_EOSArtifactSettings>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics::ClassParams = {
		&UDEPRECATED_EOSArtifactSettings::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x020002A0u,
		METADATA_PARAMS(Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings()
	{
		if (!Z_Registration_Info_UClass_UDEPRECATED_EOSArtifactSettings.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDEPRECATED_EOSArtifactSettings.OuterSingleton, Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UDEPRECATED_EOSArtifactSettings.OuterSingleton;
	}
	template<> ONLINESUBSYSTEMEOS_API UClass* StaticClass<UDEPRECATED_EOSArtifactSettings>()
	{
		return UDEPRECATED_EOSArtifactSettings::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UDEPRECATED_EOSArtifactSettings);
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_ArtifactSettings;
class UScriptStruct* FArtifactSettings::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_ArtifactSettings.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_ArtifactSettings.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FArtifactSettings, Z_Construct_UPackage__Script_OnlineSubsystemEOS(), TEXT("ArtifactSettings"));
	}
	return Z_Registration_Info_UScriptStruct_ArtifactSettings.OuterSingleton;
}
template<> ONLINESUBSYSTEMEOS_API UScriptStruct* StaticStruct<FArtifactSettings>()
{
	return FArtifactSettings::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FArtifactSettings_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ArtifactName_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ArtifactName;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClientId_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ClientId;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClientSecret_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ClientSecret;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ProductId_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ProductId;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SandboxId_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_SandboxId;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DeploymentId_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_DeploymentId;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EncryptionKey_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_EncryptionKey;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FArtifactSettings>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ArtifactName_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** This needs to match what the launcher passes in the -epicapp command line arg */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "This needs to match what the launcher passes in the -epicapp command line arg" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ArtifactName = { "ArtifactName", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FArtifactSettings, ArtifactName), METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ArtifactName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ArtifactName_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientId_MetaData[] = {
		{ "Category", "EOS Artifact Settings" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientId = { "ClientId", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FArtifactSettings, ClientId), METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientId_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientSecret_MetaData[] = {
		{ "Category", "EOS Artifact Settings" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientSecret = { "ClientSecret", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FArtifactSettings, ClientSecret), METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientSecret_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientSecret_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ProductId_MetaData[] = {
		{ "Category", "EOS Artifact Settings" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ProductId = { "ProductId", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FArtifactSettings, ProductId), METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ProductId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ProductId_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_SandboxId_MetaData[] = {
		{ "Category", "EOS Artifact Settings" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_SandboxId = { "SandboxId", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FArtifactSettings, SandboxId), METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_SandboxId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_SandboxId_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_DeploymentId_MetaData[] = {
		{ "Category", "EOS Artifact Settings" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_DeploymentId = { "DeploymentId", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FArtifactSettings, DeploymentId), METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_DeploymentId_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_DeploymentId_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_EncryptionKey_MetaData[] = {
		{ "Category", "EOS Artifact Settings" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_EncryptionKey = { "EncryptionKey", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FArtifactSettings, EncryptionKey), METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_EncryptionKey_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_EncryptionKey_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FArtifactSettings_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ArtifactName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientId,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ClientSecret,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_ProductId,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_SandboxId,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_DeploymentId,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewProp_EncryptionKey,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FArtifactSettings_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_OnlineSubsystemEOS,
		nullptr,
		&NewStructOps,
		"ArtifactSettings",
		sizeof(FArtifactSettings),
		alignof(FArtifactSettings),
		Z_Construct_UScriptStruct_FArtifactSettings_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FArtifactSettings_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FArtifactSettings_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FArtifactSettings()
	{
		if (!Z_Registration_Info_UScriptStruct_ArtifactSettings.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_ArtifactSettings.InnerSingleton, Z_Construct_UScriptStruct_FArtifactSettings_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_ArtifactSettings.InnerSingleton;
	}
	void UEOSSettings::StaticRegisterNativesUEOSSettings()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEOSSettings);
	UClass* Z_Construct_UClass_UEOSSettings_NoRegister()
	{
		return UEOSSettings::StaticClass();
	}
	struct Z_Construct_UClass_UEOSSettings_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CacheDir_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_CacheDir;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DefaultArtifactName_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_DefaultArtifactName;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TickBudgetInMilliseconds_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_TickBudgetInMilliseconds;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bEnableOverlay_MetaData[];
#endif
		static void NewProp_bEnableOverlay_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableOverlay;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bEnableSocialOverlay_MetaData[];
#endif
		static void NewProp_bEnableSocialOverlay_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableSocialOverlay;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bShouldEnforceBeingLaunchedByEGS_MetaData[];
#endif
		static void NewProp_bShouldEnforceBeingLaunchedByEGS_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bShouldEnforceBeingLaunchedByEGS;
		static const UECodeGen_Private::FStrPropertyParams NewProp_TitleStorageTags_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TitleStorageTags_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_TitleStorageTags;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TitleStorageReadChunkLength_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_TitleStorageReadChunkLength;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Artifacts_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Artifacts_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_Artifacts;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bUseEAS_MetaData[];
#endif
		static void NewProp_bUseEAS_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseEAS;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bUseEOSConnect_MetaData[];
#endif
		static void NewProp_bUseEOSConnect_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseEOSConnect;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bMirrorStatsToEOS_MetaData[];
#endif
		static void NewProp_bMirrorStatsToEOS_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bMirrorStatsToEOS;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bMirrorAchievementsToEOS_MetaData[];
#endif
		static void NewProp_bMirrorAchievementsToEOS_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bMirrorAchievementsToEOS;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bUseEOSSessions_MetaData[];
#endif
		static void NewProp_bUseEOSSessions_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseEOSSessions;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bMirrorPresenceToEAS_MetaData[];
#endif
		static void NewProp_bMirrorPresenceToEAS_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bMirrorPresenceToEAS;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UEOSSettings_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_URuntimeOptionsBase,
		(UObject* (*)())Z_Construct_UPackage__Script_OnlineSubsystemEOS,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "EOSSettings.h" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_CacheDir_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/**\n\x09 * The directory any PDS/TDS files are cached into. This is per artifact e.g.:\n\x09 *\n\x09 * <UserDir>/<ArtifactId>/<CacheDir>\n\x09 */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "The directory any PDS/TDS files are cached into. This is per artifact e.g.:\n\n<UserDir>/<ArtifactId>/<CacheDir>" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_CacheDir = { "CacheDir", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UEOSSettings, CacheDir), METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_CacheDir_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_CacheDir_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_DefaultArtifactName_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Used when launched from a store other than EGS or when the specified artifact name was not present */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Used when launched from a store other than EGS or when the specified artifact name was not present" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_DefaultArtifactName = { "DefaultArtifactName", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UEOSSettings, DefaultArtifactName), METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_DefaultArtifactName_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_DefaultArtifactName_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_TickBudgetInMilliseconds_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Used to throttle how much time EOS ticking can take */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Used to throttle how much time EOS ticking can take" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_TickBudgetInMilliseconds = { "TickBudgetInMilliseconds", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UEOSSettings, TickBudgetInMilliseconds), METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_TickBudgetInMilliseconds_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_TickBudgetInMilliseconds_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableOverlay_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Set to true to enable the overlay (ecom features) */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to enable the overlay (ecom features)" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableOverlay_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bEnableOverlay = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableOverlay = { "bEnableOverlay", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableOverlay_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableOverlay_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableOverlay_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableSocialOverlay_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Set to true to enable the social overlay (friends, invites, etc.) */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to enable the social overlay (friends, invites, etc.)" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableSocialOverlay_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bEnableSocialOverlay = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableSocialOverlay = { "bEnableSocialOverlay", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableSocialOverlay_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableSocialOverlay_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableSocialOverlay_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bShouldEnforceBeingLaunchedByEGS_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Set to true to enable the social overlay (friends, invites, etc.) */" },
		{ "DisplayName", "Require Being Launched by the Epic Games Store" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to enable the social overlay (friends, invites, etc.)" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bShouldEnforceBeingLaunchedByEGS_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bShouldEnforceBeingLaunchedByEGS = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bShouldEnforceBeingLaunchedByEGS = { "bShouldEnforceBeingLaunchedByEGS", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bShouldEnforceBeingLaunchedByEGS_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bShouldEnforceBeingLaunchedByEGS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bShouldEnforceBeingLaunchedByEGS_MetaData)) };
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageTags_Inner = { "TitleStorageTags", nullptr, (EPropertyFlags)0x0000000000004000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageTags_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Tag combinations for paged queries in title file enumerations, separate tags within groups using `+` */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Tag combinations for paged queries in title file enumerations, separate tags within groups using `+`" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageTags = { "TitleStorageTags", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UEOSSettings, TitleStorageTags), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageTags_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageTags_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageReadChunkLength_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Chunk size used when reading a title file */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Chunk size used when reading a title file" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageReadChunkLength = { "TitleStorageReadChunkLength", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UEOSSettings, TitleStorageReadChunkLength), METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageReadChunkLength_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageReadChunkLength_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_Artifacts_Inner = { "Artifacts", nullptr, (EPropertyFlags)0x0000000000004000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FArtifactSettings, METADATA_PARAMS(nullptr, 0) }; // 1985851846
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_Artifacts_MetaData[] = {
		{ "Category", "EOS Settings" },
		{ "Comment", "/** Per artifact SDK settings. A game might have a FooStaging, FooQA, and public Foo artifact */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Per artifact SDK settings. A game might have a FooStaging, FooQA, and public Foo artifact" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_Artifacts = { "Artifacts", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UEOSSettings, Artifacts), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_Artifacts_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_Artifacts_MetaData)) }; // 1985851846
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEAS_MetaData[] = {
		{ "Category", "EOSPlus Login Settings" },
		{ "Comment", "/** Set to true to have Epic Accounts used (friends list will be unified with the default platform) */" },
		{ "DisplayName", "Use Epic Account for EOS login (requires account linking)" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to have Epic Accounts used (friends list will be unified with the default platform)" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEAS_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bUseEAS = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEAS = { "bUseEAS", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEAS_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEAS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEAS_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSConnect_MetaData[] = {
		{ "Category", "EOSPlus Login Settings" },
		{ "Comment", "/** Set to true to have EOS Connect APIs used to link accounts for crossplay */" },
		{ "DisplayName", "Use Crossplatform User IDs for EOS Login (doesn't use Epic Account)" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to have EOS Connect APIs used to link accounts for crossplay" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSConnect_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bUseEOSConnect = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSConnect = { "bUseEOSConnect", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSConnect_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSConnect_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSConnect_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorStatsToEOS_MetaData[] = {
		{ "Category", "Crossplay Settings" },
		{ "Comment", "/** Set to true to write stats to EOS as well as the default platform */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to write stats to EOS as well as the default platform" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorStatsToEOS_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bMirrorStatsToEOS = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorStatsToEOS = { "bMirrorStatsToEOS", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorStatsToEOS_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorStatsToEOS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorStatsToEOS_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorAchievementsToEOS_MetaData[] = {
		{ "Category", "Crossplay Settings" },
		{ "Comment", "/** Set to true to write achievement data to EOS as well as the default platform */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to write achievement data to EOS as well as the default platform" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorAchievementsToEOS_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bMirrorAchievementsToEOS = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorAchievementsToEOS = { "bMirrorAchievementsToEOS", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorAchievementsToEOS_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorAchievementsToEOS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorAchievementsToEOS_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSSessions_MetaData[] = {
		{ "Category", "Crossplay Settings" },
		{ "Comment", "/** Set to true to use EOS for session registration with data mirrored to the default platform */" },
		{ "DisplayName", "Use Crossplay Sessions" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to use EOS for session registration with data mirrored to the default platform" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSSessions_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bUseEOSSessions = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSSessions = { "bUseEOSSessions", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSSessions_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSSessions_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSSessions_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorPresenceToEAS_MetaData[] = {
		{ "Category", "Crossplay Settings" },
		{ "Comment", "/** Set to true to have Epic Accounts presence information updated when the default platform is updated */" },
		{ "ModuleRelativePath", "Public/EOSSettings.h" },
		{ "ToolTip", "Set to true to have Epic Accounts presence information updated when the default platform is updated" },
	};
#endif
	void Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorPresenceToEAS_SetBit(void* Obj)
	{
		((UEOSSettings*)Obj)->bMirrorPresenceToEAS = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorPresenceToEAS = { "bMirrorPresenceToEAS", nullptr, (EPropertyFlags)0x0010000000004015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UEOSSettings), &Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorPresenceToEAS_SetBit, METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorPresenceToEAS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorPresenceToEAS_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UEOSSettings_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_CacheDir,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_DefaultArtifactName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_TickBudgetInMilliseconds,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableOverlay,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bEnableSocialOverlay,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bShouldEnforceBeingLaunchedByEGS,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageTags_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageTags,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_TitleStorageReadChunkLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_Artifacts_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_Artifacts,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEAS,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSConnect,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorStatsToEOS,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorAchievementsToEOS,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bUseEOSSessions,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSSettings_Statics::NewProp_bMirrorPresenceToEAS,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UEOSSettings_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEOSSettings>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UEOSSettings_Statics::ClassParams = {
		&UEOSSettings::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UEOSSettings_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::PropPointers),
		0,
		0x001000A6u,
		METADATA_PARAMS(Z_Construct_UClass_UEOSSettings_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UEOSSettings_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UEOSSettings()
	{
		if (!Z_Registration_Info_UClass_UEOSSettings.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEOSSettings.OuterSingleton, Z_Construct_UClass_UEOSSettings_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UEOSSettings.OuterSingleton;
	}
	template<> ONLINESUBSYSTEMEOS_API UClass* StaticClass<UEOSSettings>()
	{
		return UEOSSettings::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UEOSSettings);
	struct Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_Statics::ScriptStructInfo[] = {
		{ FArtifactSettings::StaticStruct, Z_Construct_UScriptStruct_FArtifactSettings_Statics::NewStructOps, TEXT("ArtifactSettings"), &Z_Registration_Info_UScriptStruct_ArtifactSettings, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FArtifactSettings), 1985851846U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UDEPRECATED_EOSArtifactSettings, UDEPRECATED_EOSArtifactSettings::StaticClass, TEXT("UDEPRECATED_EOSArtifactSettings"), &Z_Registration_Info_UClass_UDEPRECATED_EOSArtifactSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDEPRECATED_EOSArtifactSettings), 2835852885U) },
		{ Z_Construct_UClass_UEOSSettings, UEOSSettings::StaticClass, TEXT("UEOSSettings"), &Z_Registration_Info_UClass_UEOSSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEOSSettings), 1851601777U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_3695692375(TEXT("/Script/OnlineSubsystemEOS"),
		Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Public_EOSSettings_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
