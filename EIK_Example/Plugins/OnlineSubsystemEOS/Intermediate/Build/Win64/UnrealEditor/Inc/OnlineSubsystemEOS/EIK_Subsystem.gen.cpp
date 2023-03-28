// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OnlineSubsystemEOS/Subsystem/EIK_Subsystem.h"
#include "Engine/Classes/Engine/GameInstance.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEIK_Subsystem() {}
// Cross Module References
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UEIK_Subsystem_NoRegister();
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UEIK_Subsystem();
	ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
	UPackage* Z_Construct_UPackage__Script_OnlineSubsystemEOS();
// End Cross Module References
	void UEIK_Subsystem::StaticRegisterNativesUEIK_Subsystem()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEIK_Subsystem);
	UClass* Z_Construct_UClass_UEIK_Subsystem_NoRegister()
	{
		return UEIK_Subsystem::StaticClass();
	}
	struct Z_Construct_UClass_UEIK_Subsystem_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UEIK_Subsystem_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
		(UObject* (*)())Z_Construct_UPackage__Script_OnlineSubsystemEOS,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEIK_Subsystem_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "Subsystem/EIK_Subsystem.h" },
		{ "ModuleRelativePath", "Subsystem/EIK_Subsystem.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UEIK_Subsystem_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEIK_Subsystem>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UEIK_Subsystem_Statics::ClassParams = {
		&UEIK_Subsystem::StaticClass,
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
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UEIK_Subsystem_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UEIK_Subsystem_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UEIK_Subsystem()
	{
		if (!Z_Registration_Info_UClass_UEIK_Subsystem.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEIK_Subsystem.OuterSingleton, Z_Construct_UClass_UEIK_Subsystem_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UEIK_Subsystem.OuterSingleton;
	}
	template<> ONLINESUBSYSTEMEOS_API UClass* StaticClass<UEIK_Subsystem>()
	{
		return UEIK_Subsystem::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UEIK_Subsystem);
	struct Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Subsystem_EIK_Subsystem_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Subsystem_EIK_Subsystem_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UEIK_Subsystem, UEIK_Subsystem::StaticClass, TEXT("UEIK_Subsystem"), &Z_Registration_Info_UClass_UEIK_Subsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEIK_Subsystem), 735613340U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Subsystem_EIK_Subsystem_h_3384254253(TEXT("/Script/OnlineSubsystemEOS"),
		Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Subsystem_EIK_Subsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Subsystem_EIK_Subsystem_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
