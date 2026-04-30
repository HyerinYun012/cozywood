#include "ToolManagerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Items/ItemBase.h"
#include "../ItemData/ItemDataStructs.h"

UToolManagerComponent::UToolManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsToolEquipped = false;
}

void UToolManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UToolManagerComponent::EquipTool(UItemBase* ToolItem, UStaticMeshComponent* TargetMeshComp)
{
	if (!ToolItem || !TargetMeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("도구 데이터 혹은 목표 메시 컴포넌트가 유효하지 않습니다."));
		return;
	}

	UStaticMesh* ToolStaticMesh = ToolItem->VisualData.Mesh;

	if (ToolStaticMesh)
	{
		// 모델링 세팅
		TargetMeshComp->SetStaticMesh(ToolStaticMesh);

		// 장착 상태 ON
		bIsToolEquipped = true;

		// 숨겨뒀던 모델링을 화면에 보이게 켬
		TargetMeshComp->SetVisibility(true);

		UE_LOG(LogTemp, Log, TEXT("%s 도구 장착 완료!"), *ToolItem->ItemID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s 도구에 스태틱 메시 데이터가 없습니다."), *ToolItem->ItemID.ToString());
	}
}