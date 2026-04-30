#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class AC1Character;
class UWrapBox;
class UTextBlock;
class UInventoryComponent;
class UInventoryItemSlot;
class UUniformGridPanel;
class UUniformGridSlot;

UCLASS()
class COZYWOOD_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshInventory();

	UPROPERTY(meta=(BindWidget))
	class UUniformGridPanel* InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CapacityInfo;


	UPROPERTY()
	AC1Character* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;
protected:
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
