#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C1PlayerController.generated.h"

class UInputMappingContext;
class UShopDialogueWidget;
class UShopModeSelectWidget;
class UShopBuyWidget;
class UShopSellWidget;
class UShopWidget;
class UUserWidget;
class UItemBase;

UCLASS()
class COZYWOOD_API AC1PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable, Category = "Shop")
	void OpenShopModeSelect();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void OpenShopBuy();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void OpenShopSell();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void CloseShopUI();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool IsShopOpen() const;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool BuyItem(FName ItemID, int32 Price, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool SellItem(UItemBase* ItemToSell, int32 SellPrice, int32 Quantity = 1);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    TSubclassOf<UShopModeSelectWidget> ShopModeSelectWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    TSubclassOf<UShopBuyWidget> ShopBuyWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    TSubclassOf<UShopSellWidget> ShopSellWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    TSubclassOf<UShopWidget> ShopWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<UShopModeSelectWidget> ShopModeSelectWidgetInstance;

    UPROPERTY()
    TObjectPtr<UShopBuyWidget> ShopBuyWidgetInstance;

    UPROPERTY()
    TObjectPtr<UShopSellWidget> ShopSellWidgetInstance;

    void ApplyShopInputMode(UUserWidget* FocusWidget);
    void RestoreGameInputMode();
    void HandleEscapeKey();

    FTimerHandle HealthCheckTimerHandle;
    FTimerHandle TimeUpdateTimerHandle;

    // CloseShopUI 직후 E키로 즉시 재오픈되는 것을 막기 위한 타임스탬프
    float ShopLastCloseTime = -1.0f;
};

