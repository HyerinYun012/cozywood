#include "PlantBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/C1Character.h"
#include "../Components/GardenFarmingComponent.h"
#include "../Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Items/ItemBase.h"

APlantBase::APlantBase()
{
    PrimaryActorTick.bCanEverTick = false;
    PlantMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComp"));
    RootComponent = PlantMeshComp;

    PlantMeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void APlantBase::BeginPlay()
{
    Super::BeginPlay();
}

// 처음 씨앗을 심었을 때 세팅
void APlantBase::InitializePlant(FItemData InItemData)
{
    ItemInfo = InItemData;
    CurrentGrowthDay = 0;
    CurrentState = EPlantState::Growing;
    bIsWateredToday = false;

    this->Tags.Add(FName("Plant"));
    this->Tags.Add(FName("GardenItem"));

    UpdatePlantMesh(); // 모든 데이터 세팅 후 메시 입히기
}

// 물뿌리개로 E 눌렀을 때
void APlantBase::ReceiveWater()
{
    if (CurrentState == EPlantState::Growing && !bIsWateredToday)
    {
        bIsWateredToday = true; // 1. 물 마심 상태로 변경
        UE_LOG(LogTemp, Warning, TEXT("식물이 물을 마셨습니다! 내일 성장합니다."));

        // 2. 물을 마셨다는 사실을 잃어버리지 않게 즉시 저장!
        if (AC1Character* PlayerChar = Cast<AC1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
        {
            // 캐릭터에게서 파밍 컴포넌트를 찾아 강제 저장 명령
            if (UGardenFarmingComponent* FarmingComp = PlayerChar->FindComponentByClass<UGardenFarmingComponent>())
            {
                FarmingComp->SavePlants();
                UE_LOG(LogTemp, Log, TEXT("물 준 상태가 안전하게 저장되었습니다."));
            }
        }
    }
}

// 하루가 지났을 때의 판정
void APlantBase::OnNewDayStarted()
{
    if (CurrentState == EPlantState::FullyGrown || CurrentState == EPlantState::Withered) return;

    if (bIsWateredToday)
    {
        // 물을 줬다면 1일 성장
        CurrentGrowthDay++;
        bIsWateredToday = false; // 초기화

        // 유저가 설정한 GrowthDays에 도달했는가?
        if (CurrentGrowthDay >= ItemInfo.PlantData.GrowthDays)
        {
            CurrentState = EPlantState::FullyGrown;
            UE_LOG(LogTemp, Warning, TEXT("식물 성장 완료!!"));
        }
    }
    else
    {
        // 물을 안 줬다면 즉시 시듦
        CurrentState = EPlantState::Withered;
        UE_LOG(LogTemp, Error, TEXT("물이 없어서 식물이 시들었습니다..."));
    }

    // 어찌 됐든 상태가 변했으니 메시를 무조건 갱신합니다.
    UpdatePlantMesh();
}

// 상태에 맞춰 메시를 갈아끼우는 함수
void APlantBase::UpdatePlantMesh()
{
    if (!PlantMeshComp) return;

    if (CurrentState == EPlantState::FullyGrown)
    {
        this->Tags.Remove(FName("Plant")); // 물뿌리개 인식 해제
        this->Tags.Add(FName("Grown"));    // 맨손 인식용 태그
        ItemInfo.Category = EItemCategory::GrownPlant;
    }
    else if (CurrentState == EPlantState::Withered)
    {
        this->Tags.Remove(FName("Plant")); // 시든 식물도 물뿌리개 인식 해제
        this->Tags.Add(FName("Withered")); // 맨손 인식용 태그
    }
    else
    {
        this->Tags.Add(FName("Plant")); // 오직 자라는 중일 때만 Plant 태그
        this->Tags.Remove(FName("Grown"));
        this->Tags.Remove(FName("Withered"));
    }


    // 상태에 따른 3D 모델(메시) 강제 적용
    switch (CurrentState)
    {
    case EPlantState::Growing:
        // 자라는 중일 때 -> GrowthMeshes 배열 사용
        if (ItemInfo.PlantData.GrowthMeshes.Num() > 0)
        {
            // GrowthDay를 인덱스로 사용하되, 안전하게 최소/최대값 제한
            int32 MeshIndex = FMath::Clamp(CurrentGrowthDay, 0, ItemInfo.PlantData.GrowthMeshes.Num() - 1);
            if (ItemInfo.PlantData.GrowthMeshes[MeshIndex])
            {
                PlantMeshComp->SetStaticMesh(ItemInfo.PlantData.GrowthMeshes[MeshIndex]);
            }
        }
        break;

    case EPlantState::FullyGrown:
        // 다 자랐을 때 -> VisualData.Mesh 사용
        if (ItemInfo.VisualData.Mesh)
        {
            PlantMeshComp->SetStaticMesh(ItemInfo.VisualData.Mesh);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("에러: %s 씨앗의 VisualData.Mesh가 비어있습니다!"), *ItemInfo.TextData.Name.ToString());
        }
        break;

    case EPlantState::Withered:
        if (WitheredMesh)
        {
            PlantMeshComp->SetStaticMesh(WitheredMesh);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("에러: BP_PlantBase에 WitheredMesh가 할당되지 않았습니다!"));
        }
        break;
    }
}

