#include "ShopSellItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "../../Items/ItemBase.h"
#include "ShopSellWidget.h"

void UShopSellItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
		SlotButton->OnClicked.AddDynamic(this, &UShopSellItemSlotWidget::HandleSlotClicked);

	if (SellConfirmButton)
		SellConfirmButton->OnClicked.AddDynamic(this, &UShopSellItemSlotWidget::HandleSellConfirm);

	if (SellCancelButton)
		SellCancelButton->OnClicked.AddDynamic(this, &UShopSellItemSlotWidget::HandleSellCancel);
}

void UShopSellItemSlotWidget::InitSlot(UItemBase* InItem, int32 InSellPrice, UShopSellWidget* InOwner)
{
	ItemRef = InItem;
	SellOwner = InOwner;
	CachedPrice = InSellPrice;

	SetActionBoxVisible(false);

	if (ItemRef)
	{
		// 아이템이 있는 경우: 아이콘, 이름 노출 및 버튼 활성화
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
			FSlateBrush Brush;
			Brush.SetResourceObject(ItemRef->VisualData.Icon);
			ItemIcon->SetBrush(Brush);
		}
		if (NameText)
		{
			NameText->SetVisibility(ESlateVisibility::Visible);
			NameText->SetText(ItemRef->TextData.Name);
		}
		if (SlotButton) SlotButton->SetIsEnabled(true);
	}
	else
	{
		// 빈 슬롯인 경우: UI 숨김 및 클릭 방지
		if (ItemIcon) ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		if (NameText) NameText->SetVisibility(ESlateVisibility::Hidden);
		if (SlotButton) SlotButton->SetIsEnabled(false);
	}
}

void UShopSellItemSlotWidget::SetActionBoxVisible(bool bVisible)
{
	if (ActionBox)
	{
		ActionBox->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UShopSellItemSlotWidget::HandleSlotClicked()
{
	if (!ItemRef) return;

	if (SellOwner)
	{
		// 기존에 켜져 있던 다른 슬롯의 ActionBox를 끄기 위해 Owner에게 알림
		SellOwner->OnSlotSelected(this);
	}

	SetActionBoxVisible(true);
}

void UShopSellItemSlotWidget::HandleSellConfirm()
{
	if (SellOwner && ItemRef)
	{
		// 팔기 실행 (이 함수 안에서 돈이 추가되고 아이템이 삭제되며 UI가 리빌드됨)
		SellOwner->ExecuteSell(ItemRef, CachedPrice);
	}
}

void UShopSellItemSlotWidget::HandleSellCancel()
{
	SetActionBoxVisible(false);
	if (SellOwner)
	{
		SellOwner->OnSlotDeselected();
	}
}