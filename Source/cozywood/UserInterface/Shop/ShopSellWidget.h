#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopSellWidget.generated.h"

class UButton;
class UPanelWidget;
class UTextBlock;
class UItemBase;
class UShopSellItemSlotWidget;
class UInventoryComponent;
class UEconomyComponent;

UCLASS()
class COZYWOOD_API UShopSellWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnSlotSelected(UShopSellItemSlotWidget* SelectedSlot);

	// Called by slot widget's cancel button
	void OnSlotDeselected();

	// Called by slot widget's sell button
	void ExecuteSell(UItemBase* Item, int32 Price);

	// Called explicitly when the widget is shown (also fires from NativeConstruct)
	void RebuildSellList();

protected:
	virtual void NativeConstruct() override;

	// Catches clicks on areas not consumed by child buttons (empty widget space)
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnClickedBack();

	UFUNCTION()
	void OnClickedClose();

	// Bind a transparent background button in the Blueprint for reliable "click elsewhere" deselection
	UFUNCTION()
	void OnClickedDeselectZone();

	UFUNCTION()
	void HandleMoneyChanged(int32 NewMoneyAmount);

	void RefreshMoneyText();
	void DeselectCurrentSlot();

	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<UShopSellItemSlotWidget> ShopSellItemSlotWidgetClass;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* CloseButton;

	// Optional: place a transparent full-area button behind the item list in Blueprint
	// to catch "click elsewhere" events inside a ScrollBox
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* DeselectZoneButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UPanelWidget* ItemListWrapBox;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MoneyText;

private:
	UPROPERTY()
	UInventoryComponent* CachedInventory = nullptr;

	UPROPERTY()
	UEconomyComponent* CachedEconomy = nullptr;

	UPROPERTY()
	UShopSellItemSlotWidget* CurrentlySelectedSlot;
};
