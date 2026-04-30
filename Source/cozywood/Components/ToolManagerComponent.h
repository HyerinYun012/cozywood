#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolManagerComponent.generated.h"

class UItemBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COZYWOOD_API UToolManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 생성자
	UToolManagerComponent();

protected:
	// 생성자 이후 초기화 단계에서 호출됨
	virtual void BeginPlay() override;

public:
	// 캐릭터가 호출할 장착 명령 함수
	void EquipTool(UItemBase* ToolItem, class UStaticMeshComponent* TargetMeshComp);

	// 현재 도구를 장착 중인지 확인하는 함수 (나중에 E키 모션용)
	bool IsToolEquipped() const { return bIsToolEquipped; }

private:
	// 도구 장착 상태 (나중에 E키 애니메이션 켤 때 확인용)
	bool bIsToolEquipped;
};