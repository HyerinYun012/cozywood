// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "llmtestPlayerController.h"

#ifdef LLMTEST_llmtestPlayerController_generated_h
#error "llmtestPlayerController.generated.h already included, missing '#pragma once' in llmtestPlayerController.h"
#endif
#define LLMTEST_llmtestPlayerController_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AllmtestPlayerController *************************************************
struct Z_Construct_UClass_AllmtestPlayerController_Statics;
LLMTEST_API UClass* Z_Construct_UClass_AllmtestPlayerController_NoRegister();

#define FID_serverproject_llmtest_Source_llmtest_llmtestPlayerController_h_19_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAllmtestPlayerController(); \
	friend struct ::Z_Construct_UClass_AllmtestPlayerController_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend LLMTEST_API UClass* ::Z_Construct_UClass_AllmtestPlayerController_NoRegister(); \
public: \
	DECLARE_CLASS2(AllmtestPlayerController, APlayerController, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/llmtest"), Z_Construct_UClass_AllmtestPlayerController_NoRegister) \
	DECLARE_SERIALIZER(AllmtestPlayerController)


#define FID_serverproject_llmtest_Source_llmtest_llmtestPlayerController_h_19_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AllmtestPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	AllmtestPlayerController(AllmtestPlayerController&&) = delete; \
	AllmtestPlayerController(const AllmtestPlayerController&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AllmtestPlayerController); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AllmtestPlayerController); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AllmtestPlayerController) \
	NO_API virtual ~AllmtestPlayerController();


#define FID_serverproject_llmtest_Source_llmtest_llmtestPlayerController_h_16_PROLOG
#define FID_serverproject_llmtest_Source_llmtest_llmtestPlayerController_h_19_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_serverproject_llmtest_Source_llmtest_llmtestPlayerController_h_19_INCLASS_NO_PURE_DECLS \
	FID_serverproject_llmtest_Source_llmtest_llmtestPlayerController_h_19_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AllmtestPlayerController;

// ********** End Class AllmtestPlayerController ***************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_serverproject_llmtest_Source_llmtest_llmtestPlayerController_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
