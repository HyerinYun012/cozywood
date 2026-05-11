#include "C1Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"
#include "../UserInterface/CozywoodHUD.h"
#include "../Components/InventoryComponent.h"
#include "../Components/InteractionComponent.h" 
#include "../Components/GardenHousingComponent.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "../Components/EconomyComponent.h"
#include "../Items/ItemBase.h"
#include "../Player/C1PlayerController.h"
#include "../Components/ToolManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../World/PlantBase.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/GardenFarmingComponent.h"

AC1Character::AC1Character()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp"));
	GardenHousingComp = CreateDefaultSubobject<UGardenHousingComponent>(TEXT("GardenHousingComp"));
	GardenFarmingComp = CreateDefaultSubobject<UGardenFarmingComponent>(TEXT("GardenFarmingComp"));

	if (CameraComp)
	{
		CameraComp->SetupAttachment(GetCapsuleComponent());

		CameraComp->bUsePawnControlRotation = true;
		bUseControllerRotationYaw = true;

		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 3000.0f;
		GetCharacterMovement()->GroundFriction = 10.0f;
		GetCharacterMovement()->SetWalkableFloorAngle(50.0f);
		GetCharacterMovement()->MaxStepHeight = 45.f;
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 0.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bDoCollisionTest = false;

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(50);

	CameraComp->SetupAttachment(SpringArmComp);

	EconomyComp = CreateDefaultSubobject<UEconomyComponent>(TEXT("EconomyComp"));

	EquippedToolMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquippedToolMeshComp"));
	EquippedToolMeshComp->SetupAttachment(CameraComp);
	EquippedToolMeshComp->SetVisibility(false);
	EquippedToolMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ToolManagerComp = CreateDefaultSubobject<UToolManagerComponent>(TEXT("ToolManagerComp"));
}

void AC1Character::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		HUD = Cast<ACozywoodHUD>(PC->GetHUD());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay: 플레이어 컨트롤러를 아직 찾지 못했습니다."));
	}

	CameraComp->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 20.f));

	if (GardenHousingComp)
	{
		GardenHousingComp->LoadGarden();
	}
}

void AC1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GardenHousingComp && GardenHousingComp->IsPlacing())
	{
		GardenHousingComp->UpdateHologram();
	}
	else if (InteractionComp)
	{
		InteractionComp->PerformInteractionCheck();
	}
}

// ==========================================
// 모드 및 입력 관련 지시 함수들
// ==========================================

// 정원 모드 키는 조건(main 레벨에서만 킬 수 있도록 설정, 각 조건에 따른 다른 UI창 띄우기)
void AC1Character::EnterGardenMode()
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	if (CurrentLevelName.ToLower() != TEXT("main"))
	{
		UE_LOG(LogTemp, Warning, TEXT("차단: 정원 모드는 main 레벨에서만 켤 수 있습니다"));

		ShowLocationWarningUI();

		return;
	}

	bIsGardenMode = true;
	ShowGardenModeAlertUI();
}

void AC1Character::OnTryRecordEmotion()
{
	// 조건: 가구가 10개 이상이고, 아직 기록하지 않았을 경우만
	if (GardenHousingComp && GardenHousingComp->IsReadyForNaming() && !GardenHousingComp->IsRewardReceived())
	{
		// 이때 위젯을 생성하고 화면에 띄우기
		ShowGardenResultUI();

		// 입력 모드를 UI도 가능하게 변경 (마우스 커서 필요 시)
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void AC1Character::ExitGardenMode()
{
	// =================================================================
	//  2. 일반 케이스: 평범하게 정원 모드를 취소하고 나갈 때 (기존 로직)
	// =================================================================
	bIsGardenMode = false;

	// 일반 종료 시에도 글자 제거
	HideGardenModeAlertUI();

	HideGardenRecordPrompt();

	UnequipTool();

	// 모드 종료 시 각 컴포넌트 정리
	if (InteractionComp) { InteractionComp->ClearInteraction(); }
	if (GardenHousingComp) { 
		GardenHousingComp->CancelPlacement();
		GardenHousingComp->ResetPlacementCount();

		GardenHousingComp->SaveGarden();
	}
}

void AC1Character::HandlePickup(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("[HandlePickup] CALLED"));

	// 1. 상점/채팅 UI가 열려 있으면 즉시 차단
	AC1PlayerController* PC = Cast<AC1PlayerController>(GetController());
	// if (PC && (PC->IsChatOpen() || PC->IsShopOpen())) return; 블루프린트로 대화시스템 구현

	// ==========================================
	// 2. 정원 모드 + 물뿌리개 사용 로직 수정
	// ==========================================
	if (bIsGardenMode && CurrentEquippedToolType == EItemToolType::EITT_WateringCan)
	{
		// 1) 대상 유무와 상관없이 물뿌리는 파티클+애니메이션 무조건 실행
		PlayWateringEffect();

		// 2) 바라보고 있는 대상이 자라는 중인 식물이라면 물을 줌
		AActor* Focused = InteractionComp ? InteractionComp->GetFocusedActor() : nullptr;
		if (Focused && Focused->ActorHasTag(FName("Plant")))
		{
			APlantBase* Plant = Cast<APlantBase>(Focused);
			if (Plant)
			{
				Plant->ReceiveWater();
			}
		}
		// 물뿌리개를 사용했으므로 다른 상호작용은 무시하고 종료
		return;
	}

	if (!InteractionComp) return;

	// 3. ShopNPC 태그를 가진 액터 → 상점 즉시 열기
	AActor* Focused = InteractionComp->GetFocusedActor();
	if (IsValid(Focused) && Focused->ActorHasTag(FName("ShopNPC")))
	{
		if (PC) PC->OpenShopModeSelect();
		return;
	}

	// 4. 일반 상호작용
	InteractionComp->BeginInteract();
}

void AC1Character::ConfirmPlacement()
{
	// 채팅창이 떠 있다면 캐릭터의 좌클릭 로직은 무시
	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetController()))
	{
		// 블루프린트로 대화시스템 구현 예정
	}

	if (GardenHousingComp && GardenHousingComp->IsPlacing())
	{
		GardenHousingComp->ConfirmPlacement();
	}
	// 2. 가구 배치 중이 아니라면? (평상시 상태)
	else 
	{
		if (TryOpenShopFromFocusedActor())
		{
			return;
		}

		OnNormalLeftClick();
	}
}

