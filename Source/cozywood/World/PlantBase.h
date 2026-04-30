#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ItemData/ItemDataStructs.h"
#include "PlantBase.generated.h"

UENUM(BlueprintType)
enum class EPlantState : uint8 {
    Growing,    // 자라는 중
    FullyGrown, // 다 자람
    Withered    // 시듦 (물을 안 줌)
};

UCLASS()
class COZYWOOD_API APlantBase : public AActor
{
    GENERATED_BODY()

public:
    APlantBase();

    // 1. 이 식물이 어떤 아이템 정보를 기반으로 하는지 저장
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
    FItemData ItemInfo;

    // 2. 현재 성장 단계 (0: 씨앗, 1: 묘목, 2: 완료 등)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant")
    int32 CurrentGrowthStage = 0;

    // 3. 물 주기 함수 (물뿌리개에서 호출)
    UFUNCTION(BlueprintCallable, Category = "Plant")
    void ReceiveWater();

    UPROPERTY(BlueprintReadWrite)
    bool bIsWateredToday = false; // 오늘 물을 줬는가?

    UPROPERTY(EditAnywhere, Category = "Plant")
    UStaticMesh* WitheredMesh; // 시들었을 때 보여줄 공용 메쉬 (말라비틀어진 모양)

    // 하루가 지날 때 호출될 함수
    void OnNewDayStarted();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant")
    EPlantState CurrentState = EPlantState::Growing;

	void InitializePlant(FItemData InItemData); // 씨앗 심을 때 초기화하는 함수

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* PlantMeshComp;

    void UpdatePlantMesh(); // 단계별로 메쉬 바꾸는 함수

};