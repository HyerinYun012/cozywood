// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodellmtest_init() {}
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");	LLMTEST_API UFunction* Z_Construct_UDelegateFunction_llmtest_OnEnemyDied__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_llmtest;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_llmtest()
	{
		if (!Z_Registration_Info_UPackage__Script_llmtest.OuterSingleton)
		{
		static UObject* (*const SingletonFuncArray[])() = {
			(UObject* (*)())Z_Construct_UDelegateFunction_llmtest_OnEnemyDied__DelegateSignature,
		};
		static const UECodeGen_Private::FPackageParams PackageParams = {
			"/Script/llmtest",
			SingletonFuncArray,
			UE_ARRAY_COUNT(SingletonFuncArray),
			PKG_CompiledIn | 0x00000000,
			0x743FDF2E,
			0xBF4609C4,
			METADATA_PARAMS(0, nullptr)
		};
		UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_llmtest.OuterSingleton, PackageParams);
	}
	return Z_Registration_Info_UPackage__Script_llmtest.OuterSingleton;
}
static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_llmtest(Z_Construct_UPackage__Script_llmtest, TEXT("/Script/llmtest"), Z_Registration_Info_UPackage__Script_llmtest, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x743FDF2E, 0xBF4609C4));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
