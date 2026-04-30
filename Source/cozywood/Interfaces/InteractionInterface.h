#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AC1Character;

UENUM(BlueprintType)
enum class EItemToolType : uint8
{
	EITT_None UMETA(DisplayName = "None"),
	EITT_WateringCan UMETA(DisplayName = "Watering Can"),
	EITT_Shovel UMETA(DisplayName = "Shovel")
};

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_Pickup UMETA(DisplayName = "Pickup"),
	EIT_Dialogue UMETA(DisplayName = "Dialogue"),
	EIT_Container UMETA(DisplayName = "Container"),
	EIT_Usable UMETA(DisplayName = "Usable")
};

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_BODY()

	FInteractableData() :
		InteractableType(EInteractableType::EIT_Pickup),
		Name(FText::GetEmpty()),
		Action(FText::GetEmpty()),
		Quantity(0),
		InteractionDuration(0.0f)
	{
	}

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	// used only for pickups
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	// used for interactions that require time (e.g., harvesting, crafting)
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COZYWOOD_API IInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void BeginFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void BeginInteract(EItemToolType EquippedTool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AC1Character* PlayerCharacter);

	virtual FInteractableData GetInteractableData() const = 0;
};
