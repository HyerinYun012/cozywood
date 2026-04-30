#include "GardenFarmingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "../Character/C1Character.h"
#include "../World/PlantBase.h"
#include "../Components/InventoryComponent.h"
#include "../Items/ItemBase.h"

UGardenFarmingComponent::UGardenFarmingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGardenFarmingComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AC1Character>(GetOwner());
}

void UGardenFarmingComponent::StartPlanting(UItemBase* SeedItem)
{
	if (!SeedItem || !OwnerCharacter || !OwnerCharacter->PlayerInventory) return;

	// 1. 심을 위치 계산
	FVector SpawnLocation = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 150.f);
	SpawnLocation.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// 2. 스폰 클래스 결정
	TSubclassOf<AActor> PlantClass = SeedItem->VisualData.ActorClass;
	if (!PlantClass) PlantClass = APlantBase::StaticClass();

	FActorSpawnParameters SpawnParams;
	APlantBase* NewPlant = GetWorld()->SpawnActor<APlantBase>(PlantClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	if (NewPlant)
	{
		// 3. 데이터 테이블에서 정보 찾기
		static const FString ContextString(TEXT("Planting"));
		FItemData* RowData = OwnerCharacter->PlayerInventory->ItemDataTable->FindRow<FItemData>(SeedItem->ItemID, ContextString);

		if (RowData)
		{
			NewPlant->InitializePlant(*RowData);

			// 4. 씨앗 소모
			OwnerCharacter->PlayerInventory->RemoveAmountOfItem(SeedItem, 1);
			UE_LOG(LogTemp, Warning, TEXT("%s 심기 성공!"), *SeedItem->TextData.Name.ToString());
		}
	}
}

// ⭐️ 블루프린트 시간 시스템에서 호출할 함수
void UGardenFarmingComponent::NotifyDayPassed()
{
	TArray<AActor*> AllPlants;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlantBase::StaticClass(), AllPlants);

	for (AActor* Actor : AllPlants)
	{
		if (APlantBase* Plant = Cast<APlantBase>(Actor))
		{
			Plant->OnNewDayStarted();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("모든 식물의 하루 성장 로직이 실행되었습니다."));
}