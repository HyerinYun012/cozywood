#include "EconomyComponent.h"

UEconomyComponent::UEconomyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentMoney = 0;
}

void UEconomyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEconomyComponent::AddMoney(int32 Amount)
{
	if (Amount <= 0) return;

	CurrentMoney += Amount;
	UE_LOG(LogTemp, Warning, TEXT("[%d]원이 입금되었습니다. (현재 잔액: %d원)"), Amount, CurrentMoney);

	// 돈이 바뀌었으니 UI 쪽에 알림 전송!
	if (OnMoneyChanged.IsBound())
	{
		OnMoneyChanged.Broadcast(CurrentMoney);
	}
}

bool UEconomyComponent::SpendMoney(int32 Amount)
{
	if (Amount <= 0) return false;

	if (CurrentMoney >= Amount)
	{
		CurrentMoney -= Amount;
		UE_LOG(LogTemp, Warning, TEXT("[%d]원이 출금되었습니다. (현재 잔액: %d원)"), Amount, CurrentMoney);

		if (OnMoneyChanged.IsBound())
		{
			OnMoneyChanged.Broadcast(CurrentMoney);
		}
		return true; // 결제 성공
	}

	UE_LOG(LogTemp, Warning, TEXT("잔액이 부족합니다! (현재 잔액: %d원)"), CurrentMoney);
	return false; // 결제 실패
}