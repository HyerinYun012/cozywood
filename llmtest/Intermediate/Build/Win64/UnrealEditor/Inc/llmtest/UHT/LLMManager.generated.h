// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "LLMManager.h"

#ifdef LLMTEST_LLMManager_generated_h
#error "LLMManager.generated.h already included, missing '#pragma once' in LLMManager.h"
#endif
#define LLMTEST_LLMManager_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class ALLMManager **************************************************************
struct Z_Construct_UClass_ALLMManager_Statics;
LLMTEST_API UClass* Z_Construct_UClass_ALLMManager_NoRegister();

#define FID_serverproject_llmtest_Source_llmtest_LLMManager_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesALLMManager(); \
	friend struct ::Z_Construct_UClass_ALLMManager_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend LLMTEST_API UClass* ::Z_Construct_UClass_ALLMManager_NoRegister(); \
public: \
	DECLARE_CLASS2(ALLMManager, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/llmtest"), Z_Construct_UClass_ALLMManager_NoRegister) \
	DECLARE_SERIALIZER(ALLMManager)


#define FID_serverproject_llmtest_Source_llmtest_LLMManager_h_14_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ALLMManager(ALLMManager&&) = delete; \
	ALLMManager(const ALLMManager&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ALLMManager); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ALLMManager); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ALLMManager) \
	NO_API virtual ~ALLMManager();


#define FID_serverproject_llmtest_Source_llmtest_LLMManager_h_11_PROLOG
#define FID_serverproject_llmtest_Source_llmtest_LLMManager_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_serverproject_llmtest_Source_llmtest_LLMManager_h_14_INCLASS_NO_PURE_DECLS \
	FID_serverproject_llmtest_Source_llmtest_LLMManager_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ALLMManager;

// ********** End Class ALLMManager ****************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_serverproject_llmtest_Source_llmtest_LLMManager_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
