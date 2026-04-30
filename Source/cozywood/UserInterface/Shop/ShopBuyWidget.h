#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopBuyWidget.generated.h"

class UButton;
class UWrapBox;
class UTextBlock;
class UImage;
class UTexture2D;
class UShopItemSlotWidget;
class UInventoryComponent;
class UEconomyComponent;
class UWidget;

UCLASS()
class COZYWOOD_API UShopBuyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SelectShopItem(const FName& InItemID, const FText& InName, const FText& InDescription, int32 InPrice, UTexture2D* InIcon);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedBack();

	UFUNCTION()
	void OnClickedClose();

	UFUNCTION()
	void OnClickedBuy();

	UFUNCTION()
	void OnClickedCancelSelection();

	UFUNCTION()
	void HandleMoneyChanged(int32 NewMoneyAmount);

	void RebuildShopList();
	void RefreshMoneyText();
	void ShowSelectedPanel(bool bShow);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FName> ShopItemIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UShopItemSlotWidget> ShopItemSlotWidgetClass;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BuyButton;

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
	UTextBlock* BuyButtonText;

	UPROPERTY(meta = (BindWidgetOptional))
	UWidget* SelectedDetailBox;

private:
	UPROPERTY()
	UInventoryComponent* CachedInventory = nullptr;

	UPROPERTY()
	UEconomyComponent* CachedEconomy = nullptr;

	FName SelectedItemID = NAME_None;
	int32 SelectedPrice = 0;
};