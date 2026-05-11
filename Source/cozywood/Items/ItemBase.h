#pragma once

#include "CoreMinimal.h"
#include "../ItemData/ItemDataStructs.h"
#include "ItemBase.generated.h"

class AC1Character;
class UInventoryComponent;

UCLASS(BlueprintType)
class COZYWOOD_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	// Properties & Variables
	UPROPERTY()
	UInventoryComponent* OwningInventory; // 이 아이템이 속한 인벤토리 컴포넌트에 대한 참조

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID; // 아이템 고유 ID (예: "RedFlowerSeed")

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Category == EItemCategory::Tool"), Category = "Item Data")
	EItemToolType ToolType = EItemToolType::EITT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Category == EItemCategory::Seed || Category == EItemCategory::GrownPlant"), Category = "Item Data")
	FPlantData PlantData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemVisualData VisualData;

	bool bIsCopy; // 이 아이템이 원본에서 복사된 것인지 여부 (인벤토리에 추가될 때 원본을 복사해서 사용하기 위해)
	bool bIsPickup; // 이 아이템이 월드에서 줍는 아이템인지 여부 (인벤토리에 추가될 때 원본을 그대로 사용하기 위해)


	// Functions
	UItemBase();

	void ResetItemFlags();

	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemBase* CreateItemCopy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE bool IsStackable() const {
		return bIsStackable;
	}

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE bool CanStackWith(const UItemBase* Other) const {
		return Other && ItemID == Other->ItemID && IsStackable();
	}

	virtual void UseItem(AC1Character* Character); // 아이템 사용 시의 기본 동작 (예: 수확, 심기 등)


	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE bool IsFullItemStack() const {
		return Quantity >= MaxStackCount; // 아이템이 한칸당 최대 겹칠 수 있는 수량과 비교하여 가득 찼는지 확인
	}

protected:
	bool operator==(const UItemBase& OtherID) const {
		return this->ItemID == OtherID.ItemID && Category == OtherID.Category;
	}
};
