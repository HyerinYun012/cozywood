#include "PlantBase.h"
#include "Components/StaticMeshComponent.h"

// ⭐️ 1. 생성자 (에러 LNK2019의 주범! 몸통을 만들어줍니다)
APlantBase::APlantBase()
{
	// 식물은 보통 매 프레임 계산(Tick)이 필요 없으니 꺼둡니다 (최적화)
	PrimaryActorTick.bCanEverTick = false;

	// 실제로 메쉬를 담을 그릇(컴포넌트)을 생성합니다.
	PlantMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComp"));

	// 이 메쉬를 이 액터의 중심(Root)으로 설정합니다.
	RootComponent = PlantMeshComp;
}

// 2. 게임 시작 시 실행될 함수 (선언만 있고 몸통이 없음)
void APlantBase::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작되자마자 현재 단계(씨앗 등)에 맞는 메쉬를 입혀줍니다.
	UpdatePlantMesh();
}


void APlantBase::ReceiveWater()
{
	if (CurrentState == EPlantState::Growing)
	{
		// 즉시 성장시키는 대신 오늘 물 먹은 기록만 남김
		bIsWateredToday = true;

		// 물 먹었다는 이펙트나 사운드를 띄우고 싶으면 이 곳에서 구현

		UE_LOG(LogTemp, Warning, TEXT("식물이 물 주기 완료 내일 성장 예정!"));
	}
}

void APlantBase::UpdatePlantMesh()
{
	if (PlantMeshComp && ItemInfo.PlantData.GrowthMeshes.IsValidIndex(CurrentGrowthStage))
	{
		PlantMeshComp->SetStaticMesh(ItemInfo.PlantData.GrowthMeshes[CurrentGrowthStage]);
	}
}

void APlantBase::OnNewDayStarted()
{
	if (CurrentState == EPlantState::FullyGrown || CurrentState == EPlantState::Withered) return;

	if (bIsWateredToday)
	{
		CurrentGrowthStage++;
		bIsWateredToday = false;
		UpdatePlantMesh();

		if (CurrentGrowthStage >= ItemInfo.PlantData.GrowthMeshes.Num() - 1)
		{
			CurrentState = EPlantState::FullyGrown;
		}
	}
	else
	{
		CurrentState = EPlantState::Withered;
		if (WitheredMesh && PlantMeshComp)
		{
			PlantMeshComp->SetStaticMesh(WitheredMesh);
		}
		UE_LOG(LogTemp, Warning, TEXT("물을 주지 않아 식물이 시들었습니다."));
	}
}

void APlantBase::InitializePlant(FItemData InItemData)
{
	// 1. 데이터 복사
	ItemInfo = InItemData;

	// 2. 초기 상태 설정
	CurrentGrowthStage = 0;
	CurrentState = EPlantState::Growing;
	bIsWateredToday = false;

	// 3. 씨앗 메쉬(0번) 바로 입히기
	UpdatePlantMesh();

	// 4. 태그 달기 (캐릭터가 물뿌리개로 인식 가능하도록 설정)
	this->Tags.Add(FName("Plant"));
}

