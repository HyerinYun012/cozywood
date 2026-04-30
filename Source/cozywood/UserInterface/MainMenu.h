#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/C1Character.h"
#include "MainMenu.generated.h"


UCLASS()
class COZYWOOD_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
public:
	UPROPERTY()
	AC1Character* PlayerCharacter;
};
