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

	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

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

			if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				// 이 물체가 정원 아이템(가구 or 식물)인지 확인
				bool bIsAlwaysInteract = HitActor->ActorHasTag(FName("AlwaysInteract")); // 일반적인 정원에 심거나 배치하지 않는 아이템
				bool bIsGardenItem = HitActor->ActorHasTag(FName("GardenItem")) || HitActor->IsA(AADecoFurniture::StaticClass()); // 가구, 식물(정원 아이템)
				
				// 정원모드에서 배치되는 물체가 아닌 물체 처리
				if (bIsAlwaysInteract)
				{
					if (HitActor != InteractionData.CurrentInteractable)
					{
						if (InteractionData.CurrentInteractable) { SetOutline(InteractionData.CurrentInteractable, false); }
						FoundInteractable(HitActor); // 외곽선(SetOutline)은 절대 켜지 않음
					}
					return;
				}

				// 정원 아이템 처리
				if (bIsGardenItem)
				{
					// 관람 모드(정원 모드가 아닐 때)면 상호작용이 없음
					if (!OwnerCharacter->bIsGardenMode)
					{
						ClearInteraction();
						return;
					}

					// 정원 모드일 때만 외곽선 켜고 상호작용 준비
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

	// 허공을 보거나 벽을 보면 초기화
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
		UE_LOG(LogTemp, Warning, TEXT("정원 꾸미기 모드: 아이템 즉시 획득!"));
	}
}