// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "LLMManager.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeLLMManager() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor();
LLMTEST_API UClass* Z_Construct_UClass_ALLMManager();
LLMTEST_API UClass* Z_Construct_UClass_ALLMManager_NoRegister();
UPackage* Z_Construct_UPackage__Script_llmtest();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ALLMManager **************************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_ALLMManager;
UClass* ALLMManager::GetPrivateStaticClass()
{
	using TClass = ALLMManager;
	if (!Z_Registration_Info_UClass_ALLMManager.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("LLMManager"),
			Z_Registration_Info_UClass_ALLMManager.InnerSingleton,
			StaticRegisterNativesALLMManager,
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
	return Z_Registration_Info_UClass_ALLMManager.InnerSingleton;
}
UClass* Z_Construct_UClass_ALLMManager_NoRegister()
{
	return ALLMManager::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ALLMManager_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "LLMManager.h" },
		{ "ModuleRelativePath", "LLMManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class ALLMManager constinit property declarations ******************************
// ********** End Class ALLMManager constinit property declarations ********************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ALLMManager>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_ALLMManager_Statics
UObject* (*const Z_Construct_UClass_ALLMManager_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_llmtest,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ALLMManager_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ALLMManager_Statics::ClassParams = {
	&ALLMManager::StaticClass,
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
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ALLMManager_Statics::Class_MetaDataParams), Z_Construct_UClass_ALLMManager_Statics::Class_MetaDataParams)
};
void ALLMManager::StaticRegisterNativesALLMManager()
{
}
UClass* Z_Construct_UClass_ALLMManager()
{
	if (!Z_Registration_Info_UClass_ALLMManager.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ALLMManager.OuterSingleton, Z_Construct_UClass_ALLMManager_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ALLMManager.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, ALLMManager);
ALLMManager::~ALLMManager() {}
// ********** End Class ALLMManager ****************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_LLMManager_h__Script_llmtest_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ALLMManager, ALLMManager::StaticClass, TEXT("ALLMManager"), &Z_Registration_Info_UClass_ALLMManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ALLMManager), 4095713312U) },
	};
}; // Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_LLMManager_h__Script_llmtest_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_LLMManager_h__Script_llmtest_3981437297{
	TEXT("/Script/llmtest"),
	Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_LLMManager_h__Script_llmtest_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_serverproject_llmtest_Source_llmtest_LLMManager_h__Script_llmtest_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
