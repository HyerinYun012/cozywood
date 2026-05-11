#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopBuyWidget.generated.h"

class UButton;
class UPanelWidget;
class UTextBlock;
class UShopItemSlotWidget;
class UInventoryComponent;
class UEconomyComponent;

UCLASS()
class COZYWOOD_API UShopBuyWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UInventoryComponent* CachedInventory = nullptr;

	UPROPERTY()
	UEconomyComponent* CachedEconomy = nullptr;

	UPROPERTY()
	class UShopItemSlotWidget* CurrentlySelectedSlot;

public:
	// Called by slot widget's inline buy button
	void ExecuteBuy(const FName& ItemID, int32 Price);

	// Called explicitly when the widget is shown (also fires from NativeConstruct)
	void RebuildShopList();
	void OnSlotSelected(class UShopItemSlotWidget* SelectedSlot);
	void DeselectCurrentSlot();

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnClickedBack();

	UFUNCTION()
	void OnClickedClose();

	UFUNCTION()
	void HandleMoneyChanged(int32 NewMoneyAmount);

	void RefreshMoneyText();

	// Item IDs to display in this shop (set in Blueprint defaults)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FName> ShopItemIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UShopItemSlotWidget> ShopItemSlotWidgetClass;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UPanelWidget* ItemListWrapBox;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MoneyText;

	UPROPERTY(meta = (BindWidget))
	class UButton* InlineBuyButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* InlineCancelButton;

	UFUNCTION()
	void OnClickedInlineBuy();

	UFUNCTION()
	void OnClickedInlineCancel();
};
