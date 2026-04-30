#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GardenSave.generated.h"

// 가구 하나당 기록할 내용 (이름표, 위치, 회전값)
USTRUCT(BlueprintType)
struct FSavedFurnitureData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform FurnitureTransform;
};

UCLASS()
class COZYWOOD_API UGardenSave : public USaveGame
{
	GENERATED_BODY()

public:
	// 가구 데이터를 담을 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<FSavedFurnitureData> SavedFurnitures;
};