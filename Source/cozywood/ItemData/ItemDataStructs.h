#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "../Interfaces/InteractionInterface.h"
#include "ItemDataStructs.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	None,
	Tool UMETA(DisplayName = "도구"),
	Furniture UMETA(DisplayName = "가구"),
	Seed UMETA(DisplayName = "씨앗 및 묘목"), // 씨앗 및 묘목 
	GrownPlant UMETA(DisplayName = "식물") // 다 자란 식물
};

UENUM()
enum class EPlantType : uint8 {
	None,
	Flower,
	Tree
};

USTRUCT(BlueprintType)
struct FItemTextData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name; // 아이템 이름 (예: "빨간 꽃 씨앗") 

	UPROPERTY(EditAnywhere)
	FText Description; // 아이템 설명 (예: "이 씨앗은 빨간 꽃이 자랍니다.") 

	UPROPERTY(EditAnywhere)
	FText InteractionText; // 상호작용 시 표시되는 텍스트 (예: "수확하려면 클릭하세요.") 

	UPROPERTY(EditAnywhere)
	FText UsageText; // 사용 방법에 대한 설명 (예: "씨앗을 심으려면 땅을 클릭하세요.") 
};

USTRUCT(BlueprintType)
struct FPlantData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPlantType PlantType; // 식물 유형 (꽃, 나무 등) 

	UPROPERTY(EditAnywhere)
	int32 GrowthDays; // 완전히 자라기까지 걸리는 일수 

	UPROPERTY(EditAnywhere) 
	TArray<UStaticMesh*> GrowthMeshes; // 성장 단계별로 사용할 메시 배열 (예: 묘목, 중간 단계, 완전히 자란 식물)
};

USTRUCT(BlueprintType)
struct FItemVisualData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	UTexture2D* Icon;

	// 인벤토리에서 버리거나 세상에 배치할 때 스폰될 실제 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSubclassOf<AActor> ActorClass;


	// 홀로그램에 입힐 스태틱 메시 (StartPlacement에서 사용 중)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	UStaticMesh* Mesh; // 아이템의 3D 모델
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase {
	GENERATED_BODY()

public:
	// 1. 이 아이템이 겹쳐지는 종류인가? (태생적 성질)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	bool bIsStackable;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 MaxStackCount; // 고정값 (데이터 테이블에서 99, 1 등으로 입력)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName ItemID; // 아이템 고유 ID (예: "RedFlowerSeed")

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EItemCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Category == EItemCategory::Tool"), Category = "Item Data")
	EItemToolType ToolType = EItemToolType::EITT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Category == EItemCategory::Seed || Category == EItemCategory::GrownPlant"), Category = "Item Data")
	FPlantData PlantData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemVisualData VisualData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 SellValue; // 상점에 팔 때 받을 수 있는 기본 가격
};