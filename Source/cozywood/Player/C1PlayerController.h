#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C1PlayerController.generated.h"

class UNPCChatWidget;
class ALLMManager;
class UInputMappingContext;
class UShopDialogueWidget;
class UShopModeSelectWidget;
class UShopBuyWidget;
class UShopSellWidget;
class UUserWidget;

UCLASS()
class COZYWOOD_API AC1PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void OpenNPCChat(const FString& NpcId);

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void SendCurrentChatMessage();

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void CloseNPCChat();

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    bool IsChatOpen() const { return NPCChatWidgetInstance != nullptr; }

    UFUNCTION(BlueprintCallable, Category = "Shop") //추가
	void OpenShopModeSelect();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void OpenShopBuy();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void OpenShopSell();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void CloseShopUI();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool IsShopOpen() const; //추가

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Chat")
    TSubclassOf<UNPCChatWidget> NPCChatWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Chat")
    TSubclassOf<ALLMManager> LLMManagerClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    TSubclassOf<UShopDialogueWidget> ShopDialogueWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    TSubclassOf<UShopBuyWidget> ShopBuyWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    TSubclassOf<UShopSellWidget> ShopSellWidgetClass; //추가

private:
    UPROPERTY()
    TObjectPtr<UNPCChatWidget> NPCChatWidgetInstance;

    UPROPERTY()
    TObjectPtr<ALLMManager> LLMManagerInstance;

    UPROPERTY()
    TObjectPtr<UShopDialogueWidget> ShopDialogueWidgetInstance; //추가

    UPROPERTY()
    TObjectPtr<UShopBuyWidget> ShopBuyWidgetInstance;

    UPROPERTY()
    TObjectPtr<UShopSellWidget> ShopSellWidgetInstance; //추가

    void ApplyShopInputMode(UUserWidget* FocusWidget);
    void RestoreGameInputMode();

    // 델리게이트와 형식을 맞춘 핸들러 함수들
    UFUNCTION()
    void HandleChatResponse(const FString& InReply, float ResponseTimeSeconds);

    UFUNCTION()
    void HandleGreetingResponse(const FString& InGreeting, float ResponseTimeSeconds);

    UFUNCTION()
    void HandleServerHealthChecked(bool bIsConnected);

    void RequestServerHealthCheck();
    void UpdateClockText();

    FTimerHandle HealthCheckTimerHandle;
    FTimerHandle TimeUpdateTimerHandle;
};