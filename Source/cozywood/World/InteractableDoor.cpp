#include "InteractableDoor.h"
#include "Components/StaticMeshComponent.h"
#include "../Character/C1Character.h" // 캐릭터 헤더 경로 맞게 수정

AInteractableDoor::AInteractableDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	// InteractionComponent가 이 문을 "항상 상호작용 가능"으로 인식하도록 태그 추가
	Tags.Add(FName("AlwaysInteract"));

	// 카메라 레이저(LineTrace)가 문에 맞아야 하므로 Visibility 채널을 막음(Block)
	DoorMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AInteractableDoor::Interact_Implementation(AC1Character* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("컴포넌트를 통한 문 상호작용 성공!"));
	// 블루프린트에서 레벨 이동 노드 연결
}

FInteractableData AInteractableDoor::GetInteractableData() const
{
	FInteractableData DoorData;
	DoorData.InteractableType = EInteractableType::EIT_Usable;
	DoorData.Name = FText::FromString(TEXT("문"));
	DoorData.Action = FText::FromString(TEXT("이동하기"));
	return DoorData;
}