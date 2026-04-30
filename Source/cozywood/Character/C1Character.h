#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../ItemData/ItemDataStructs.h"
#include "../Interfaces/InteractionInterface.h"
#include "C1Character.generated.h"

class ACozywoodHUD;
class UInventoryComponent;
class UInteractionComponent;
class UGardenHousingComponent;
class UEconomyComponent;

UCLASS()
class COZYWOOD_API AC1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AC1Character();
	
	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool TryOpenShopFromFocusedActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 블루프린트(UI) 띄우라고 지시하는 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Housing|UI")
	void ShowGardenResultUI();

	// 알림 UI 끄기
	UFUNCTION(BlueprintImplementableEvent, Category = "Housing|UI")
	void HideGardenRecordPrompt();

	// 정원 모드 UI 켜기 지시
	UFUNCTION(BlueprintImplementableEvent, Category = "Housing|UI")
	void ShowGardenModeAlertUI();

	// 정원 모드 UI 끄기 지시
	UFUNCTION(BlueprintImplementableEvent, Category = "Housing|UI")
	void HideGardenModeAlertUI();

	// 가구 배치 중이 아닐 때 일반 좌클릭을 처리할 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnNormalLeftClick();

	// ==========================================
	// 컴포넌트
	// ==========================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGardenHousingComponent* GardenHousingComp;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNetworkComponent* NetworkComp;

	// ==========================================
	// 카메라 & 기본 세팅
	// ==========================================
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY()
	ACozywoodHUD* HUD;

	// ==========================================
	// Enhanced Input 액션
	// ==========================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PickupAction; // E키

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LeftClickAction; // 좌클릭

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ToggleMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RotateWheelAction; // 마우스 휠

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RotateHologramAction; // R키

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* EnterGardenAction; // G키

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ExitGardenAction; // ESC키

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RecordEmotionAction; // 감정 기록용 입력 (예: F키)

	// ==========================================
	// 입력 처리용 껍데기 함수들 (구현부에서 컴포넌트 호출)
	// ==========================================
protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 상호작용 관련 입력
	void HandlePickup(const FInputActionValue& Value);

	// 건축 관련 입력
	void ConfirmPlacement();
	void RotateHologram();
	void RotateHologramWheel(const FInputActionValue& Value);

	// 모드 관련
	void EnterGardenMode();
	void ExitGardenMode();
	void ToggleMenu();

	// 정원 창작 관련 입력
	void OnTryRecordEmotion();
	void TryOpenEmotionRecord();

	// 나중에 만들 재배 관련 뼈대
	// void StartPlanting(TSubclassOf<AActor> PlantClass, UStaticMesh* PlantMesh);

	// 1. 실제 도구 모델링 컴포넌트 (카메라에 달려있음)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip")
	class UStaticMeshComponent* EquippedToolMeshComp;

	// 2. 전담 컴포넌트 (파일 분리한 것)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip")
	class UToolManagerComponent* ToolManagerComp;

	// 재무 부서 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UEconomyComponent* EconomyComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UGardenFarmingComponent* GardenFarmingComp;

	// 현재 장착 중인 도구를 기억하는 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool")
	UItemBase* CurrentEquippedItem;

public:
	// ==========================================
	// 캐릭터 상태 및 데이터
	// ==========================================
	// 현재 손에 들고 있는 도구 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	EItemToolType CurrentEquippedToolType = EItemToolType::EITT_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mode")
	bool bIsGardenMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	AActor* FocusedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanOpen;

	// UI에서 현재 이 아이템이 장착 중인지 확인하기 위한 함수
	UFUNCTION(BlueprintPure, Category = "Tool")
	bool IsItemEquipped(UItemBase* ItemToCheck) const;

	// UI 버튼에서 호출할 통합 장착/해제 토글 함수
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void ToggleEquipTool(UItemBase* ToolItem);

	// 장착 해제 전용 함수
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void UnequipTool();

	// 인벤토리 UI 등에서 호출할 통합 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItemFromInventory(class UItemBase* ItemToUse);

	// UI에서 도구 장착 시 호출
	UFUNCTION(BlueprintCallable, Category = "Inventory|Action")
	void EquipTool(class UItemBase* ToolItem);

	// UI에서 씨앗/묘목 심기 시 호출 (매개변수를 UItemBase 통째로 받도록 변경)
	UFUNCTION(BlueprintCallable, Category = "Inventory|Action")
	void StartPlanting(class UItemBase* SeedItem);

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };
	void UpdateInteractionWidget() const;

	FORCEINLINE UEconomyComponent* GetEconomyComp() const { return EconomyComp; }

	// 블루프린트에서 호출할 알림 UI용 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGardenRecordPrompt();

	// 물뿌리개 애니메이션과 파티클을 재생하라고 블루프린트에 지시하는 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Tool|Animation")
	void PlayWateringEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowLocationWarningUI();
};