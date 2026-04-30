#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopSellWidget.generated.h"

class UButton;
class UWrapBox;
class UTextBlock;
class UImage;
class UTexture2D;
class UItemBase;
class UShopItemSlotWidget;
class UInventoryComponent;
class UEconomyComponent;
class UWidget;

UCLASS()
class COZYWOOD_API UShopSellWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SelectInventoryItem(UItemBase* InItem, const FText& InName, const FText& InDescription, int32 InPrice, UTexture2D* InIcon);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedBack();

	UFUNCTION()
	void OnClickedClose();

	UFUNCTION()
	void OnClickedSell();

	UFUNCTION()
	void OnClickedCancelSelection();

	UFUNCTION()
	void HandleMoneyChanged(int32 NewMoneyAmount);

	void RebuildSellList();
	void RefreshMoneyText();
	void ShowSelectedPanel(bool bShow);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UShopItemSlotWidget> ShopItemSlotWidgetClass;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SellButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* CancelSelectionButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UWrapBox* ItemListWrapBox;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MoneyText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SelectedItemNameText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SelectedItemDescriptionText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SelectedItemPriceText;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* SelectedItemImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SellButtonText;

	UPROPERTY(meta = (BindWidgetOptional))
	UWidget* SelectedDetailBox;

private:
	UPROPERTY()
	UInventoryComponent* CachedInventory = nullptr;

	UPROPERTY()
	UEconomyComponent* CachedEconomy = nullptr;

	UPROPERTY()
	UItemBase* SelectedInventoryItem = nullptr;

	int32 SelectedPrice = 0;
};