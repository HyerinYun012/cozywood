#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

class UButton;
class UPanelWidget;
class UTextBlock;

UCLASS()
class COZYWOOD_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BuyButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SellButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UPanelWidget* BuyPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UPanelWidget* SellPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MoneyText;

private:
	UFUNCTION()
	void OnClickedBuy();

	UFUNCTION()
	void OnClickedSell();

	UFUNCTION()
	void OnClickedClose();

	void ShowBuyPanel();
	void ShowSellPanel();
};
