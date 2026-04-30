#include "InventoryItemSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../../Items/ItemBase.h"
#include "InventoryTooltip.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "../../Character/C1Character.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ToolTipClass) {
		CreatedTooltip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		CreatedTooltip->InventorySlotBeingHovered = this;
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateSlot();
}

void UInventoryItemSlot::UpdateSlot()
{
	// 아이템 데이터가 들어있는 경우
	if (ItemReference)
	{
		// 카테고리별 배경색 설정  -> 수정 필요
		switch (ItemReference->Category)
		{
		case EItemCategory::Tool:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemCategory::Furniture:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemCategory::Seed:
			ItemBorder->SetBrushColor(FLinearColor(0.54f, 0.27f, 0.07f, 1.0f));
			break;
		case EItemCategory::GrownPlant:
			ItemBorder->SetBrushColor(FLinearColor::Green);
			break;
		default:
			ItemBorder->SetBrushColor(FLinearColor::Black);
		}

		// 아이콘 설정
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemIcon->SetBrushFromTexture(ItemReference->VisualData.Icon);

		// 수량(숫자) 표시 설정
		if (ItemReference->IsStackable() && ItemReference->Quantity > 1)
		{
			ItemQuantity->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}

		// 버튼 활성화 & 툴팁 장착
		Button_0->SetIsEnabled(true);
		if (CreatedTooltip) {
			SetToolTip(CreatedTooltip);
		}
	}
	// 아이템 데이터가 없는 빈 슬롯인 경우
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));

		Button_0->SetIsEnabled(false);
		SetToolTip(nullptr);
	}
}


void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
