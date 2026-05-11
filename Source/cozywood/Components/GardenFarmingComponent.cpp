#include "GardenFarmingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "../Character/C1Character.h"
#include "../World/PlantBase.h"
#include "../Components/InventoryComponent.h"
#include "../Items/ItemBase.h"
#include "../Save/GardenSave.h"

void UGardenFarmingComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AC1Character>(GetOwner());

	FTimerHandle LoadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(LoadTimerHandle, this, &UGardenFarmingComponent::LoadPlants, 0.2f, false);
}

UGardenFarmingComponent::UGardenFarmingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGardenFarmingComponent::SavePlants()
{
	FString SaveSlotName = TEXT("GardenSaveSlot"); // 가구/인벤토리와 같은 슬롯 이름
	UGardenSave* SaveInstance = Cast<UGardenSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveInstance) { SaveInstance = Cast<UGardenSave>(UGameplayStatics::CreateSaveGameObject(UGardenSave::StaticClass())); }

	SaveInstance->SavedPlants.Empty(); // 기존 기록 초기화

	// 월드에 있는 모든 식물을 찾아서 기록
	TArray<AActor*> AllPlants;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlantBase::StaticClass(), AllPlants);

	for (AActor* Actor : AllPlants)
	{
		if (APlantBase* Plant = Cast<APlantBase>(Actor))
		{
			FSavedPlantData PlantData;
			PlantData.ItemID = Plant->ItemInfo.ItemID;
			PlantData.Location = Plant->GetActorLocation();
			PlantData.GrowthDay = Plant->CurrentGrowthDay;
			PlantData.State = static_cast<uint8>(Plant->CurrentState);
			PlantData.bWatered = Plant->bIsWateredToday;

			PlantData.LastSavedInGameDay = OwnerCharacter->GetCurrentInGameDay();

			SaveInstance->SavedPlants.Add(PlantData);
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, 0);
	UE_LOG(LogTemp, Log, TEXT("식물 %d개 세이브 완료"), SaveInstance->SavedPlants.Num());
}

void UGardenFarmingComponent::LoadPlants()
{
	if (!PlantActorClass || !OwnerCharacter || !OwnerCharacter->PlayerInventory) return;

	// 로드 시점의 현재 날짜를 기억
	LastTrackedDay = OwnerCharacter->GetCurrentInGameDay();

	FString SaveSlotName = TEXT("GardenSaveSlot");
	UGardenSave* SaveInstance = Cast<UGardenSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveInstance) return;

	for (const FSavedPlantData& SavedData : SaveInstance->SavedPlants)
	{
		FActorSpawnParameters SpawnParams;
		APlantBase* NewPlant = GetWorld()->SpawnActor<APlantBase>(PlantActorClass, SavedData.Location, FRotator::ZeroRotator, SpawnParams);

		if (NewPlant)
		{
			FItemData* RowData = OwnerCharacter->PlayerInventory->ItemDataTable->FindRow<FItemData>(SavedData.ItemID, TEXT("PlantLoad"));
			if (RowData)
			{
				NewPlant->InitializePlant(*RowData);
				NewPlant->RestorePlantState(SavedData.GrowthDay, SavedData.State, SavedData.bWatered);

				int32 CurrentInGameDay = OwnerCharacter->GetCurrentInGameDay();
				NewPlant->CheckTimeTravelAndGrowth(SavedData.LastSavedInGameDay, CurrentInGameDay);
			}
			else { NewPlant->Destroy(); }
		}
	}
	UE_LOG(LogTemp, Log, TEXT("식물 %d개 로드 완료"), SaveInstance->SavedPlants.Num());
}

