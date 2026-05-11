#include "ShopWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "../../Player/C1PlayerController.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuyButton)
	{
		BuyButton->OnClicked.AddDynamic(this, &UShopWidget::OnClickedBuy);
	}

	if (SellButton)
	{
		SellButton->OnClicked.AddDynamic(this, &UShopWidget::OnClickedSell);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UShopWidget::OnClickedClose);
	}

	ShowBuyPanel();
}

void UShopWidget::OnClickedBuy()
{
	ShowBuyPanel();
}

void UShopWidget::OnClickedSell()
{
	ShowSellPanel();
}

void UShopWidget::OnClickedClose()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->CloseShopUI();
	}
}

void UShopWidget::ShowBuyPanel()
{
	if (BuyPanel)
	{
		BuyPanel->SetVisibility(ESlateVisibility::Visible);
	}
	if (SellPanel)
	{
		SellPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UShopWidget::ShowSellPanel()
{
	if (BuyPanel)
	{
		BuyPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (SellPanel)
	{
		SellPanel->SetVisibility(ESlateVisibility::Visible);
	}
}
