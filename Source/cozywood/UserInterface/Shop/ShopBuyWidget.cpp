#include "ShopBuyWidget.h"
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
#include "../../ItemData/ItemDataStructs.h"
#include "Engine/DataTable.h"

void UShopBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedBack);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedClose);
	}

	if (BuyButton)
	{
		BuyButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedBuy);
	}

	if (CancelSelectionButton)
	{
		CancelSelectionButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedCancelSelection);
	}

	if (AC1Character* Character = Cast<AC1Character>(GetOwningPlayerPawn()))
	{
		CachedInventory = Character->GetInventory();
		CachedEconomy = Character->GetEconomyComp();

		if (CachedEconomy)
		{
			CachedEconomy->OnMoneyChanged.AddDynamic(this, &UShopBuyWidget::HandleMoneyChanged);
		}
	}

	ShowSelectedPanel(false);
	RefreshMoneyText();
	RebuildShopList();
}

void UShopBuyWidget::OnClickedBack()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->OpenShopModeSelect();
	}
}

void UShopBuyWidget::OnClickedClose()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->CloseShopUI();
	}
}

void UShopBuyWidget::OnClickedBuy()
{
	if (!CachedInventory || !CachedEconomy || SelectedItemID.IsNone())
	{
		return;
	}

	if (CachedEconomy->SpendMoney(SelectedPrice))
	{
		CachedInventory->AddStartingItemByID(SelectedItemID, 1);
	}

	RefreshMoneyText();
}

void UShopBuyWidget::OnClickedCancelSelection()
{
	ShowSelectedPanel(false);
	SelectedItemID = NAME_None;
	SelectedPrice = 0;
}

void UShopBuyWidget::HandleMoneyChanged(int32 NewMoneyAmount)
{
	RefreshMoneyText();
}

void UShopBuyWidget::RefreshMoneyText()
{
	if (MoneyText && CachedEconomy)
	{
		MoneyText->SetText(FText::AsNumber(CachedEconomy->GetCurrentMoney()));
	}
}

void UShopBuyWidget::ShowSelectedPanel(bool bShow)
{
	if (SelectedDetailBox)
	{
		SelectedDetailBox->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UShopBuyWidget::SelectShopItem(const FName& InItemID, const FText& InName, const FText& InDescription, int32 InPrice, UTexture2D* InIcon)
{
	SelectedItemID = InItemID;
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

	if (BuyButtonText)
	{
		BuyButtonText->SetText(FText::FromString(TEXT("구매")));
	}

	ShowSelectedPanel(true);
}

void UShopBuyWidget::RebuildShopList()
{
	if (!CachedInventory || !CachedInventory->ItemDataTable || !ItemListWrapBox || !ShopItemSlotWidgetClass)
	{
		return;
	}

	ItemListWrapBox->ClearChildren();

	static const FString ContextString(TEXT("ShopBuyData"));

	for (const FName& ItemID : ShopItemIDs)
	{
		FItemData* RowData = CachedInventory->ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
		if (!RowData)
		{
			continue;
		}

		UShopItemSlotWidget* NewSlot = CreateWidget<UShopItemSlotWidget>(this, ShopItemSlotWidgetClass);
		if (!NewSlot)
		{
			continue;
		}

		NewSlot->InitBuySlot(
			ItemID,
			RowData->TextData.Name,
			RowData->TextData.Description,
			RowData->SellValue,
			RowData->VisualData.Icon,
			this
		);

		ItemListWrapBox->AddChild(NewSlot);
	}
}