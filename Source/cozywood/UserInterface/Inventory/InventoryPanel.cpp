#include "InventoryPanel.h"
#include "../../Character/C1Character.h"
#include "../../Components/InventoryComponent.h"
#include "InventoryItemSlot.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"


void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AC1Character>(GetOwningPlayerPawn());
	if (PlayerCharacter) {
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference) {
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();

            RefreshInventory();
		}
	}
}

void UInventoryPanel::SetInfoText() const
{
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"), InventoryReference->GetInventoryContents().Num(), InventoryReference->GetSlotsCapacity()));
}

void UInventoryPanel::RefreshInventory()
{
    if (InventoryReference && InventorySlotClass) {

        InventoryPanel->ClearChildren();

        const int32 ItemsPerRow = 5;

        // 1. 아이템 목록과 최대 칸 수(Capacity)를 가져옴
        auto Contents = InventoryReference->GetInventoryContents();
        int32 Capacity = InventoryReference->GetSlotsCapacity();

        // 2. 최대 칸 수(50번)만큼 무조건 반복
        for (int32 Index = 0; Index < Capacity; ++Index) {

            UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);

            // 3. 해당 칸(Index)에 아이템이 있으면 넣어주고, 없으면 nullptr(빈 칸) 처리
            if (Contents.IsValidIndex(Index)) {
                ItemSlot->SetItemReference(Contents[Index]);
            }
            else {
                ItemSlot->SetItemReference(nullptr);
            }

            // 4. 행(Row)과 열(Column) 계산
            int32 Row = Index / ItemsPerRow;
            int32 Column = Index % ItemsPerRow;

            // 5. 그리드 패널에 자식으로 추가
            UUniformGridSlot* GridSlot = InventoryPanel->AddChildToUniformGrid(ItemSlot, Row, Column);

            // 6. 정사각형 모양 유지
            if (GridSlot) {
                GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
                GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
            }
        }
        SetInfoText();
    }
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
