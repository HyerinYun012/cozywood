#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopModeSelectWidget.generated.h"

class UButton;

UCLASS()
class COZYWOOD_API UShopModeSelectWidget : public UUserWidget
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

	UFUNCTION()
	void OnClickedBuy();

	UFUNCTION()
	void OnClickedSell();

	UFUNCTION()
	void OnClickedClose();
};
