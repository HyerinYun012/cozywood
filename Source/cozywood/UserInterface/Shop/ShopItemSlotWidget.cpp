#include "ShopItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "../../Items/ItemBase.h"
#include "ShopBuyWidget.h"
#include "ShopSellWidget.h"

void UShopItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &UShopItemSlotWidget::HandleClicked);
	}
}

void UShopItemSlotWidget::InitBuySlot(
	const FName& InItemID,
	const FText& InName,
	const FText& InDescription,
	int32 InPrice,
	UTexture2D* InIcon,
	UShopBuyWidget* InOwner
)
{
	bIsSellSlot = false;
	ItemID = InItemID;
	CachedName = InName;
	CachedDescription = InDescription;
	CachedPrice = InPrice;
	IconTexture = InIcon;
	BuyOwner = InOwner;
	SellOwner = nullptr;
	ItemRef = nullptr;

	ApplyVisuals();
}

void UShopItemSlotWidget::InitSellSlot(
	UItemBase* InItem,
	const FText& InName,
	const FText& InDescription,
	int32 InPrice,
	UTexture2D* InIcon,
	UShopSellWidget* InOwner
)
{
	bIsSellSlot = true;
	ItemRef = InItem;
	CachedName = InName;
	CachedDescription = InDescription;
	CachedPrice = InPrice;
	IconTexture = InIcon;
	SellOwner = InOwner;
	BuyOwner = nullptr;

	if (InItem)
	{
		ItemID = InItem->ItemID;
	}

	ApplyVisuals();
}

void UShopItemSlotWidget::ApplyVisuals()
{
	if (NameText)
	{
		NameText->SetText(CachedName);
	}

	if (PriceText)
	{
		PriceText->SetText(FText::AsNumber(CachedPrice));
	}

	if (ItemIcon && IconTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(IconTexture);
		ItemIcon->SetBrush(Brush);
	}
}

void UShopItemSlotWidget::HandleClicked()
{
	if (!bIsSellSlot)
	{
		if (BuyOwner)
		{
			BuyOwner->SelectShopItem(ItemID, CachedName, CachedDescription, CachedPrice, IconTexture);
		}
	}
	else
	{
		if (SellOwner)
		{
			SellOwner->SelectInventoryItem(ItemRef, CachedName, CachedDescription, CachedPrice, IconTexture);
		}
	}
}