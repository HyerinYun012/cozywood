#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../ItemData/ItemDataStructs.h"
#include "ShopItemSlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UWidget;
class UTexture2D;
class UItemBase;
class UShopBuyWidget;

UCLASS()
class COZYWOOD_API UShopItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Buy slot: rich card with all item info + inline buy button
	void InitBuySlot(
		const FName& InItemID,
		const FText& InName,
		const FText& InDescription,
		const FText& InUsageText,
		EItemCategory InCategory,
		int32 InPrice,
		UTexture2D* InIcon,
		UShopBuyWidget* InOwner
	);

	// void SetActionBoxVisible(bool bVisible);

	UItemBase* GetItemRef() const { return ItemRef; }
	FName GetItemID() const { return ItemID; }
	int32 GetPrice() const { return CachedPrice; }
	int32 GetCachedPrice() const { return CachedPrice; }

protected:
	virtual void NativeConstruct() override;

	// ---- Common bindings ----
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SlotButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PriceText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* UsageText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CategoryText;

	// Panel containing InlineBuyButton + InlineCancelButton (collapsed by default)
	// UPROPERTY(meta = (BindWidgetOptional))
	// UWidget* ActionBox;

	// Confirm button: executes Buy or Sell depending on slot type
	// UPROPERTY(meta = (BindWidgetOptional))
	// UButton* InlineBuyButton;

	// UPROPERTY(meta = (BindWidgetOptional))
	// UButton* InlineCancelButton;

private:
	UFUNCTION()
	void HandleClicked();

	// UFUNCTION()
	// void HandleInlineBuy();

	// UFUNCTION()
	// void HandleInlineCancel();

	void ApplyVisuals();

	UPROPERTY()
	UItemBase* ItemRef = nullptr;

	UPROPERTY()
	UTexture2D* IconTexture = nullptr;

	UPROPERTY()
	UShopBuyWidget* BuyOwner = nullptr;

	FName ItemID = NAME_None;
	FText CachedName;
	FText CachedDescription;
	FText CachedUsageText;
	FText CachedCategory;
	int32 CachedPrice = 0;
};
