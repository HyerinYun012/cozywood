#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputCoreTypes.h"
#include "NPCCharacter.generated.h"

UCLASS()
class COZYWOOD_API ANPCCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ANPCCharacter();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnNPCClicked(AActor* TouchedActor, FKey ButtonPressed);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FString NPCId = TEXT("pia");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FString NPCDisplayName = TEXT("Pia");
};