void AC1Character::RotateHologram()
{
	if (GardenHousingComp)
	{
		GardenHousingComp->RotateHologramToggle();
	}
}

void AC1Character::RotateHologramWheel(const FInputActionValue& Value)
{
	if (GardenHousingComp)
	{
		float WheelValue = Value.Get<float>();
		GardenHousingComp->RotateHologramWheel(WheelValue);
	}
}

// ==========================================
// 인벤토리 연동
// ==========================================

void AC1Character::UseItemFromInventory(UItemBase* ItemToUse)
{
	// 1. 방어 코드: 아이템이 없거나, 정원 모드가 아니면 취소
	if (!ItemToUse || !bIsGardenMode) return;

	switch (ItemToUse->Category)
	{
	case EItemCategory::Furniture:
	case EItemCategory::GrownPlant:
		UE_LOG(LogTemp, Log, TEXT("배치 모드 시작"));

		if (GardenHousingComp)
		{
			//GardenHousingComp 에게 메쉬, 액터 클래스, ItemToUse를 넘겨줌
			GardenHousingComp->StartPlacement(ItemToUse->VisualData.ActorClass, ItemToUse->VisualData.Mesh, ItemToUse);
		}
		break;

	case EItemCategory::Seed:
		UE_LOG(LogTemp, Log, TEXT("심기 모드 시작"));
		StartPlanting(ItemToUse);
		break;

	case EItemCategory::Tool:
		UE_LOG(LogTemp, Log, TEXT("도구 장착 시작"));
		ToggleEquipTool(ItemToUse);
		break;

	default:
		break;
	}
}

// 1. 상태 확인용 함수 (이름 바뀐 변수 사용)
bool AC1Character::IsItemEquipped(UItemBase* ItemToCheck) const
{
	// 1. 방어 코드: 검사할 아이템이 없거나, 내가 지금 아무것도 안 들고 있다면 False
	if (!ItemToCheck || !CurrentEquippedItem)
	{
		return false;
	}

	// 2. 메모리 주소가 달라도 명찰(ItemID)이 같으면 같은 아이템으로 취급
	return CurrentEquippedItem->ItemID == ItemToCheck->ItemID;
}

// 2. 장착/해제 토글 로직
void AC1Character::ToggleEquipTool(UItemBase* ToolItem)
{
	if (!ToolItem || !bIsGardenMode) return;

	// 이미 장착된 도구를 다시 눌렀다면 -> 해제
	if (IsItemEquipped(ToolItem))
	{
		UnequipTool();
	}
	// 다른 도구거나 빈 손이라면 -> 장착
	else
	{
		EquipTool(ToolItem);
	}
}

// 3. 장착 함수 (두 변수 동시 업데이트)
void AC1Character::EquipTool(UItemBase* ToolItem)
{
	if (!ToolItem) return;

	// 아이템 포인터와 도구 종류(Enum)를 동시에 세팅
	CurrentEquippedItem = ToolItem;
	CurrentEquippedToolType = ToolItem->ToolType;

	if (ToolManagerComp)
	{
		ToolManagerComp->EquipTool(ToolItem, EquippedToolMeshComp);
		EquippedToolMeshComp->SetVisibility(true); // 도구 보이게 하기
	}

	UE_LOG(LogTemp, Warning, TEXT("도구 장착 완료: %s, 도구 타입: %d"), *ToolItem->GetName(), (int32)CurrentEquippedToolType);
}

