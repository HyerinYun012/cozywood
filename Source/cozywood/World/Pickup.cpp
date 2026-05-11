#include "Pickup.h"
#include "../Character/C1Character.h"
#include "../ItemData/ItemDataStructs.h"
#include "../Items/ItemBase.h"
#include "../Interfaces/InteractionInterface.h"
#include "../Components/InventoryComponent.h"



APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone()) {
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		// ItemData를 무사히 찾아왔을 때만 아래 로직을 실행!
		if (ItemData)
		{
			ItemReference = NewObject<UItemBase>(this, BaseClass);

			ItemReference->ItemID = ItemData->ItemID;
			ItemReference->Category = ItemData->Category;
			ItemReference->VisualData = ItemData->VisualData;

			InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

			PickupMesh->SetStaticMesh(ItemData->VisualData.Mesh);

			UpdateInteractableData();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Pickup 초기화 실패! 데이터 테이블에서 [%s]를 찾을 수 없습니다."), *DesiredItemID.ToString());
		}
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	PickupMesh->SetStaticMesh(ItemReference->VisualData.Mesh);
	
	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::EIT_Pickup;
	// InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	// C4244 fix: int32 → int8 (FInteractableData::Quantity는 int8)
	InstanceInteractableData.Quantity = static_cast<int8>(ItemReference->Quantity);
	// InteractableData = InstanceInteractableData;
}

void APickup::BeginFocus_Implementation()
{
	if (PickupMesh) {
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus_Implementation()
{
	if (PickupMesh) {
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact_Implementation(AC1Character* PlayerCharacter)
{
	if(PlayerCharacter) {
		TakePickup(PlayerCharacter);
	}
}

void APickup::TakePickup(const AC1Character* Taker)
{
	if (IsValid(this)) { 
		if (ItemReference) {
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory()) {
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.OperationResult) {
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					// Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				}
				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"))
		}
	}
}

#if WITH_EDITOR
void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID)) {
		if (ItemDataTable) {
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString())) {
				PickupMesh->SetStaticMesh(ItemData->VisualData.Mesh);
			}
		}
	}
}
#endif

