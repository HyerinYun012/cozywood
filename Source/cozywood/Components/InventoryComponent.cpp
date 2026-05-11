#include "InventoryComponent.h"
#include "../Items/ItemBase.h"
#include "../Save/GardenSave.h"
#include "Kismet/GameplayStatics.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 저장 파일에서 인벤토리를 먼저 복구한 뒤, 이후 변경 시 자동 저장되도록 바인딩
	LoadInventory();
	OnInventoryUpdated.AddUObject(this, &UInventoryComponent::SaveInventory);
}

// 저장 파일 → 인벤토리 복구 (BeginPlay 시 1회 호출, 저장 파일 없으면 빈 인벤토리로 시작)
void UInventoryComponent::LoadInventory()
{
	if (!ItemDataTable) return;
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) return;

	UGardenSave* SaveInstance = Cast<UGardenSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveInstance) return;

	for (const FSavedInventoryItem& SavedItem : SaveInstance->SavedInventoryItems)
	{
		if (SavedItem.Quantity > 0)
		{
			AddStartingItemByID(SavedItem.ItemID, SavedItem.Quantity);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("인벤토리 불러오기 완료: %d종 아이템 복구됨."), SaveInstance->SavedInventoryItems.Num());
}

// 인벤토리 현재 상태를 저장 파일에 기록 (OnInventoryUpdated 브로드캐스트 시 자동 호출)
void UInventoryComponent::SaveInventory()
{
	// 기존 저장 파일을 먼저 불러와서 가구 데이터를 보존한 뒤 인벤토리만 갱신
	UGardenSave* SaveInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveInstance = Cast<UGardenSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	if (!SaveInstance)
	{
		SaveInstance = Cast<UGardenSave>(UGameplayStatics::CreateSaveGameObject(UGardenSave::StaticClass()));
	}

	SaveInstance->SavedInventoryItems.Empty();
	for (UItemBase* Item : InventoryContents)
	{
		if (Item && Item->Quantity > 0)
		{
			FSavedInventoryItem SavedItem;
			SavedItem.ItemID = Item->ItemID;
			SavedItem.Quantity = Item->Quantity;
			SaveInstance->SavedInventoryItems.Add(SavedItem);
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, 0);
}

// ID를 받아서 데이터 테이블 기반으로 아이템 객체를 생성해 인벤토리에 추가 (로드 및 픽업 공용)
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
	if (!ItemIn) return 0; // 방어 코드 추가

	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	// 1. 수량만 먼저 깎습니다.
	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	// 2. 수량이 0 이하가 되었다면 배열에서 안전하게 완전히 뽑아냄
	if (ItemIn->Quantity <= 0)
	{
		InventoryContents.RemoveSingle(ItemIn);
	}

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
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
		NewItemCopy->bIsCopy = true;
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

	UItemBase* FoundItem = nullptr;

	// 1단계: 지울 아이템을 찾기만 함 (배열을 건드리지 않음)
	for (UItemBase* Item : InventoryContents)
	{
		if (Item && Item->VisualData.ActorClass == InClass)
		{
			FoundItem = Item;
			break;
		}
	}

	// 2단계: 찾은 아이템이 있다면, 반복문 밖에서 안전하게 차감 및 삭제 진행
	if (FoundItem)
	{
		FoundItem->SetQuantity(FoundItem->Quantity - 1);

		// 수량이 0 이하가 되면 배열에서 완전히 제거
		if (FoundItem->Quantity <= 0)
		{
			InventoryContents.RemoveSingle(FoundItem);
		}

		OnInventoryUpdated.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("아이템 차감 성공: %s (남은 수량: %d)"), *FoundItem->TextData.Name.ToString(), FoundItem->Quantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("차감할 아이템을 찾지 못함"));
	}
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
