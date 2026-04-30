#include "C1GameMode.h"
#include "UObject/ConstructorHelpers.h"

AC1GameMode::AC1GameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_C1Character.BP_C1Character_C"));

    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}