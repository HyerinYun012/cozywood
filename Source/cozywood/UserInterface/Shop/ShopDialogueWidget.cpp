#include "ShopDialogueWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../Player/C1PlayerController.h"

void UShopDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (DialogueText)
	{
		DialogueText->SetText(FText::FromString(TEXT("삐빅- 여기는 각종 다양한 정원 아이템을 판매 및 구매 가능한 상점입니다. 무엇을 도와드릴까요?")));
	}

	if (BuyButton)
	{
		BuyButton->OnClicked.AddDynamic(this, &UShopDialogueWidget::OnClickedBuy);
	}

	if (SellButton)
	{
		SellButton->OnClicked.AddDynamic(this, &UShopDialogueWidget::OnClickedSell);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UShopDialogueWidget::OnClickedClose);
	}
}

void UShopDialogueWidget::OnClickedBuy()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->OpenShopBuy();
	}
}

void UShopDialogueWidget::OnClickedSell()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->OpenShopSell();
	}
}

void UShopDialogueWidget::OnClickedClose()
{
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
	{
		PC->CloseShopUI();
	}
}