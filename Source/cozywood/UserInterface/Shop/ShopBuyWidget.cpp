#include "ShopBuyWidget.h"
#include "ShopItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "../../Player/C1PlayerController.h"
#include "../../Character/C1Character.h"
#include "../../Components/InventoryComponent.h"
#include "../../Components/EconomyComponent.h"
#include "../../ItemData/ItemDataStructs.h"
#include "Engine/DataTable.h"

FReply UShopBuyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	DeselectCurrentSlot(); // 빈 화면 클릭 시 닫기
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UShopBuyWidget::OnClickedBack()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
		PC->OpenShopModeSelect();
}

void UShopBuyWidget::OnClickedClose()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
		PC->CloseShopUI();
}

void UShopBuyWidget::HandleMoneyChanged(int32 NewMoneyAmount)
{
	RefreshMoneyText();
}

void UShopBuyWidget::RefreshMoneyText()
{
	if (MoneyText && CachedEconomy)
		MoneyText->SetText(FText::AsNumber(CachedEconomy->GetCurrentMoney()));
}

void UShopBuyWidget::ExecuteBuy(const FName& ItemID, int32 Price)
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
		PC->BuyItem(ItemID, Price, 1);
}

void UShopBuyWidget::RebuildShopList()
{
	if (!CachedInventory)
	{
		if (AC1Character* Character = Cast<AC1Character>(GetOwningPlayerPawn()))
		{
			CachedInventory = Character->GetInventory();
			CachedEconomy = Character->GetEconomyComp();
		}
	}

	if (!CachedInventory || !CachedInventory->ItemDataTable || !ItemListWrapBox || !ShopItemSlotWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopBuyWidget RebuildShopList: missing ref - Inventory:%s DataTable:%s WrapBox:%s SlotClass:%s"),
			CachedInventory ? TEXT("OK") : TEXT("NULL"),
			(CachedInventory && CachedInventory->ItemDataTable) ? TEXT("OK") : TEXT("NULL"),
			ItemListWrapBox ? TEXT("OK") : TEXT("NULL"),
			ShopItemSlotWidgetClass ? TEXT("OK") : TEXT("NULL"));
		return;
	}

	// ShopItemIDs가 비어있으면 DataTable의 모든 행을 자동으로 채움
	if (ShopItemIDs.IsEmpty())
	{
		ShopItemIDs = CachedInventory->ItemDataTable->GetRowNames();
	}

	ItemListWrapBox->ClearChildren();

	static const FString ContextString(TEXT("ShopBuyData"));

	for (const FName& ItemID : ShopItemIDs)
	{
		FItemData* RowData = CachedInventory->ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
		if (!RowData) continue;

		UShopItemSlotWidget* NewSlot = CreateWidget<UShopItemSlotWidget>(this, ShopItemSlotWidgetClass);
		if (!NewSlot) continue;

		NewSlot->InitBuySlot(
			ItemID,
			RowData->TextData.Name,
			RowData->TextData.Description,
			RowData->TextData.UsageText,
			RowData->Category,
			RowData->SellValue,
			RowData->VisualData.Icon,
			this
		);

		ItemListWrapBox->AddChild(NewSlot);
	}
}

void UShopBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
		BackButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedBack);
	if (CloseButton)
		CloseButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedClose);

	// 구매, 취소 버튼 바인딩 추가
	if (InlineBuyButton)
		InlineBuyButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedInlineBuy);
	if (InlineCancelButton)
		InlineCancelButton->OnClicked.AddDynamic(this, &UShopBuyWidget::OnClickedInlineCancel);

	if (AC1Character* Character = Cast<AC1Character>(GetOwningPlayerPawn()))
	{
		CachedInventory = Character->GetInventory();
		CachedEconomy = Character->GetEconomyComp();

		if (CachedEconomy)
			CachedEconomy->OnMoneyChanged.AddDynamic(this, &UShopBuyWidget::HandleMoneyChanged);
	}

	RefreshMoneyText();
	RebuildShopList();
}

void UShopBuyWidget::OnSlotSelected(UShopItemSlotWidget* SelectedSlot)
{
	// 블루프린트에서 팝업 이동 및 숨김을 처리하므로, C++에서는 정보 갱신만 합니다.
	CurrentlySelectedSlot = SelectedSlot;
}

void UShopBuyWidget::DeselectCurrentSlot()
{
	// 선택 해제 정보만 초기화
	CurrentlySelectedSlot = nullptr;
}

// 새로 추가하는 함수들
void UShopBuyWidget::OnClickedInlineBuy()
{
	// 현재 선택된 슬롯이 있다면, 그 슬롯의 정보를 가져와서 구매 실행
	if (CurrentlySelectedSlot)
	{
		ExecuteBuy(CurrentlySelectedSlot->GetItemID(), CurrentlySelectedSlot->GetCachedPrice());
	}
}

void UShopBuyWidget::OnClickedInlineCancel()
{
	DeselectCurrentSlot();
}
