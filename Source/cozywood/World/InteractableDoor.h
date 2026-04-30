#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/InteractionInterface.h"
#include "InteractableDoor.generated.h"

// 전방 선언 (컴파일 속도 향상을 위해 헤더에서는 포인터만 알림)
class UBoxComponent;
class UStaticMeshComponent;
class AC1Character;

UCLASS()
class COZYWOOD_API AInteractableDoor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// 기본 생성자
	AInteractableDoor();

	// 에디터에서 문마다 개별적으로 이동할 레벨 이름을 적을 수 있게 열어줍니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	FName TargetLevelName;

	virtual void Interact_Implementation(AC1Character* PlayerCharacter) override;
	virtual FInteractableData GetInteractableData() const override;
protected:
	// 문의 외형을 담당할 스태틱 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
	UStaticMeshComponent* DoorMesh;
};