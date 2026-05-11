#include "EconomyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Save/GardenSave.h"

UEconomyComponent::UEconomyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentMoney = 1000;
}

void UEconomyComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadMoney();
}

void UEconomyComponent::AddMoney(int32 Amount)
{
	if (Amount <= 0) return;
	CurrentMoney += Amount;

	if (OnMoneyChanged.IsBound())
		OnMoneyChanged.Broadcast(CurrentMoney);

	SaveMoney();
}

bool UEconomyComponent::SpendMoney(int32 Amount)
{
	if (Amount <= 0 || CurrentMoney < Amount) return false;

	CurrentMoney -= Amount;
	if (OnMoneyChanged.IsBound())
		OnMoneyChanged.Broadcast(CurrentMoney);

	SaveMoney();

	return true;
}

void UEconomyComponent::SaveMoney()
{
	FString SaveSlotName = TEXT("GardenSaveSlot");
	UGardenSave* SaveInstance = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveInstance = Cast<UGardenSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	if (!SaveInstance)
	{
		SaveInstance = Cast<UGardenSave>(UGameplayStatics::CreateSaveGameObject(UGardenSave::StaticClass()));
	}

	if (SaveInstance)
	{
		SaveInstance->SavedMoney = CurrentMoney;
		UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, 0); 
	}
}

void UEconomyComponent::LoadMoney()
{
	FString SaveSlotName = TEXT("GardenSaveSlot");

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		if (UGardenSave* SaveInstance = Cast<UGardenSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
		{
			CurrentMoney = SaveInstance->SavedMoney;

			if (OnMoneyChanged.IsBound())
				OnMoneyChanged.Broadcast(CurrentMoney);

			return;
		}
	}

	SaveMoney();

	if (OnMoneyChanged.IsBound())
		OnMoneyChanged.Broadcast(CurrentMoney);
}