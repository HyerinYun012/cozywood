#include "ShopModeSelectWidget.h"
#include "Components/Button.h"
#include "../../Player/C1PlayerController.h"

void UShopModeSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuyButton)
	{
		BuyButton->OnClicked.AddDynamic(this, &UShopModeSelectWidget::OnClickedBuy);
	}

	if (SellButton)
	{
		SellButton->OnClicked.AddDynamic(this, &UShopModeSelectWidget::OnClickedSell);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UShopModeSelectWidget::OnClickedClose);
	}
}

void UShopModeSelectWidget::OnClickedBuy()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->OpenShopBuy();
	}
}

void UShopModeSelectWidget::OnClickedSell()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->OpenShopSell();
	}
}

void UShopModeSelectWidget::OnClickedClose()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->CloseShopUI();
	}
}