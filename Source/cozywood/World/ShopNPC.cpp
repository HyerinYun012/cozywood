#include "ShopNPC.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Character/C1Character.h"
#include "../Player/C1PlayerController.h"

AShopNPC::AShopNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	// 캡슐을 루트로: StaticMesh가 없어도 ECC_Visibility 라인 트레이스에 항상 감지됨
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->InitCapsuleSize(40.f, 88.f);
	CapsuleComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CapsuleComp;

	// 메시는 시각적 용도, 충돌은 캡슐이 담당
	NpcMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NpcMeshComp"));
	NpcMeshComp->SetupAttachment(CapsuleComp);
	NpcMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BabyRobotMesh(
		TEXT("/Game/Fab/Baby_Robot/baby_robot/StaticMeshes/baby_robot"));
	if (BabyRobotMesh.Succeeded())
	{
		NpcMeshComp->SetStaticMesh(BabyRobotMesh.Object);
	}

	// "AlwaysInteract": InteractionComponent가 정원 모드 관계없이 항상 감지
	// "ShopNPC":        TryOpenShopFromFocusedActor (좌클릭)에서도 상점을 열 수 있게 식별
	Tags.Add(TEXT("AlwaysInteract"));
	Tags.Add(TEXT("ShopNPC"));

	NpcName = FText::FromString(TEXT("상점 로봇"));
}

void AShopNPC::BeginFocus_Implementation()
{
	// AlwaysInteract 경로에서는 InteractionComponent가 외곽선을 켜지 않으므로 별도 처리 없음
}

void AShopNPC::EndFocus_Implementation()
{
}

void AShopNPC::BeginInteract_Implementation(EItemToolType EquippedTool)
{
}

void AShopNPC::EndInteract_Implementation()
{
}

void AShopNPC::Interact_Implementation(AC1Character* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	AC1PlayerController* PC = Cast<AC1PlayerController>(PlayerCharacter->GetController());
	if (PC)
	{
		PC->OpenShopModeSelect();
	}
}

FInteractableData AShopNPC::GetInteractableData() const
{
	FInteractableData Data;
	Data.InteractableType = EInteractableType::EIT_Dialogue;
	Data.Name = NpcName;
	Data.Action = FText::FromString(TEXT("대화하기"));
	Data.InteractionDuration = 0.0f;
	return Data;
}