// 4. 해제 함수 (두 변수 모두 초기화)
void AC1Character::UnequipTool()
{
	// 빈 손 상태로 만들 때 두 변수 모두 초기화
	CurrentEquippedItem = nullptr;
	CurrentEquippedToolType = EItemToolType::EITT_None;

	if (EquippedToolMeshComp)
	{
		EquippedToolMeshComp->SetVisibility(false); // 도구 숨기기
		EquippedToolMeshComp->SetStaticMesh(nullptr); // 메쉬 비우기
	}

	UE_LOG(LogTemp, Warning, TEXT("도구 장착 해제 완료"));
}

void AC1Character::StartPlanting(UItemBase* SeedItem)
{
	if (GardenFarmingComp)
	{
		GardenFarmingComp->StartPlanting(SeedItem);
	}
}


// ==========================================
// 기본 이동 및 UI 세팅 
// ==========================================

void AC1Character::ToggleMenu()
{
	if (HUD == nullptr)
	{
		HUD = Cast<ACozywoodHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	}
	if (HUD)
	{
		HUD->ToggleMenu();
	}
}

void AC1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (EnterGardenAction) {
			EnhancedInputComponent->BindAction(EnterGardenAction, ETriggerEvent::Started, this, &AC1Character::ToggleGardenMode);
		}
		if (RotateHologramAction) {
			EnhancedInputComponent->BindAction(RotateHologramAction, ETriggerEvent::Started, this, &AC1Character::RotateHologram);
		}
		if (RotateWheelAction) {
			EnhancedInputComponent->BindAction(RotateWheelAction, ETriggerEvent::Triggered, this, &AC1Character::RotateHologramWheel);
		}
		if (LeftClickAction) {
			EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AC1Character::ConfirmPlacement);
		}
		if (MoveAction) {
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AC1Character::Move);
		}
		if (LookAction) {
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AC1Character::Look);
		}
		if (PickupAction) {
			EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &AC1Character::HandlePickup);
		}
		if (ToggleMenuAction) {
			EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &AC1Character::ToggleMenu);
		}
		if (RecordEmotionAction) {
			EnhancedInputComponent->BindAction(RecordEmotionAction, ETriggerEvent::Started, this, &AC1Character::TryOpenEmotionRecord);
		}
		if (OpenMenuAction) {
			EnhancedInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &AC1Character::TriggerSystemMenu);
		}
		// InteractAction이 에디터에서 잘 할당되어 있다면 바인딩
		
	}
}

void AC1Character::Move(const FInputActionValue& Value)
{
	// if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetController()))
		// if (PC->IsShopOpen() || PC->IsChatOpen()) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AC1Character::Look(const FInputActionValue& Value)
{
	// if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetController()))
		// if (PC->IsShopOpen() || PC->IsChatOpen()) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

bool AC1Character::TryOpenShopFromFocusedActor()
{
	if (!InteractionComp) return false;

	AActor* Focused = InteractionComp->GetFocusedActor();
	if (!IsValid(Focused)) return false;

	if (!Focused->ActorHasTag(FName("ShopNPC"))) return false;

	if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetController()))
	{
		PC->OpenShopModeSelect();
		return true;
	}
	return false;
}

void AC1Character::TryOpenEmotionRecord()
{
	if (GardenHousingComp && GardenHousingComp->IsReadyForNaming())
	{
		ShowGardenResultUI();
		HideGardenRecordPrompt();
	}
}

void AC1Character::ShowPlantOverlapErrorUI()
{
	// 1. 에러 팝업 위젯 클래스가 블루프린트에서 잘 지정되어 있는지 확인
	if (PlantOverlapErrorWidgetClass)
	{
		// 2. 위젯 생성
		UUserWidget* ErrorWidget = CreateWidget<UUserWidget>(GetWorld(), PlantOverlapErrorWidgetClass);

		if (ErrorWidget)
		{
			// 3. 화면에 띄우기
			ErrorWidget->AddToViewport(100);
		}
	}
	else
	{
		// 클래스가 지정 안 되어 있으면 로그 띄우기
		UE_LOG(LogTemp, Warning, TEXT("식물 겹침 에러 위젯 클래스가 세팅되지 않았음 캐릭터 블루프린트를 확인"));
	}
}

void AC1Character::ToggleGardenMode()
{
	if (bIsGardenMode)
	{
		// 이미 정원 모드라면? -> 끄기
		ExitGardenMode();
	}
	else
	{
		// 정원 모드가 아니라면? -> 켜기
		EnterGardenMode();
	}
}

void AC1Character::TriggerSystemMenu()
{
	UE_LOG(LogTemp, Log, TEXT("ESC 키 눌림 시스템 메뉴를 엽니다."));

	// 블루프린트에 만들어둘 OnOpenSystemMenu 이벤트를 호출 (실행)
	OnOpenSystemMenu();
}