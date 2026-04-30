#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EconomyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewMoneyAmount);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COZYWOOD_API UEconomyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEconomyComponent();

protected:
	virtual void BeginPlay() override;

private:
	// 현재 소지금
	UPROPERTY(EditAnywhere, Category = "Finance")
	int32 CurrentMoney;

public:
	UPROPERTY(BlueprintAssignable, Category = "Finance|Events")
	FOnMoneyChanged OnMoneyChanged;

	// 입금
	UFUNCTION(BlueprintCallable, Category = "Finance")
	void AddMoney(int32 Amount);

	// 출금 (돈이 부족하면 false 반환)
	UFUNCTION(BlueprintCallable, Category = "Finance")
	bool SpendMoney(int32 Amount);

	// 현재 잔액 확인
	UFUNCTION(BlueprintPure, Category = "Finance")
	int32 GetCurrentMoney() const { return CurrentMoney; }
};