void APlantBase::Interact_Implementation(AC1Character* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    if (CurrentState == EPlantState::FullyGrown)
    {
        UE_LOG(LogTemp, Warning, TEXT("다 자란 식물 수확 시도!"));

        if (PlayerCharacter->PlayerInventory)
        {
            PlayerCharacter->PlayerInventory->AddStartingItemByID(ItemInfo.HarvestResultID, 1);

            UE_LOG(LogTemp, Warning, TEXT("인벤토리에 수확물이 성공적으로 들어왔습니다!"));
        }

        this->Destroy();
        if (UGardenFarmingComponent* FarmingComp = PlayerCharacter->FindComponentByClass<UGardenFarmingComponent>())
        {
            FarmingComp->SavePlants();
        }
    }

    else if (CurrentState == EPlantState::Withered)
    {
        UE_LOG(LogTemp, Warning, TEXT("시든 식물을 뽑아내어 버렸습니다."));

        this->Destroy();

        if (UGardenFarmingComponent* FarmingComp = PlayerCharacter->FindComponentByClass<UGardenFarmingComponent>())
        {
            FarmingComp->SavePlants();
        }
    }

    else if (CurrentState == EPlantState::Growing)
    {
        UE_LOG(LogTemp, Warning, TEXT("아직 자라는 중입니다... (상호작용 불가)"));
    }
}

// 외곽선 켜기 (이미 InteractionComponent에서 해주지만, 인터페이스 필수 구현)
void APlantBase::BeginFocus_Implementation()
{
}

// 외곽선 끄기
void APlantBase::EndFocus_Implementation()
{
}

void APlantBase::RestorePlantState(int32 InGrowthDay, uint8 InState, bool bInWatered)
{
    CurrentGrowthDay = InGrowthDay;
    CurrentState = static_cast<EPlantState>(InState);
    bIsWateredToday = bInWatered;

    // 다 자란 상태로 로드되었다면 태그와 카테고리도 알맞게 교체
    if (CurrentState == EPlantState::FullyGrown)
    {
        this->Tags.Remove(FName("Plant"));
        this->Tags.Add(FName("Grown"));
        ItemInfo.Category = EItemCategory::GrownPlant;
    }

    // 복구된 상태에 맞춰서 메쉬 갱신
    UpdatePlantMesh();
}

void APlantBase::CheckTimeTravelAndGrowth(int32 SavedDay, int32 CurrentDay)
{
    // 이미 다 자랐거나(FullyGrown) 시든(Withered) 식물은 시간 변화의 영향을 받지 않음
    if (CurrentState == EPlantState::FullyGrown || CurrentState == EPlantState::Withered)
    {
        UE_LOG(LogTemp, Log, TEXT("이미 다 자랐거나 시든 식물이라 시간 여행 패널티를 무시합니다."));
        return;
    }

    // 며칠이 지났는지 단순 빼기 계산
    int32 PassedDays = CurrentDay - SavedDay;

    if (PassedDays == 0)
    {
        // 접속을 여러 번 해도 아무 변화 없음
        UE_LOG(LogTemp, Log, TEXT("같은 날짜 접속: 상태 유지 (시들지 않음)"));
    }
    else if (PassedDays < 0)
    {
        // 물을 줬든 안 줬든 즉시 시듦
        UE_LOG(LogTemp, Error, TEXT("과거로 타임슬립 감지! 식물이 즉시 시듭니다."));
        CurrentState = EPlantState::Withered;
        UpdatePlantMesh();
    }
    else // 다음날 이후
    {
        UE_LOG(LogTemp, Log, TEXT("게임 속 시간으로 %d일이 지났습니다. 성장 계산 시작."), PassedDays);

        // 접속 안 한 날짜 수만큼 하루치 성장 사이클 실행
        for (int32 i = 0; i < PassedDays; ++i)
        {
            OnNewDayStarted();

            if (CurrentState == EPlantState::FullyGrown || CurrentState == EPlantState::Withered)
            {
                break;
            }
        }
    }
}