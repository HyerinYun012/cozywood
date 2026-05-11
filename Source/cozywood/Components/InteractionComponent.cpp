#include "InteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "../Interfaces/InteractionInterface.h"
#include "../World/ADecoFurniture.h"
#include "../Character/C1Character.h" 

UInteractionComponent::UInteractionComponent()
{
	// 레이저 검사는 필요할 때만 호출할 것이므로 자체 Tick은 꺼두기 (최적화)
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::PerformInteractionCheck()
{
	AC1Character* OwnerCharacter = Cast<AC1Character>(GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->CameraComp) return;

	// C4244 fix: GetTimeSeconds()는 double 반환 → float으로 명시적 캐스트
	InteractionData.LastInteractionCheckTime = static_cast<float>(GetWorld()->GetTimeSeconds());

	FVector TraceStart = OwnerCharacter->CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + (OwnerCharacter->CameraComp->GetForwardVector() * InteractionCheckDistance);
	float LookDirection = FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), OwnerCharacter->GetViewRotation().Vector());

	if (LookDirection > 0)
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = TraceHit.GetActor();

			// 인터페이스를 구현했거나, 특정 태그(ShopNPC, AlwaysInteract)를 가졌다면 검출 통과
			if (HitActor && (HitActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass())
				|| HitActor->ActorHasTag(FName("ShopNPC"))
				|| HitActor->ActorHasTag(FName("AlwaysInteract"))))
			{
				bool bIsAlwaysInteract = HitActor->ActorHasTag(FName("AlwaysInteract")) || HitActor->ActorHasTag(FName("ShopNPC"));
				bool bIsGardenItem = HitActor->ActorHasTag(FName("GardenItem")) || HitActor->IsA(AADecoFurniture::StaticClass());

				if (bIsAlwaysInteract)
				{
					if (HitActor != InteractionData.CurrentInteractable)
					{
						if (InteractionData.CurrentInteractable) { SetOutline(InteractionData.CurrentInteractable, false); }
						FoundInteractable(HitActor);
					}
					return;
				}

				if (bIsGardenItem)
				{
					if (!OwnerCharacter->bIsGardenMode)
					{
						ClearInteraction();
						return;
					}

					bool bIsHoldingWateringCan = (OwnerCharacter->CurrentEquippedToolType == EItemToolType::EITT_WateringCan);
					bool bIsGrowingPlant = HitActor->ActorHasTag(FName("Plant")); // 자라는 중인 식물인지 확인

					// 1. 자라는 중인 식물은 '무조건' 물뿌리개가 있어야만 외곽선 표시 (맨손이면 무시)
					if (bIsGrowingPlant && !bIsHoldingWateringCan)
					{
						ClearInteraction();
						return;
					}

					// 2. 가구나 다 자란 식물은 물뿌리개를 들고 있으면 외곽선 표시 금지 (맨손일 때만 표시)
					if (!bIsGrowingPlant && bIsHoldingWateringCan)
					{
						ClearInteraction();
						return;
					}

					// 위 조건들을 모두 통과했다면 정상적으로 외곽선 켜기
					if (HitActor != InteractionData.CurrentInteractable)
					{
						if (InteractionData.CurrentInteractable) { SetOutline(InteractionData.CurrentInteractable, false); }
						SetOutline(HitActor, true);
						FoundInteractable(HitActor);
					}
					return;
				}
			}
		}
	}

	ClearInteraction();
}

void UInteractionComponent::ClearInteraction()
{
	if (InteractionData.CurrentInteractable)
	{
		SetOutline(InteractionData.CurrentInteractable, false);
		NoInteractableFound();
	}
}

void UInteractionComponent::SetOutline(AActor* TargetActor, bool bEnable)
{
	if (!TargetActor) return;
	TArray<UMeshComponent*> Meshes;
	TargetActor->GetComponents<UMeshComponent>(Meshes);
	for (UMeshComponent* MeshComp : Meshes)
	{
		MeshComp->SetRenderCustomDepth(bEnable);
	}
}

void UInteractionComponent::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) { EndInteract(); }

	if (InteractionData.CurrentInteractable) {
		TargetInteractable = InteractionData.CurrentInteractable;
		if (IsValid(TargetInteractable.GetObject()))
		{
			IInteractionInterface::Execute_EndFocus(TargetInteractable.GetObject());
		}
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	if (IsValid(TargetInteractable.GetObject()))
	{
		IInteractionInterface::Execute_BeginFocus(TargetInteractable.GetObject());
	}
}

void UInteractionComponent::NoInteractableFound()
{
	if (IsInteracting()) { GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); }

	if (InteractionData.CurrentInteractable) {
		if (IsValid(TargetInteractable.GetObject()))
		{
			IInteractionInterface::Execute_EndFocus(TargetInteractable.GetObject());
		}
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void UInteractionComponent::BeginInteract()
{
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable && IsValid(TargetInteractable.GetObject()))
	{
		AC1Character* OwnerCharacter = Cast<AC1Character>(GetOwner());

		IInteractionInterface::Execute_BeginInteract(TargetInteractable.GetObject(), OwnerCharacter->CurrentEquippedToolType);

		float InteractionDuration = 0.0f;

		if (TargetInteractable.GetInterface() != nullptr)
		{
			InteractionDuration = TargetInteractable->GetInteractableData().InteractionDuration;
		}

		if (FMath::IsNearlyZero(InteractionDuration, 0.1f)) {
			Interact();
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Interaction, this, &UInteractionComponent::Interact, InteractionDuration, false);
		}
	}
}

void UInteractionComponent::EndInteract()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		IInteractionInterface::Execute_EndInteract(TargetInteractable.GetObject());
	}
}

void UInteractionComponent::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		AC1Character* OwnerCharacter = Cast<AC1Character>(GetOwner());

		IInteractionInterface::Execute_Interact(TargetInteractable.GetObject(), OwnerCharacter);
	}
}

void UInteractionComponent::DirectInteract()
{
	AC1Character* OwnerCharacter = Cast<AC1Character>(GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->bIsGardenMode) return;

	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable && IsValid(TargetInteractable.GetObject()))
	{
		IInteractionInterface::Execute_Interact(TargetInteractable.GetObject(), OwnerCharacter);
		UE_LOG(LogTemp, Warning, TEXT("정원 꾸미기 모드: 아이템 즉시 획득"));
	}
}