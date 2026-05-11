#include "ItemBase.h"
#include "../Components/InventoryComponent.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false){
	Quantity = 1;
	ItemID = "DefaultItem";
	Category = EItemCategory::Tool;
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() {
	UItemBase* ItemCopy = NewObject<UItemBase>(GetTransientPackage(), this->GetClass());

	ItemCopy->Quantity = this->Quantity;
	ItemCopy->MaxStackCount = this->MaxStackCount;
	ItemCopy->bIsStackable = this->bIsStackable;
	ItemCopy->ItemID = this->ItemID;
	ItemCopy->Category = this->Category;
	ItemCopy->ToolType = this->ToolType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->PlantData = this->PlantData;
	ItemCopy->VisualData = this->VisualData;
	ItemCopy->bIsCopy = true;
	ItemCopy->bIsPickup = false;

	return ItemCopy;
}

void UItemBase::SetQuantity(int32 NewQuantity) {
	if (NewQuantity != this->Quantity) {
		Quantity = FMath::Clamp(NewQuantity, 0, IsStackable() ? MaxStackCount : 1);
		if (OwningInventory) {
			if (Quantity <= 0) {
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
		
	}


}

void UItemBase::UseItem(AC1Character* Character) {
	// 기본적으로는 아무 동작도 하지 않음
	// 각 아이템 유형에 따라 상호작용 로직을 구현할 수 있음 (예: 수확, 심기 등)
}

