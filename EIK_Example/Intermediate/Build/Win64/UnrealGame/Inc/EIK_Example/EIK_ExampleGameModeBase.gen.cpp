// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EIK_Example/EIK_ExampleGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEIK_ExampleGameModeBase() {}
// Cross Module References
	EIK_EXAMPLE_API UClass* Z_Construct_UClass_AEIK_ExampleGameModeBase_NoRegister();
	EIK_EXAMPLE_API UClass* Z_Construct_UClass_AEIK_ExampleGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_EIK_Example();
// End Cross Module References
	void AEIK_ExampleGameModeBase::StaticRegisterNativesAEIK_ExampleGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AEIK_ExampleGameModeBase);
	UClass* Z_Construct_UClass_AEIK_ExampleGameModeBase_NoRegister()
	{
		return AEIK_ExampleGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_EIK_Example,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "EIK_ExampleGameModeBase.h" },
		{ "ModuleRelativePath", "EIK_ExampleGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AEIK_ExampleGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics::ClassParams = {
		&AEIK_ExampleGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AEIK_ExampleGameModeBase()
	{
		if (!Z_Registration_Info_UClass_AEIK_ExampleGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AEIK_ExampleGameModeBase.OuterSingleton, Z_Construct_UClass_AEIK_ExampleGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AEIK_ExampleGameModeBase.OuterSingleton;
	}
	template<> EIK_EXAMPLE_API UClass* StaticClass<AEIK_ExampleGameModeBase>()
	{
		return AEIK_ExampleGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AEIK_ExampleGameModeBase);
	struct Z_CompiledInDeferFile_FID_EIK_Example_Source_EIK_Example_EIK_ExampleGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Source_EIK_Example_EIK_ExampleGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AEIK_ExampleGameModeBase, AEIK_ExampleGameModeBase::StaticClass, TEXT("AEIK_ExampleGameModeBase"), &Z_Registration_Info_UClass_AEIK_ExampleGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AEIK_ExampleGameModeBase), 3405511359U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Source_EIK_Example_EIK_ExampleGameModeBase_h_2319416570(TEXT("/Script/EIK_Example"),
		Z_CompiledInDeferFile_FID_EIK_Example_Source_EIK_Example_EIK_ExampleGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_EIK_Example_Source_EIK_Example_EIK_ExampleGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
