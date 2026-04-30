#include "NPCCharacter.h"
#include "../Player/C1PlayerController.h"
#include "Kismet/GameplayStatics.h"

ANPCCharacter::ANPCCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    OnClicked.AddDynamic(this, &ANPCCharacter::OnNPCClicked);
}

void ANPCCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ANPCCharacter::OnNPCClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    UE_LOG(LogTemp, Warning, TEXT("NPC Clicked: %s"), *NPCId);

    if (AC1PlayerController* PC = Cast<AC1PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
    {
        PC->OpenNPCChat(NPCId);
    }
}