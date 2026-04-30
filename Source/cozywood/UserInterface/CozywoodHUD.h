#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CozywoodHUD.generated.h"

class UMainMenu;

UCLASS()
class COZYWOOD_API ACozywoodHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	bool bIsMenuVisible;

	ACozywoodHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();


protected:
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	virtual void BeginPlay() override;
};
