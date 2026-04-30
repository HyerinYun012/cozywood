#include "InventoryTooltip.h"
#include "InventoryItemSlot.h"
#include "../../Items/ItemBase.h"
#include "Components/TextBlock.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	if (!InventorySlotBeingHovered) return;

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	// 아이템 정보가 비어있어도 종료
	if (!ItemBeingHovered) return;

	ItemName->SetText(ItemBeingHovered->TextData.Name);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);

	switch (ItemBeingHovered->Category) {
		case EItemCategory::Tool:
			ItemType->SetText(FText::FromString("Tool"));

			break;
		case EItemCategory::Furniture:
			ItemType->SetText(FText::FromString("Furniture"));
			break;
		case EItemCategory::Seed:
			// 식물의 타입(나무인지 꽃인지)을 검사해서 텍스트를 다르게 설정
			if (ItemBeingHovered->PlantData.PlantType == EPlantType::Tree)
			{
				ItemType->SetText(FText::FromString(TEXT("묘목")));
			}
			else if (ItemBeingHovered->PlantData.PlantType == EPlantType::Flower)
			{
				ItemType->SetText(FText::FromString(TEXT("씨앗")));
			}
			else
			{
				// 혹시 설정이 안 되어 있을 때를 대비한 기본값
				ItemType->SetText(FText::FromString(TEXT("씨앗/묘목")));
			}
			break;
		case EItemCategory::GrownPlant:
			if (ItemBeingHovered->PlantData.PlantType == EPlantType::Tree)
			{
				ItemType->SetText(FText::FromString(TEXT("나무")));
			}
			else if (ItemBeingHovered->PlantData.PlantType == EPlantType::Flower)
			{
				ItemType->SetText(FText::FromString(TEXT("꽃")));
			}
			else
			{
				// 혹시 설정이 안 되어 있을 때를 대비한 기본값
				ItemType->SetText(FText::FromString(TEXT("식물")));
			}
			break;
		default: break;
	}
