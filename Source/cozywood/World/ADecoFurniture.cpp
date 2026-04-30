// Fill out your copyright notice in the Description page of Project Settings.


#include "ADecoFurniture.h"
#include "../Components/InventoryComponent.h"
#include "../Character/C1Character.h"

// Sets default values
AADecoFurniture::AADecoFurniture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FurnitureMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FurnitureMeshComp"));
	RootComponent = FurnitureMeshComp;
}

// Called when the game starts or when spawned
void AADecoFurniture::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AADecoFurniture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInteractableData AADecoFurniture::GetInteractableData() const
{
	FInteractableData Data;
	Data.InteractableType = EInteractableType::EIT_Pickup; // 가구 회수용
	Data.Name = FText::FromString(TEXT("가구"));
	return Data;
}


// 가구 외형을 가져오는 방식은 바꿀 예정(최적화 관점에서 수정 필요)
void AADecoFurniture::SetFurnitureData(UStaticMesh* NewMesh, FName ItemID)
{
	if (FurnitureMeshComp && NewMesh)
	{
		// 껍데기에 데이터 테이블에서 가져온 알맹이(메쉬)를 씌운다
		FurnitureMeshComp->SetStaticMesh(NewMesh);

		// 스폰될 때 ID 기록
		FurnitureItemID = ItemID;
	}

}

void AADecoFurniture::Interact_Implementation(AC1Character* PlayerCharacter)
{
	if (!PlayerCharacter || !PlayerCharacter->bIsGardenMode) return;

	// 도구를 들고 있지 않을 때
	if (PlayerCharacter->CurrentEquippedToolType == EItemToolType::EITT_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("가구를 회수하여 인벤토리에 넣습니다."));

		if (UInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory())
		{
			PlayerInventory->AddStartingItemByID(FurnitureItemID, 1);
		}
		Destroy(); 
	}
	// 도구(물뿌리개 등)를 들고 있으면 아무 반응 안 함
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("도구를 들고 있어서 가구를 주울 수 없습니다."));
	}
}