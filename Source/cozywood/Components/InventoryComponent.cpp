#include "InventoryComponent.h"
#include "../Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (FName ItemID : DefaultStartingItems)
	{
		// ID를 던져서 아이템 객체를 인벤토리에 넣는다.
		AddStartingItemByID(ItemID, 1);
	}

}

// 임의로 초기 아이템을 인벤에 넣는 것(기능 테스트용 함수) -> 지울거임
void UInventoryComponent::AddStartingItemByID(FName ItemID, int32 AmountToAdd)
{
	if (!ItemDataTable) return;

	static const FString ContextString(TEXT("Item Data Context"));
	FItemData* RowData = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);

	if (RowData)
	{
		UItemBase* CreatedItem = NewObject<UItemBase>(this, UItemBase::StaticClass());

		CreatedItem->ItemID = ItemID;
		CreatedItem->Quantity = AmountToAdd;
		CreatedItem->MaxStackCount = RowData->MaxStackCount;
		CreatedItem->bIsStackable = RowData->bIsStackable;
		CreatedItem->Category = RowData->Category;
		CreatedItem->ToolType = RowData->ToolType;
		CreatedItem->TextData = RowData->TextData;
		CreatedItem->PlantData = RowData->PlantData;
		CreatedItem->VisualData = RowData->VisualData;
		CreatedItem->bIsCopy = true;
		CreatedItem->bIsPickup = false;

		HandleAddItem(CreatedItem);
	}
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn) {
		if (InventoryContents.Contains(ItemIn)) {
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if (auto* Result = InventoryContents.FindByKey(ItemIn)) {
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (auto* Result = InventoryContents.FindByPredicate([ItemIn](const TObjectPtr<UItemBase>& InventoryItem)
		{
			return InventoryItem->ItemID == ItemIn->ItemID && !InventoryItem->IsFullItemStack();
		}
	))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->MaxStackCount - StackableItem->Quantity;
	
	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActulaAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActulaAmountToRemove);

	OnInventoryUpdated.Broadcast();

	return ActulaAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if(!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
		
	}
}


FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem, int32 RequestedAddAmount)
{
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity) {
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All inventory slots are full"), InputItem->TextData.Name));
	}

	AddNewItem(InputItem, RequestedAddAmount);
	return FItemAddResult::AddedAll(RequestedAddAmount, FText::Format(
		FText::FromString("Successfully added {0} {1} to the inventory."), RequestedAddAmount, InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || !ItemIn) return 0;

	int32 AmountToDistribute = RequestedAddAmount;

	// 1. 기존에 있는 부분 스택 먼저 꽉 채우기 (for-each 루프로 무한 루프 방지)
	for (auto& InventoryItem : InventoryContents)
	{
		// 같은 아이디이면서 꽉 차지 않은 스택 찾기
		if (InventoryItem && InventoryItem->ItemID == ItemIn->ItemID && !InventoryItem->IsFullItemStack())
		{
			const int32 AmountToMakeFullStack = CalculateNumberForFullStack(InventoryItem, AmountToDistribute);

			InventoryItem->SetQuantity(InventoryItem->Quantity + AmountToMakeFullStack);
			AmountToDistribute -= AmountToMakeFullStack;

			// 더 이상 분배할 수량이 없으면 즉시 종료
			if (AmountToDistribute <= 0) {
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount;
			}
		}
	}

	// 2. 남은 수량이 있다면 빈 슬롯에 새로운 스택으로 추가
	const int32 MaxStackSize = ItemIn->MaxStackCount;

	while (AmountToDistribute > 0 && InventoryContents.Num() < InventorySlotsCapacity)
	{
		int32 AmountToAddThisSlot = FMath::Min(AmountToDistribute, MaxStackSize);

		// 새 복사본 생성 후 인벤토리에 추가 (AddNewItem 내부에서 이중 복사되지 않게 처리)
		UItemBase* NewItemCopy = ItemIn->CreateItemCopy();
		NewItemCopy->bIsCopy = true; // 확실한 꼬임 방지
		AddNewItem(NewItemCopy, AmountToAddThisSlot);

		AmountToDistribute -= AmountToAddThisSlot;
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner()) {
		const int32 InitialRequestedAddAmount = InputItem->Quantity;


		if (!InputItem->IsStackable())
		{
			return HandleNonStackableItems(InputItem, InitialRequestedAddAmount);
		}

		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if(StackableAmountAdded == InitialRequestedAddAmount)
		{
			
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0) {

		}

		if (StackableAmountAdded <= 0) {

		}
		return FItemAddResult::AddedAll(StackableAmountAdded, FText::GetEmpty());
	}

	return FItemAddResult::AddedNone(FText::GetEmpty());
}

void UInventoryComponent::RemoveItemByActorClass(TSubclassOf<AActor> InClass)
{
	if (!InClass) return;

	for (int32 i = 0; i < InventoryContents.Num(); ++i)
	{
		UItemBase* Item = InventoryContents[i];
		if (Item && Item->VisualData.ActorClass == InClass)
		{
			Item->SetQuantity(Item->Quantity - 1);

			UE_LOG(LogTemp, Log, TEXT("아이템 차감 성공: %s (남은 수량: %d)"), *Item->TextData.Name.ToString(), Item->Quantity);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("차감할 아이템을 찾지 못했습니다."));
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	if (!ItemToRemove) return;

	InventoryContents.RemoveSingle(ItemToRemove);

	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem = nullptr;

	if (Item->bIsCopy || Item->bIsPickup) {
		//if the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else {
		// used when spliting or dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	OnInventoryUpdated.Broadcast();
}
