#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interfaces/InteractionInterface.h"
#include "../ItemData/ItemDataStructs.h"
#include "InteractionComponent.generated.h"

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData() :
		CurrentInteractable(nullptr),
		LastInteractionCheckTime(0.0f)
	{
	}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COZYWOOD_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PerformInteractionCheck();
	void BeginInteract();
	void EndInteract();
	void Interact();
	void DirectInteract();

	// 관람 모드(ESC)로 전환될 때 테두리를 강제로 꺼줌
	void ClearInteraction();

	// 외곽선 켜고 끄기 
	void SetOutline(AActor* TargetActor, bool bEnable);

	FORCEINLINE bool IsInteracting() const { return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); }

	FORCEINLINE class AActor* GetFocusedActor() const { return InteractionData.CurrentInteractable;}

protected:
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionCheckFrequency = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionCheckDistance = 500.0f;

	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;
};