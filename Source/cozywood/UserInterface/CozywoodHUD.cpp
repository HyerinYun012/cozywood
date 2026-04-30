#include "CozywoodHUD.h"
#include "MainMenu.h"

ACozywoodHUD::ACozywoodHUD()
{
}

void ACozywoodHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		if(MainMenuWidget)
		{
			MainMenuWidget->AddToViewport(5);
			MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


void ACozywoodHUD::DisplayMenu()
{
	if (MainMenuWidget) {
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACozywoodHUD::HideMenu()
{
	if (MainMenuWidget) {
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACozywoodHUD::ToggleMenu()
{
	if (bIsMenuVisible) {
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else {
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}