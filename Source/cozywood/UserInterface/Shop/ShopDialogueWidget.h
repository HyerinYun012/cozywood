#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopDialogueWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class COZYWOOD_API UShopDialogueWidget : public UUserWidget
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
	UTextBlock* DialogueText;

	UFUNCTION()
	void OnClickedBuy();

	UFUNCTION()
	void OnClickedSell();

	UFUNCTION()
	void OnClickedClose();
};
