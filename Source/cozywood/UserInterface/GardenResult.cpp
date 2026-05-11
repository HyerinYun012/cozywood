#include "GardenResult.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

void UGardenResultWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 다음 버튼 클릭 시 2단계로 이동
    if (NextButton)
    {
        NextButton->OnClicked.AddDynamic(this, &UGardenResultWidget::GoToNextStep);
    }

    // 저장 버튼 클릭 시 서버 전송
    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this, &UGardenResultWidget::SaveAndSendToServer);
    }
}

// 감정 버튼 클릭 시 호출 (블루프린트에서 각 버튼마다 단어를 인자로 넣어 호출하게 설정)
void UGardenResultWidget::HandleEmotionClick(FString SelectedWord)
{
    // 이미 선택한 단어면 취소
    if (SelectedEmotions.Contains(SelectedWord))
    {
        SelectedEmotions.Remove(SelectedWord);
        return;
    }

    // 3개까지만 선택 가능
    if (SelectedEmotions.Num() < 3)
    {
        SelectedEmotions.Add(SelectedWord);
    }
}

void UGardenResultWidget::GoToNextStep()
{
    if (StepSwitcher)
    {
        // 2단계 화면으로 전환
        StepSwitcher->SetActiveWidgetIndex(1);

        // 선택한 키워드들을 텍스트로 나열
        FString CombinedText = FString::Join(SelectedEmotions, TEXT(", "));
        SelectedEmotionsText->SetText(FText::FromString(TEXT("감정키워드 : ") + CombinedText));
    }
}

void UGardenResultWidget::SaveAndSendToServer()
{
    FString GardenName = GardenNameInput->GetText().ToString();


    UE_LOG(LogTemp, Warning, TEXT("서버 전송 완료: %s 정원"), *GardenName);
    RemoveFromParent(); // 위젯 닫기
}