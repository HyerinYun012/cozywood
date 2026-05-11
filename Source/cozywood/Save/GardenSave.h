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

USTRUCT(BlueprintType)
struct FSavedGardenSignData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GardenTitle;        // 정원 제목

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> EmotionKeywords;     // 감정 키워드 (버튼 클릭으로 들어갈 텍스트)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GardenDescription;  // 정원 설명

	// 저장된 시간을 기록할 타임스탬프
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime Timestamp;
};

// 인벤토리 아이템 하나당 기록할 내용 (ID, 수량)
USTRUCT(BlueprintType)
struct FSavedInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;
};

// 식물 하나당 기록할 내용
USTRUCT(BlueprintType)
struct FSavedPlantData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GrowthDay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWatered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LastSavedInGameDay = 0;
};

UCLASS()
class COZYWOOD_API UGardenSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	int32 SavedMoney = 1000; 

	// 가구 배치 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<FSavedFurnitureData> SavedFurnitures;

	// 인벤토리 아이템 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<FSavedInventoryItem> SavedInventoryItems;

	// 식물 배치 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<FSavedPlantData> SavedPlants;

	// 정원 표지판 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	FSavedGardenSignData GardenSignInfo;
};