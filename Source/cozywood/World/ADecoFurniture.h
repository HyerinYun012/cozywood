// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/InteractionInterface.h"
#include "ADecoFurniture.generated.h"

class AC1Character;

UCLASS()
class COZYWOOD_API AADecoFurniture : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AADecoFurniture();

	virtual void Interact_Implementation(AC1Character* PlayerCharacter) override;
	virtual FInteractableData GetInteractableData() const override;

	// 외형을 담당할 스태틱 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Furniture")
	class UStaticMeshComponent* FurnitureMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 이 가구가 데이터 테이블에서 어떤 ID(행 이름)를 가졌는지 기억할 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Furniture")
	FName FurnitureItemID;

	UFUNCTION(BlueprintCallable, Category = "Furniture")
	void SetFurnitureData(class UStaticMesh* NewMesh, FName ItemID);

};
