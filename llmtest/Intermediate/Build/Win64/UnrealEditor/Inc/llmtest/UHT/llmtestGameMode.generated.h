// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "llmtestGameMode.h"

#ifdef LLMTEST_llmtestGameMode_generated_h
#error "llmtestGameMode.generated.h already included, missing '#pragma once' in llmtestGameMode.h"
#endif
#define LLMTEST_llmtestGameMode_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AllmtestGameMode *********************************************************
struct Z_Construct_UClass_AllmtestGameMode_Statics;
LLMTEST_API UClass* Z_Construct_UClass_AllmtestGameMode_NoRegister();

#define FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAllmtestGameMode(); \
	friend struct ::Z_Construct_UClass_AllmtestGameMode_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend LLMTEST_API UClass* ::Z_Construct_UClass_AllmtestGameMode_NoRegister(); \
public: \
	DECLARE_CLASS2(AllmtestGameMode, AGameModeBase, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/llmtest"), Z_Construct_UClass_AllmtestGameMode_NoRegister) \
	DECLARE_SERIALIZER(AllmtestGameMode)


#define FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h_15_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AllmtestGameMode(AllmtestGameMode&&) = delete; \
	AllmtestGameMode(const AllmtestGameMode&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AllmtestGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AllmtestGameMode); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(AllmtestGameMode) \
	NO_API virtual ~AllmtestGameMode();


#define FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h_12_PROLOG
#define FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h_15_INCLASS_NO_PURE_DECLS \
	FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AllmtestGameMode;

// ********** End Class AllmtestGameMode ***********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_serverproject_llmtest_Source_llmtest_llmtestGameMode_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
