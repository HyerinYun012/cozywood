#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCChatWidget.generated.h"

class UTextBlock;
class UEditableTextBox;
class UButton;
class UVerticalBox;

UCLASS()
class COZYWOOD_API UNPCChatWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 1. PlayerController에서 직접 호출할 수 있도록 public으로 이동
    void AddMessageToChat(const FString& Message);

    // 2. 델리게이트와 형식을 맞춘 단 하나의 AddChatMessage (중복 제거됨)
    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void AddChatMessage(const FString& NewText, float ResponseTimeSeconds);

    // --- 기존 UI 제어 함수들 ---
    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void SetNpcId(const FString& InNpcId);

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    FString GetNpcId() const;

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    FString GetInputMessage() const;

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void ClearInputMessage();

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void SetServerStatusText(const FString& NewText);

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void SetResponseTimeText(const FString& NewText);

    UFUNCTION(BlueprintCallable, Category = "NPC Chat")
    void SetTimeText(const FString& NewText);

    UPROPERTY(BlueprintReadWrite, Category = "NPC")
    FString CurrentNPCId;

protected:
    UFUNCTION()
    void HandleSendButtonClicked();

    UFUNCTION()
    void HandleCloseButtonClicked();

    // UI 바인딩 변수들
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> ChatLog;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UEditableTextBox> InputBox;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UButton> CloseButton;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UButton> SendButton;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> ServerStatusText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> ResponseTimeText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> TimeText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Chat|Appearance")
    FSlateFontInfo ChatFont;
};