#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/InteractionInterface.h"
#include "ShopNPC.generated.h"

class AC1Character;

UCLASS()
class COZYWOOD_API AShopNPC : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AShopNPC();

	// IInteractionInterface
	virtual void BeginFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;
	virtual void BeginInteract_Implementation(EItemToolType EquippedTool) override;
	virtual void EndInteract_Implementation() override;
	virtual void Interact_Implementation(AC1Character* PlayerCharacter) override;
	virtual FInteractableData GetInteractableData() const override;

private:
	// 라인 트레이스 감지용 충돌 루트 (StaticMesh 없이도 항상 감지됨)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	// 에디터에서 메시를 교체할 수 있는 시각적 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* NpcMeshComp;

	// 상호작용 UI에 표시될 NPC 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	FText NpcName;
};
