#include "ShopSellWidget.h"
#include "ShopItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Widget.h"
#include "Engine/Texture2D.h"
#include "../../Player/C1PlayerController.h"
#include "../../Character/C1Character.h"
#include "../../Components/InventoryComponent.h"
#include "../../Components/EconomyComponent.h"
#include "../../Items/ItemBase.h"
#include "../../ItemData/ItemDataStructs.h"
#include "Engine/DataTable.h"

void UShopSellWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UShopSellWidget::OnClickedBack);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UShopSellWidget::OnClickedClose);
	}

	if (SellButton)
	{
		SellButton->OnClicked.AddDynamic(this, &UShopSellWidget::OnClickedSell);
	}

	if (CancelSelectionButton)
	{
		CancelSelectionButton->OnClicked.AddDynamic(this, &UShopSellWidget::OnClickedCancelSelection);
	}

	if (AC1Character* Character = Cast<AC1Character>(GetOwningPlayerPawn()))
	{
		CachedInventory = Character->GetInventory();
		CachedEconomy = Character->GetEconomyComp();

		if (CachedEconomy)
		{
			CachedEconomy->OnMoneyChanged.AddDynamic(this, &UShopSellWidget::HandleMoneyChanged);
		}

		if (CachedInventory)
		{
			CachedInventory->OnInventoryUpdated.AddUObject(this, &UShopSellWidget::RebuildSellList);
		}
	}

	ShowSelectedPanel(false);
	RefreshMoneyText();
	RebuildSellList();
}

void UShopSellWidget::OnClickedBack()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->OpenShopModeSelect();
	}
}

void UShopSellWidget::OnClickedClose()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->CloseShopUI();
	}
}

void UShopSellWidget::OnClickedSell()
{
	if (!CachedInventory || !CachedEconomy || !SelectedInventoryItem)
	{
		return;
	}

	SelectedInventoryItem->SetQuantity(SelectedInventoryItem->Quantity - 1);
	CachedEconomy->AddMoney(SelectedPrice);

	SelectedInventoryItem = nullptr;
	SelectedPrice = 0;
	ShowSelectedPanel(false);

	RefreshMoneyText();
	RebuildSellList();
}

void UShopSellWidget::OnClickedCancelSelection()
{
	SelectedInventoryItem = nullptr;
	SelectedPrice = 0;
	ShowSelectedPanel(false);
}

void UShopSellWidget::HandleMoneyChanged(int32 NewMoneyAmount)
{
	RefreshMoneyText();
}

void UShopSellWidget::RefreshMoneyText()
{
	if (MoneyText && CachedEconomy)
	{
		MoneyText->SetText(FText::AsNumber(CachedEconomy->GetCurrentMoney()));
	}
}

void UShopSellWidget::ShowSelectedPanel(bool bShow)
{
	if (SelectedDetailBox)
	{
		SelectedDetailBox->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UShopSellWidget::SelectInventoryItem(UItemBase* InItem, const FText& InName, const FText& InDescription, int32 InPrice, UTexture2D* InIcon)
{
	SelectedInventoryItem = InItem;
	SelectedPrice = InPrice;

	if (SelectedItemNameText)
	{
		SelectedItemNameText->SetText(InName);
	}

	if (SelectedItemDescriptionText)
	{
		SelectedItemDescriptionText->SetText(InDescription);
	}

	if (SelectedItemPriceText)
	{
		SelectedItemPriceText->SetText(FText::AsNumber(InPrice));
	}

	if (SelectedItemImage && InIcon)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(InIcon);
		SelectedItemImage->SetBrush(Brush);
	}

	if (SellButtonText)
	{
		SellButtonText->SetText(FText::FromString(TEXT("판매")));
	}

	ShowSelectedPanel(true);
}

void UShopSellWidget::RebuildSellList()
{
	if (!CachedInventory || !CachedInventory->ItemDataTable || !ItemListWrapBox || !ShopItemSlotWidgetClass)
	{
		return;
	}

	ItemListWrapBox->ClearChildren();

	static const FString ContextString(TEXT("ShopSellData"));

	const TArray<UItemBase*> InventoryItems = CachedInventory->GetInventoryContents();

	for (UItemBase* Item : InventoryItems)
	{
		if (!Item)
		{
			continue;
		}

		FItemData* RowData = CachedInventory->ItemDataTable->FindRow<FItemData>(Item->ItemID, ContextString);
		if (!RowData)
		{
			continue;
		}

		UShopItemSlotWidget* NewSlot = CreateWidget<UShopItemSlotWidget>(this, ShopItemSlotWidgetClass);
		if (!NewSlot)
		{
			continue;
		}

		NewSlot->InitSellSlot(
			Item,
			Item->TextData.Name,
			Item->TextData.Description,
			RowData->SellValue,
			Item->VisualData.Icon,
			this
		);

		ItemListWrapBox->AddChild(NewSlot);
	}
}