#include "GardenHousingComponent.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "CollisionQueryParams.h"
#include "Kismet/GameplayStatics.h"
#include "../Save/GardenSave.h"
#include "../Character/C1Character.h" 
#include "../Components/InventoryComponent.h"
#include "../World/ADecoFurniture.h"
#include "../Components/EconomyComponent.h"
#include "../Items/ItemBase.h"

UGardenHousingComponent::UGardenHousingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGardenHousingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGardenHousingComponent::ResetPlacementCount()
{
	PlacedFurnitureCount = 0;
	bHasReceivedDecoReward = false;

	UE_LOG(LogTemp, Log, TEXT("정원 모드 탈출: 배치 카운트가 0으로 리셋되었습니다."));
}

void UGardenHousingComponent::StartPlacement(TSubclassOf<AActor> InFurnitureClass, UStaticMesh* FurnitureMesh, class UItemBase* ItemToUse)
{
	FurnitureClassToSpawn = InFurnitureClass;
	FurnitureMeshToSpawn = FurnitureMesh;

	if (HologramActor) { HologramActor->Destroy(); }

	HologramActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	HologramMeshComp = NewObject<UStaticMeshComponent>(HologramActor);
	HologramMeshComp->RegisterComponent();
	HologramActor->SetRootComponent(HologramMeshComp);
	HologramMeshComp->SetStaticMesh(FurnitureMesh);
	HologramMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (HologramBaseMaterial)
	{
		HologramMID = UMaterialInstanceDynamic::Create(HologramBaseMaterial, this);
		HologramMeshComp->SetMaterial(0, HologramMID);
	}

	ItemBeingPlaced = ItemToUse;
}

void UGardenHousingComponent::UpdateHologram()
{
	if (!HologramActor || !HologramMID) return;

	AC1Character* OwnerCharacter = Cast<AC1Character>(GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->CameraComp) return;

	FVector TraceStart = OwnerCharacter->CameraComp->GetComponentLocation();

	float MaxPlaceDistance = 1500.0f;
	FVector TraceEnd = TraceStart + (OwnerCharacter->CameraComp->GetForwardVector() * MaxPlaceDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.AddIgnoredActor(HologramActor);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	bIsValidPlacement = false;
	FVector TargetLocation = TraceEnd;

	if (HitResult.bBlockingHit)
	{
		TargetLocation = HitResult.ImpactPoint;
		if (HitResult.ImpactNormal.Z > 0.85f)
		{
			TArray<FOverlapResult> Overlaps;
			FCollisionShape Sphere = FCollisionShape::MakeSphere(10.0f);
			GetWorld()->OverlapMultiByChannel(Overlaps, TargetLocation, FQuat::Identity, ECC_WorldDynamic, Sphere);

			for (const FOverlapResult& Overlap : Overlaps)
			{
				AActor* OverlappedActor = Overlap.GetActor();
				if (IsValid(OverlappedActor) && OverlappedActor->ActorHasTag(FName("GardenZone")))
				{
					bIsValidPlacement = true;
					break;
				}
			}
		}
	}
	else
	{
		FVector CameraLoc; FRotator CameraRot;
		OwnerCharacter->GetActorEyesViewPoint(CameraLoc, CameraRot);
		TargetLocation = CameraLoc + (CameraRot.Vector() * MaxPlaceDistance);
	}

	HologramActor->SetActorLocation(TargetLocation);

	if (bIsValidPlacement)
	{
		HologramMID->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 1.0f, 0.0f, 0.5f));
		bCanPlace = true;
	}
	else
	{
		HologramMID->SetVectorParameterValue(FName("Color"), FLinearColor(1.0f, 0.0f, 0.0f, 0.5f));
		bCanPlace = false;
	}
}

void UGardenHousingComponent::ConfirmPlacement()
{
	if (!HologramActor || !FurnitureClassToSpawn || !bIsValidPlacement) return;

	FVector SpawnLocation = HologramActor->GetActorLocation();
	FRotator SpawnRotation = HologramActor->GetActorRotation();

	AActor* NewFurniture = GetWorld()->SpawnActor<AActor>(FurnitureClassToSpawn, SpawnLocation, SpawnRotation);

	if (NewFurniture)
	{
		if (AADecoFurniture* DecoItem = Cast<AADecoFurniture>(NewFurniture))
		{
			DecoItem->SetFurnitureData(FurnitureMeshToSpawn, ItemBeingPlaced->ItemID);
		}

		// 인벤토리 컴포넌트에서 아이템 가구 하나 차감
		AC1Character* OwnerCharacter = Cast<AC1Character>(GetOwner());
		if (OwnerCharacter && OwnerCharacter->GetInventory())
		{
			OwnerCharacter->GetInventory()->RemoveItemByActorClass(FurnitureClassToSpawn);
		}

		UE_LOG(LogTemp, Log, TEXT("가구 배치 성공 및 인벤토리 차감 완료!"));
		
		PlacedFurnitureCount++;
		UE_LOG(LogTemp, Log, TEXT("현재 배치된 가구 수: %d / %d"), PlacedFurnitureCount, TargetDecoCount);

		// 목표 개수에 도달했을 때 보상 지급(정원 모드에서 10개 이상 배치 시, 한 번만)
		if (PlacedFurnitureCount >= TargetDecoCount && !bHasReceivedDecoReward)
		{
			// EconomyComponent에서 돈 주는 함수 만들어서 호출하기
			if (OwnerCharacter)
			{
				OwnerCharacter->ShowGardenRecordPrompt();
				UE_LOG(LogTemp, Warning, TEXT("10개 달성! [단축키]를 눌러 기록하세요."))
			}
		}
		CancelPlacement();
	}
}

