#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenHousingComponent.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class UStaticMesh;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COZYWOOD_API UGardenHousingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGardenHousingComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 정원모드에서 어떤 가구를 배치할지, 메쉬, 아이템 정보들을 가져옴
	void StartPlacement(TSubclassOf<AActor> InFurnitureClass, UStaticMesh* FurnitureMesh, class UItemBase* ItemToUse);
	// 매 배치마다 홀로그램 띄우기
	void UpdateHologram();
	// 아이템 배치 확정
	void ConfirmPlacement();

	// 마우스 휠 회전 관련
	void RotateHologramToggle();
	void RotateHologramWheel(float WheelValue);

	// 배치 취소 (ESC 눌렀을 때 끄기 용도)
	void CancelPlacement();

	FORCEINLINE bool IsRewardReceived() const { return bHasReceivedDecoReward; }

	// 현재 배치 모드인지(홀로그램이 떠 있는지) 확인하는 헬퍼 함수
	FORCEINLINE bool IsPlacing() const { return HologramActor != nullptr; }

	// 관람 모드(이름 짓기)에 진입할 수 있는 상태인지 확인하는 헬퍼 함수
	FORCEINLINE bool IsReadyForNaming() const
	{
		// 가구 10개 이상 깔았고 + 아직 이름 짓기(보상)를 안 했다면 true 반환
		return (PlacedFurnitureCount >= TargetDecoCount) && !bHasReceivedDecoReward;
	}

	// UI에서 이름 짓기가 끝났을 때 호출해 줄 함수
	UFUNCTION(BlueprintCallable, Category = "Housing|Event")
	void FinishNamingAndGiveReward();

	void ResetPlacementCount();

	// 장부 쓰고 읽기 함수
	UFUNCTION(BlueprintCallable, Category = "Housing|SaveLoad")
	void SaveGarden();

	UFUNCTION(BlueprintCallable, Category = "Housing|SaveLoad")
	void LoadGarden();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Housing|SaveLoad")
	FString SaveSlotName = TEXT("GardenSaveSlot");

	// 불러올 때 가구 메쉬를 다시 찾아오기 위해 필요한 데이터 테이블
	UPROPERTY(EditDefaultsOnly, Category = "Housing|Data")
	class UDataTable* ItemDB;

	// 건축 부서 전용 비밀(Protected) 변수들
	UPROPERTY()
	class AActor* HologramActor = nullptr;

	UPROPERTY()
	class UStaticMeshComponent* HologramMeshComp = nullptr;

	UPROPERTY()
	class UMaterialInstanceDynamic* HologramMID = nullptr;

	// 에디터에서 설정할 홀로그램 기본 머티리얼
	UPROPERTY(EditDefaultsOnly, Category = "Housing")
	UMaterialInterface* HologramBaseMaterial = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Housing")
	bool bCanPlace = false;

	bool bIsValidPlacement = false;
	bool bIsWheelRotationEnabled = false;

	UPROPERTY()
	TSubclassOf<AActor> FurnitureClassToSpawn;

	UPROPERTY()
	UStaticMesh* FurnitureMeshToSpawn = nullptr;

	UPROPERTY(EditAnywhere, Category = "Housing|Event")
	int32 PlacedFurnitureCount = 0; // 지금까지 배치한 가구 수

	UPROPERTY(EditAnywhere, Category = "Housing|Event")
	int32 TargetDecoCount = 10; // 보상을 주기 위한 목표 개수

	UPROPERTY(BlueprintReadOnly, Category = "Housing|Event")
	bool bHasReceivedDecoReward = false; // 보상 중복 지급 방지용

private:
	UPROPERTY()
	class UItemBase* ItemBeingPlaced;
};