#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UDataTable;
class UItemBase;
class AC1Character;

UCLASS()
class COZYWOOD_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	APickup();

	virtual FInteractableData GetInteractableData() const override { return InstanceInteractableData; }

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; }

	virtual void BeginFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AC1Character* PlayerCharacter) override;
	void UpdateInteractableData();

	void TakePickup(const AC1Character* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
