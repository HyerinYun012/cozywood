#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemSlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UTexture2D;
class UItemBase;
class UShopBuyWidget;
class UShopSellWidget;

UCLASS()
class COZYWOOD_API UShopItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitBuySlot(
		const FName& InItemID,
		const FText& InName,
		const FText& InDescription,
		int32 InPrice,
		UTexture2D* InIcon,
		UShopBuyWidget* InOwner
	);

	void InitSellSlot(
		UItemBase* InItem,
		const FText& InName,
		const FText& InDescription,
		int32 InPrice,
		UTexture2D* InIcon,
		UShopSellWidget* InOwner
	);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SlotButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PriceText;

	UFUNCTION()
	void HandleClicked();

	void ApplyVisuals();

private:
	UPROPERTY()
	UItemBase* ItemRef = nullptr;

	UPROPERTY()
	UTexture2D* IconTexture = nullptr;

	UPROPERTY()
	UShopBuyWidget* BuyOwner = nullptr;

	UPROPERTY()
	UShopSellWidget* SellOwner = nullptr;

	FName ItemID = NAME_None;
	FText CachedName;
	FText CachedDescription;
	int32 CachedPrice = 0;
	bool bIsSellSlot = false;
};
