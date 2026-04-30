// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceTestActor.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus_Implementation()
{
	if (Mesh) {
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus_Implementation()
{
	if (Mesh) {
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract_Implementation(EItemToolType EquippedTool)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));
}

void AInterfaceTestActor::EndInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));
}

void AInterfaceTestActor::Interact_Implementation(AC1Character* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor."));
}

FInteractableData AInterfaceTestActor::GetInteractableData() const
{
	return FInteractableData();
}