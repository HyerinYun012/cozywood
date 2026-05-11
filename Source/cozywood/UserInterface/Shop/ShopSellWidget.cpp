#include "ShopSellWidget.h"
#include "ShopSellItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
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
		BackButton->OnClicked.AddDynamic(this, &UShopSellWidget::OnClickedBack);
	if (CloseButton)
		CloseButton->OnClicked.AddDynamic(this, &UShopSellWidget::OnClickedClose);
	if (DeselectZoneButton)
		DeselectZoneButton->OnClicked.AddDynamic(this, &UShopSellWidget::OnClickedDeselectZone);

	if (AC1Character* Character = Cast<AC1Character>(GetOwningPlayerPawn()))
	{
		CachedInventory = Character->GetInventory();
		CachedEconomy = Character->GetEconomyComp();

		if (CachedEconomy)
			CachedEconomy->OnMoneyChanged.AddDynamic(this, &UShopSellWidget::HandleMoneyChanged);
		if (CachedInventory)
			CachedInventory->OnInventoryUpdated.AddUObject(this, &UShopSellWidget::RebuildSellList);
	}

	RefreshMoneyText();
	RebuildSellList();
}

FReply UShopSellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	DeselectCurrentSlot();
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UShopSellWidget::OnClickedBack()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
		PC->OpenShopModeSelect();
}

void UShopSellWidget::OnClickedClose()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
		PC->CloseShopUI();
}

void UShopSellWidget::OnClickedDeselectZone()
{
	DeselectCurrentSlot();
}

void UShopSellWidget::HandleMoneyChanged(int32 NewMoneyAmount)
{
	RefreshMoneyText();
}

void UShopSellWidget::RefreshMoneyText()
{
	if (MoneyText && CachedEconomy)
		MoneyText->SetText(FText::AsNumber(CachedEconomy->GetCurrentMoney()));
}

void UShopSellWidget::OnSlotSelected(UShopSellItemSlotWidget* SelectedSlot)
{
	if (CurrentlySelectedSlot && CurrentlySelectedSlot != SelectedSlot)
		CurrentlySelectedSlot->SetActionBoxVisible(false);

	CurrentlySelectedSlot = SelectedSlot;
}

void UShopSellWidget::OnSlotDeselected()
{
	CurrentlySelectedSlot = nullptr;
}

void UShopSellWidget::DeselectCurrentSlot()
{
	if (CurrentlySelectedSlot)
	{
		CurrentlySelectedSlot->SetActionBoxVisible(false);
		CurrentlySelectedSlot = nullptr;
	}
}

void UShopSellWidget::ExecuteSell(UItemBase* Item, int32 Price)
{
	// CurrentlySelectedSlot = nullptr;

	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
		PC->SellItem(Item, Price, 1);
}

void UShopSellWidget::RebuildSellList()
{
	CurrentlySelectedSlot = nullptr;

	if (!ItemListWrapBox)
	{
		UE_LOG(LogTemp, Error, TEXT("ShopSellWidget: ItemListWrapBox is null. Check BindWidget name in WBP_ShopSellWidget."));
		return;
	}
	ItemListWrapBox->ClearChildren();

	if (!CachedInventory)
	{
		UE_LOG(LogTemp, Error, TEXT("ShopSellWidget: CachedInventory is null. GetOwningPlayerPawn() may have failed in NativeConstruct."));
		return;
	}

	if (!ShopSellItemSlotWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ShopSellWidget: ShopSellItemSlotWidgetClass is not assigned. Set it in WBP_ShopSellWidget Details panel."));
		return;
	}

	static const FString ContextString(TEXT("ShopSellData"));
	const TArray<UItemBase*> Contents = CachedInventory->GetInventoryContents();
	const int32 TotalSlots = 50;

	UE_LOG(LogTemp, Log, TEXT("ShopSellWidget: Building %d slots, inventory has %d items."), TotalSlots, Contents.Num());

	for (int32 i = 0; i < TotalSlots; ++i)
	{
		TSubclassOf<UUserWidget> WClass = ShopSellItemSlotWidgetClass;
		UShopSellItemSlotWidget* NewSlot = CreateWidget<UShopSellItemSlotWidget>(this, WClass);
		if (!NewSlot)
		{
			UE_LOG(LogTemp, Error, TEXT("ShopSellWidget: CreateWidget returned null at slot %d. Check BindWidget names in WBP_ShopSellItemSlot match C++ variable names exactly."), i);
			return;
		}

		if (Contents.IsValidIndex(i) && IsValid(Contents[i]))
		{
			UItemBase* Item = Contents[i];
			int32 SellPrice = 0;
			if (CachedInventory->ItemDataTable)
			{
				if (FItemData* RowData = CachedInventory->ItemDataTable->FindRow<FItemData>(Item->ItemID, ContextString))
					SellPrice = RowData->SellValue;
			}
			NewSlot->InitSlot(Item, SellPrice, this);
		}
		else
		{
			NewSlot->InitSlot(nullptr, 0, this);
		}

		ItemListWrapBox->AddChild(NewSlot);
	}

	UE_LOG(LogTemp, Log, TEXT("ShopSellWidget: RebuildSellList complete. WrapBox child count: %d"), ItemListWrapBox->GetChildrenCount());
}
