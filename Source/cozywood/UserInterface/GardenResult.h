#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GardenResult.generated.h"

class UButton;
class UTextBlock;
class UEditableTextBox;
class UWidgetSwitcher;

UCLASS()
class COZYWOOD_API UGardenResultWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    // 1단계: 감정 선택 관련
    UPROPERTY(EditAnywhere, Category = "Settings")
    TArray<FString> EmotionPool; // 12개의 감정 단어들

    UPROPERTY()
    TArray<FString> SelectedEmotions; // 유저가 선택한 단어 (최대 3개)

    // UI 바인딩
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* StepSwitcher; // 1단계/2단계 화면 전환용

    UPROPERTY(meta = (BindWidget))
    UButton* NextButton; // 다음 버튼

    // 2단계: 이름 입력 관련
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SelectedEmotionsText; // 감정키워드 표시용

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* GardenNameInput; // 정원 이름 입력창

    UPROPERTY(meta = (BindWidget))
    UButton* SaveButton; // 저장 버튼

    UFUNCTION()
    void HandleEmotionClick(FString SelectedWord);

    UFUNCTION()
    void GoToNextStep();

    UFUNCTION()
    void SaveAndSendToServer();
};