// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "llmtestCharacter.h"

#ifdef LLMTEST_llmtestCharacter_generated_h
#error "llmtestCharacter.generated.h already included, missing '#pragma once' in llmtestCharacter.h"
#endif
#define LLMTEST_llmtestCharacter_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AllmtestCharacter ********************************************************
#define FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execDoJumpEnd); \
	DECLARE_FUNCTION(execDoJumpStart); \
	DECLARE_FUNCTION(execDoLook); \
	DECLARE_FUNCTION(execDoMove);


struct Z_Construct_UClass_AllmtestCharacter_Statics;
LLMTEST_API UClass* Z_Construct_UClass_AllmtestCharacter_NoRegister();

#define FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_24_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAllmtestCharacter(); \
	friend struct ::Z_Construct_UClass_AllmtestCharacter_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend LLMTEST_API UClass* ::Z_Construct_UClass_AllmtestCharacter_NoRegister(); \
public: \
	DECLARE_CLASS2(AllmtestCharacter, ACharacter, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/llmtest"), Z_Construct_UClass_AllmtestCharacter_NoRegister) \
	DECLARE_SERIALIZER(AllmtestCharacter)


#define FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_24_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AllmtestCharacter(AllmtestCharacter&&) = delete; \
	AllmtestCharacter(const AllmtestCharacter&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AllmtestCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AllmtestCharacter); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(AllmtestCharacter) \
	NO_API virtual ~AllmtestCharacter();


#define FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_21_PROLOG
#define FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_24_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_24_INCLASS_NO_PURE_DECLS \
	FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h_24_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AllmtestCharacter;

// ********** End Class AllmtestCharacter **********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_serverproject_llmtest_Source_llmtest_llmtestCharacter_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
