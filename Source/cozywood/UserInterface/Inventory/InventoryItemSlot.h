#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "InventoryItemSlot.generated.h"

class UInventoryTooltip;
class UDragItemVisual;
class UItemBase;
class UTextBlock;
class UBorder;
class UImage;

UCLASS()
class COZYWOOD_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { 
		ItemReference = ItemIn; 
		UpdateSlot();
	};
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; };
	
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void UpdateSlot();

protected:
	UPROPERTY(VisibleAnyWhere, Category = "Inventory Slot", meta = (BindWidget))
	class UButton* Button_0;

	UPROPERTY()
	class UInventoryTooltip* CreatedTooltip;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip > ToolTipClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory Slot")
	UItemBase* ItemReference;

	UPROPERTY(VisibleAnyWhere, Category = "Inventory Slot", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnyWhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UTextBlock* ItemQuantity;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
