// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "llmtestGameMode.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodellmtestGameMode() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
LLMTEST_API UClass* Z_Construct_UClass_AllmtestGameMode();
LLMTEST_API UClass* Z_Construct_UClass_AllmtestGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_llmtest();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AllmtestGameMode *********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AllmtestGameMode;
UClass* AllmtestGameMode::GetPrivateStaticClass()
{
	using TClass = AllmtestGameMode;
	if (!Z_Registration_Info_UClass_AllmtestGameMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("llmtestGameMode"),
			Z_Registration_Info_UClass_AllmtestGameMode.InnerSingleton,
			StaticRegisterNativesAllmtestGameMode,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AllmtestGameMode.InnerSingleton;
}
UClass* Z_Construct_UClass_AllmtestGameMode_NoRegister()
{
	return AllmtestGameMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AllmtestGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  Simple GameMode for a third person game\n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "llmtestGameMode.h" },
		{ "ModuleRelativePath", "llmtestGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Simple GameMode for a third person game" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class AllmtestGameMode constinit property declarations *************************
// ********** End Class AllmtestGameMode constinit property declarations ***************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AllmtestGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AllmtestGameMode_Statics
UObject* (*const Z_Construct_UClass_AllmtestGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_llmtest,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AllmtestGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AllmtestGameMode_Statics::ClassParams = {
	&AllmtestGameMode::StaticClass,
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
	0x008002ADu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AllmtestGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AllmtestGameMode_Statics::Class_MetaDataParams)
};
void AllmtestGameMode::StaticRegisterNativesAllmtestGameMode()
{
}
UClass* Z_Construct_UClass_AllmtestGameMode()
{
	if (!Z_Registration_Info_UClass_AllmtestGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AllmtestGameMode.OuterSingleton, Z_Construct_UClass_AllmtestGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AllmtestGameMode.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AllmtestGameMode);
AllmtestGameMode::~AllmtestGameMode() {}
// ********** End Class AllmtestGameMode ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h__Script_llmtest_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AllmtestGameMode, AllmtestGameMode::StaticClass, TEXT("AllmtestGameMode"), &Z_Registration_Info_UClass_AllmtestGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AllmtestGameMode), 4179348877U) },
	};
}; // Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h__Script_llmtest_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h__Script_llmtest_2212946946{
	TEXT("/Script/llmtest"),
	Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h__Script_llmtest_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h__Script_llmtest_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
