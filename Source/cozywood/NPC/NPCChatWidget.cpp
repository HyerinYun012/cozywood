#include "NPCChatWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "../Player/C1PlayerController.h"
#include "Blueprint/WidgetTree.h"

void UNPCChatWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SendButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("SendButton 연결 성공!"));
        SendButton->OnClicked.AddUniqueDynamic(this, &UNPCChatWidget::HandleSendButtonClicked);
    }
    else { UE_LOG(LogTemp, Error, TEXT("SendButton을 찾을 수 없습니다!")); }

    if (CloseButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("CloseButton 연결 성공!"));
        CloseButton->OnClicked.AddUniqueDynamic(this, &UNPCChatWidget::HandleCloseButtonClicked);
    }
    else { UE_LOG(LogTemp, Error, TEXT("CloseButton을 찾을 수 없습니다!")); }
}

void UNPCChatWidget::HandleCloseButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("닫기 버튼 눌림!"));

    // 1. 만약 GetOwningPlayer()로 못 가져올 경우를 대비해 GetWorld() 방식도 같이 사용
    AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer());
    if (!PC)
    {
        PC = Cast<AC1PlayerController>(GetWorld()->GetFirstPlayerController());
    }

    if (PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("컨트롤러 찾음! CloseNPCChat() 호출합니다."));
        PC->CloseNPCChat();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("컨트롤러를 찾지 못했습니다! 강제로 창을 닫습니다."));
    }

    this->RemoveFromParent();
}

// PlayerController가 서버 응답을 받으면 이 함수를 호출
void UNPCChatWidget::AddChatMessage(const FString& NewText, float ResponseTimeSeconds)
{
    // 메시지 추가
    AddMessageToChat(NewText);

    // 응답 시간 표시 (예: Response: 0.45s)
    FString TimeStr = FString::Printf(TEXT("Response: %.2fs"), ResponseTimeSeconds);
    SetResponseTimeText(TimeStr);

    // 상태 표시
    SetServerStatusText(TEXT("Connected"));
}

void UNPCChatWidget::AddMessageToChat(const FString& Message)
{
    if (!ChatLog) return;

    UTextBlock* NewMessage = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

    if (NewMessage)
    {
        NewMessage->SetText(FText::FromString(Message));
        NewMessage->SetAutoWrapText(true);

        NewMessage->SetFont(ChatFont);

        // 글자 색상 검은색으로 설정
        NewMessage->SetColorAndOpacity(FSlateColor(FLinearColor::Black));

        ChatLog->AddChildToVerticalBox(NewMessage);

        // 스크롤바를 통해 스크롤을 맨 아래로 내리는 로직 추가 구현 필요
    }
}

// UI 컴포넌트 접근 함수들
void UNPCChatWidget::SetNpcId(const FString& InNpcId) { CurrentNPCId = InNpcId; }
FString UNPCChatWidget::GetNpcId() const { return CurrentNPCId; }
FString UNPCChatWidget::GetInputMessage() const { return InputBox ? InputBox->GetText().ToString() : TEXT(""); }
void UNPCChatWidget::ClearInputMessage() { if (InputBox) InputBox->SetText(FText::GetEmpty()); }
void UNPCChatWidget::SetServerStatusText(const FString& NewText) { if (ServerStatusText) ServerStatusText->SetText(FText::FromString(NewText)); }
void UNPCChatWidget::SetResponseTimeText(const FString& NewText) { if (ResponseTimeText) ResponseTimeText->SetText(FText::FromString(NewText)); }
void UNPCChatWidget::SetTimeText(const FString& NewText) { if (TimeText) TimeText->SetText(FText::FromString(NewText)); }

void UNPCChatWidget::HandleSendButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("전송 버튼 눌림! 입력한 텍스트: %s"), *GetInputMessage())

    if (AC1PlayerController* PC = Cast<AC1PlayerController>(GetOwningPlayer()))
    {
        PC->SendCurrentChatMessage();
    }
}