void UGardenFarmingComponent::StartPlanting(UItemBase* SeedItem)
{
	if (!SeedItem || !OwnerCharacter || !PlantActorClass) return;

	// ---------------------------------------------------------
	// [1단계] 정확한 바닥 위치 찾기 (LineTrace)
	// ---------------------------------------------------------
	FVector ForwardLoc = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 150.f);

	// 머리 위 200에서 바닥 밑 500까지 레이저 쏘기
	FVector TraceStart = ForwardLoc + FVector(0.f, 0.f, 200.f);
	FVector TraceEnd = ForwardLoc - FVector(0.f, 0.f, 500.f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter); // 플레이어 무시

	FVector FinalSpawnLocation;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		// 진짜 바닥 타격점을 위치로 설정
		FinalSpawnLocation = HitResult.ImpactPoint;

		FinalSpawnLocation.Z -= 15.f;
	}
	else
	{
		// 바닥을 전혀 못 찾는 허공이라면 심기 취소
		UE_LOG(LogTemp, Warning, TEXT("심기 실패: 바닥을 찾을 수 없는 공간"));
		OwnerCharacter->ShowPlantOverlapErrorUI(); // 오류 위젯 호출
		return;
	}

	// ---------------------------------------------------------
	// [2단계] 겹침 검사
	// ---------------------------------------------------------
	// 심으려는 위치 주변에 이미 다른 식물이 있는지 가상의 구체를 그려 검사
	FCollisionShape OverlapSphere = FCollisionShape::MakeSphere(50.f);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams OverlapQueryParams;
	OverlapQueryParams.AddIgnoredActor(OwnerCharacter); // 플레이어 무시

	bool bIsOverlapped = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		FinalSpawnLocation + FVector(0.f, 0.f, 50.f), // 바닥에서 살짝 위에서 검사
		FQuat::Identity,
		ECC_Visibility,
		OverlapSphere,
		OverlapQueryParams
	);

	if (bIsOverlapped)
	{
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			AActor* OverlappedActor = Overlap.GetActor();
			if (OverlappedActor && (OverlappedActor->ActorHasTag(FName("GardenItem")) || OverlappedActor->IsA(APlantBase::StaticClass())))
			{
				UE_LOG(LogTemp, Warning, TEXT("심기 실패: 다른 식물이나 가구와 겹침"));

				// 캐릭터에게 오류 위젯 띄우라고 명령
				OwnerCharacter->ShowPlantOverlapErrorUI();
				return;
			}
		}
	}


	// ---------------------------------------------------------
	// [3단계] 최종 심기
	// ---------------------------------------------------------
	FActorSpawnParameters SpawnParams;
	APlantBase* NewPlant = GetWorld()->SpawnActor<APlantBase>(PlantActorClass, FinalSpawnLocation, FRotator::ZeroRotator, SpawnParams);

	if (NewPlant)
	{
		if (OwnerCharacter->PlayerInventory && OwnerCharacter->PlayerInventory->ItemDataTable)
		{
			FItemData* RowData = OwnerCharacter->PlayerInventory->ItemDataTable->FindRow<FItemData>(SeedItem->ItemID, TEXT("Planting"));

			if (RowData)
			{
				NewPlant->InitializePlant(*RowData);

				// 심기 성공했으니 세이브 파일 갱신
				SavePlants();

				// 인벤토리에서 씨앗 소모
				OwnerCharacter->PlayerInventory->RemoveAmountOfItem(SeedItem, 1);
			}
			else
			{
				NewPlant->Destroy(); 
			}
		}
	}
}

void UGardenFarmingComponent::NotifyDayPassed()
{
	if (!OwnerCharacter) return;

	int32 CurrentDay = OwnerCharacter->GetCurrentInGameDay();

	if (LastTrackedDay == CurrentDay || LastTrackedDay == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("잘못된 날짜 변경 알람을 차단했습니다."));
		return;
	}

	LastTrackedDay = CurrentDay;

	TArray<AActor*> AllPlants;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlantBase::StaticClass(), AllPlants);

	for (AActor* Actor : AllPlants)
	{
		if (APlantBase* Plant = Cast<APlantBase>(Actor))
		{
			Plant->OnNewDayStarted();
		}
	}
	SavePlants();
}