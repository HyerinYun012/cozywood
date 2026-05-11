#include "ShopItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Engine/Texture2D.h"
#include "../../Items/ItemBase.h"
#include "../../ItemData/ItemDataStructs.h"
#include "ShopBuyWidget.h"

void UShopItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
		SlotButton->OnClicked.AddDynamic(this, &UShopItemSlotWidget::HandleClicked);
	/*if (InlineBuyButton)
		InlineBuyButton->OnClicked.AddDynamic(this, &UShopItemSlotWidget::HandleInlineBuy);
	if (InlineCancelButton)
		InlineCancelButton->OnClicked.AddDynamic(this, &UShopItemSlotWidget::HandleInlineCancel);*/
}

void UShopItemSlotWidget::InitBuySlot(
	const FName& InItemID,
	const FText& InName,
	const FText& InDescription,
	const FText& InUsageText,
	EItemCategory InCategory,
	int32 InPrice,
	UTexture2D* InIcon,
	UShopBuyWidget* InOwner
)
{
	ItemID = InItemID;
	CachedName = InName;
	CachedDescription = InDescription;
	CachedUsageText = InUsageText;
	CachedPrice = InPrice;
	IconTexture = InIcon;
	BuyOwner = InOwner;
	ItemRef = nullptr;

	const UEnum* CategoryEnum = StaticEnum<EItemCategory>();
	CachedCategory = CategoryEnum
		? CategoryEnum->GetDisplayValueAsText(InCategory)
		: FText::GetEmpty();

	// SetActionBoxVisible(false);
	ApplyVisuals();
}

/*
void UShopItemSlotWidget::SetActionBoxVisible(bool bVisible)
{
	if (ActionBox)
		ActionBox->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
*/

void UShopItemSlotWidget::ApplyVisuals()
{
	if (NameText)        NameText->SetText(CachedName);
	if (PriceText)       PriceText->SetText(FText::AsNumber(CachedPrice));
	if (DescriptionText) DescriptionText->SetText(CachedDescription);
	if (UsageText)       UsageText->SetText(CachedUsageText);
	if (CategoryText)    CategoryText->SetText(CachedCategory);

	if (ItemIcon && IconTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(IconTexture);
		ItemIcon->SetBrush(Brush);
	}
}

void UShopItemSlotWidget::HandleClicked()
{
	// 부모 UI(BuyOwner)에게 내가 클릭되었다고 알림 (다른 창 닫기용)
	if (BuyOwner)
	{
		BuyOwner->OnSlotSelected(this);
	}

	// SetActionBoxVisible(true);
}
/*void UShopItemSlotWidget::HandleInlineBuy()
{
	if (BuyOwner)
		BuyOwner->ExecuteBuy(ItemID, CachedPrice);
}

void UShopItemSlotWidget::HandleInlineCancel()
{
	SetActionBoxVisible(false);
}
*/