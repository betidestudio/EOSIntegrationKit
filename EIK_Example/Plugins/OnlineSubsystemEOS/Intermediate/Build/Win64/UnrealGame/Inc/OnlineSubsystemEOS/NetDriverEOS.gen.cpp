// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OnlineSubsystemEOS/Private/NetDriverEOS.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNetDriverEOS() {}
// Cross Module References
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UNetDriverEOS_NoRegister();
	ONLINESUBSYSTEMEOS_API UClass* Z_Construct_UClass_UNetDriverEOS();
	SOCKETSUBSYSTEMEOS_API UClass* Z_Construct_UClass_UNetDriverEOSBase();
	UPackage* Z_Construct_UPackage__Script_OnlineSubsystemEOS();
// End Cross Module References
	void UNetDriverEOS::StaticRegisterNativesUNetDriverEOS()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UNetDriverEOS);
	UClass* Z_Construct_UClass_UNetDriverEOS_NoRegister()
	{
		return UNetDriverEOS::StaticClass();
	}
	struct Z_Construct_UClass_UNetDriverEOS_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UNetDriverEOS_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UNetDriverEOSBase,
		(UObject* (*)())Z_Construct_UPackage__Script_OnlineSubsystemEOS,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UNetDriverEOS_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "NetDriverEOS.h" },
		{ "ModuleRelativePath", "Private/NetDriverEOS.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UNetDriverEOS_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNetDriverEOS>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UNetDriverEOS_Statics::ClassParams = {
		&UNetDriverEOS::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000ACu,
		METADATA_PARAMS(Z_Construct_UClass_UNetDriverEOS_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UNetDriverEOS_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UNetDriverEOS()
	{
		if (!Z_Registration_Info_UClass_UNetDriverEOS.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNetDriverEOS.OuterSingleton, Z_Construct_UClass_UNetDriverEOS_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UNetDriverEOS.OuterSingleton;
	}
	template<> ONLINESUBSYSTEMEOS_API UClass* StaticClass<UNetDriverEOS>()
	{
		return UNetDriverEOS::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UNetDriverEOS);
	struct Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Private_NetDriverEOS_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Private_NetDriverEOS_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UNetDriverEOS, UNetDriverEOS::StaticClass, TEXT("UNetDriverEOS"), &Z_Registration_Info_UClass_UNetDriverEOS, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNetDriverEOS), 328345477U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Private_NetDriverEOS_h_673099845(TEXT("/Script/OnlineSubsystemEOS"),
		Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Private_NetDriverEOS_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_EIK_Example_Plugins_OnlineSubsystemEOS_Source_OnlineSubsystemEOS_Private_NetDriverEOS_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