void UGardenHousingComponent::RotateHologramToggle()
{
	if (HologramActor)
	{
		bIsWheelRotationEnabled = !bIsWheelRotationEnabled;
		UE_LOG(LogTemp, Log, TEXT("휠 회전 모드: %s"), bIsWheelRotationEnabled ? TEXT("활성화") : TEXT("비활성화"));
	}
}

void UGardenHousingComponent::RotateHologramWheel(float WheelValue)
{
	if (HologramActor && bIsWheelRotationEnabled)
	{
		FRotator CurrentRotation = HologramActor->GetActorRotation();
		float RotationSpeed = 10.0f;
		CurrentRotation.Yaw += (WheelValue * RotationSpeed);
		HologramActor->SetActorRotation(CurrentRotation);
	}
}

void UGardenHousingComponent::CancelPlacement()
{
	if (HologramActor)
	{
		HologramActor->Destroy();
		HologramActor = nullptr;
	}
	HologramMID = nullptr;
	FurnitureClassToSpawn = nullptr;
	bIsValidPlacement = false;
	bIsWheelRotationEnabled = false;
	bCanPlace = false;
}

void UGardenHousingComponent::FinishNamingAndGiveReward()
{
	if (bHasReceivedDecoReward) return; // 이미 받았으면 무시

	bHasReceivedDecoReward = true;
	UE_LOG(LogTemp, Warning, TEXT("정원 이름 짓기 완료!"));

	AC1Character* OwnerCharacter = Cast<AC1Character>(GetOwner());
	if (OwnerCharacter && OwnerCharacter->GetEconomyComp())
	{
		OwnerCharacter->GetEconomyComp()->AddMoney(5000); // 돈 지급
	}
}

// 정원 배치 상태를 저장
void UGardenHousingComponent::SaveGarden()
{
	UGardenSave* SaveGameInstance = Cast<UGardenSave>(UGameplayStatics::CreateSaveGameObject(UGardenSave::StaticClass()));

	// 맵에 있는 모든 가구(ADecoFurniture)를 싹 다 찾는다. 
	TArray<AActor*> FoundFurnitures;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AADecoFurniture::StaticClass(), FoundFurnitures);

	// 찾은 가구들의 정보를 기록
	for (AActor* Actor : FoundFurnitures)
	{
		AADecoFurniture* Furniture = Cast<AADecoFurniture>(Actor);
		if (Furniture)
		{
			FSavedFurnitureData Data;
			Data.ItemID = Furniture->FurnitureItemID; // 아이템 ID
			Data.FurnitureTransform = Furniture->GetActorTransform(); // 위치와 회전 값
			SaveGameInstance->SavedFurnitures.Add(Data);
		}
	}

	// 위의 아이템 배치 상태들을 컴퓨터 하드디스크에 저장
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	UE_LOG(LogTemp, Warning, TEXT("정원 저장 완료! 총 %d개 가구 저장됨."), SaveGameInstance->SavedFurnitures.Num());
}

void UGardenHousingComponent::LoadGarden()
{
	// 하드디스크에 가구 상태 기록을 확인하고, 있으면 가져옵니다.
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		UGardenSave* LoadGameInstance = Cast<UGardenSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (LoadGameInstance && ItemDB)
		{
			// 혹시 기존에 남아있는 가구가 있다면 지움 (겹침 방지)
			TArray<AActor*> OldFurnitures;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AADecoFurniture::StaticClass(), OldFurnitures);
			for (AActor* OldActor : OldFurnitures)
			{
				OldActor->Destroy();
			}

			// 기록을 보면서 하나씩 소환
			for (const FSavedFurnitureData& SavedData : LoadGameInstance->SavedFurnitures)
			{
				// 이름표(ItemID)로 데이터 테이블에서 가구 정보를 찾습니다.
				FItemData* ItemData = ItemDB->FindRow<FItemData>(SavedData.ItemID, TEXT("LoadFurniture"));

				if (ItemData && ItemData->VisualData.ActorClass)
				{
					// 저장된 아이템 위치(Transform)에 해당 액터를 생성
					AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemData->VisualData.ActorClass, SavedData.FurnitureTransform);

					// 생성된 껍데기에 메쉬랑 이름표를 다시 달아주기
					AADecoFurniture* NewFurniture = Cast<AADecoFurniture>(SpawnedActor);
					if (NewFurniture)
					{
						NewFurniture->SetFurnitureData(ItemData->VisualData.Mesh, SavedData.ItemID);
					}
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("정원 불러오기 완료! 총 %d개 가구 배치됨."), LoadGameInstance->SavedFurnitures.Num());
		}
	}
}