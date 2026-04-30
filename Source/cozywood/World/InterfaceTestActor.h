// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class COZYWOOD_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInterfaceTestActor();

protected:
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;
	virtual void BeginInteract_Implementation(EItemToolType EquippedTool) override;
	virtual void EndInteract_Implementation() override;
	virtual void Interact_Implementation(AC1Character* PlayerCharacter) override;

	virtual FInteractableData GetInteractableData() const override;
};
