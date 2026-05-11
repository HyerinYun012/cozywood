#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../ItemData/ItemDataStructs.h"
#include "GardenFarmingComponent.generated.h"

class AC1Character;
class UItemBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COZYWOOD_API UGardenFarmingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGardenFarmingComponent();

	// 씨앗 심기
	void StartPlanting(UItemBase* SeedItem);

	UFUNCTION(BlueprintCallable, Category = "Farming")
	void NotifyDayPassed();

	void SavePlants();

	UPROPERTY(EditAnywhere, Category = "Farming")
	TSubclassOf<class APlantBase> PlantActorClass;

	UPROPERTY()
	int32 LastTrackedDay = -1;

protected:
	UPROPERTY()
	AC1Character* OwnerCharacter;

	virtual void BeginPlay() override;

private:
	void LoadPlants();
};