#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopSellItemSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UWidget;
class UItemBase;
class UShopSellWidget;

UCLASS()
class COZYWOOD_API UShopSellItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitSlot(UItemBase* InItem, int32 InSellPrice, UShopSellWidget* InOwner);

	void SetActionBoxVisible(bool bVisible);

protected:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;

	UPROPERTY(meta = (BindWidget))
	UWidget* ActionBox;

	UPROPERTY(meta = (BindWidget))
	UButton* SellConfirmButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SellCancelButton;

private:
	UFUNCTION()
	void HandleSlotClicked();

	UFUNCTION()
	void HandleSellConfirm();

	UFUNCTION()
	void HandleSellCancel();

	UPROPERTY()
	UItemBase* ItemRef;

	UPROPERTY()
	UShopSellWidget* SellOwner;

	int32 CachedPrice;
};