#include "C1PlayerController.h"
#include "../NPC/NPCChatWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "../NPC/ServerInterface/LLMManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "../UserInterface/Shop/ShopDialogueWidget.h"
#include "../UserInterface/Shop/ShopBuyWidget.h"
#include "../UserInterface/Shop/ShopSellWidget.h"

void AC1PlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;

    // 1. LLMManager 인스턴스 생성 또는 스폰
    if (LLMManagerClass)
    {
        LLMManagerInstance = GetWorld()->SpawnActor<ALLMManager>(LLMManagerClass);
    }

    // 2. 델리게이트 바인딩 (LLM -> PC)
    if (LLMManagerInstance)
    {
        LLMManagerInstance->OnChatResponse.AddDynamic(this, &AC1PlayerController::HandleChatResponse);
        LLMManagerInstance->OnGreetingResponse.AddDynamic(this, &AC1PlayerController::HandleGreetingResponse);
        LLMManagerInstance->OnServerHealthChecked.AddDynamic(this, &AC1PlayerController::HandleServerHealthChecked);
    }

    GetWorldTimerManager().SetTimer(TimeUpdateTimerHandle, this, &AC1PlayerController::UpdateClockText, 1.0f, true);
    GetWorldTimerManager().SetTimer(HealthCheckTimerHandle, this, &AC1PlayerController::RequestServerHealthCheck, 3.0f, true);
}

void AC1PlayerController::OpenNPCChat(const FString& NpcId)
{
    if (!NPCChatWidgetInstance && NPCChatWidgetClass)
    {
        NPCChatWidgetInstance = CreateWidget<UNPCChatWidget>(this, NPCChatWidgetClass);
    }

    if (NPCChatWidgetInstance)
    {

        NPCChatWidgetInstance->SetNpcId(NpcId);

        if (!NPCChatWidgetInstance->IsInViewport())
        {
            NPCChatWidgetInstance->AddToViewport(10);

            if (LLMManagerInstance) { LLMManagerInstance->RequestGreeting(NpcId, TEXT("player1")); }

            FInputModeGameAndUI InputMode;
            TSharedPtr<SWidget> CachedWidget = NPCChatWidgetInstance->GetCachedWidget();
            if (CachedWidget.IsValid()) { InputMode.SetWidgetToFocus(CachedWidget); }

            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetHideCursorDuringCapture(false);
            SetInputMode(InputMode);

            bShowMouseCursor = true;

            SetIgnoreLookInput(true);
            SetIgnoreMoveInput(true);
        }
    }
}

void AC1PlayerController::SendCurrentChatMessage()
{
    // 1. 위젯이 유효하고 인풋박스에 글자가 있는지 확인
    if (NPCChatWidgetInstance)
    {
        FString UserMessage = NPCChatWidgetInstance->GetInputMessage();
        if (UserMessage.IsEmpty()) return;

        // 2. 내 채팅 로그에 먼저 표시
        NPCChatWidgetInstance->AddMessageToChat(FString::Printf(TEXT("Player: %s"), *UserMessage));

        // 3. 서버(LLMManager)에 전송
        if (LLMManagerInstance)
        {
            FString NpcId = NPCChatWidgetInstance->GetNpcId();
            LLMManagerInstance->SendChatMessage(NpcId, TEXT("player1"), UserMessage);
        }

        // 4. 입력창 비우기
        NPCChatWidgetInstance->ClearInputMessage();
    }
}

void AC1PlayerController::HandleChatResponse(const FString& InReply, float ResponseTimeSeconds)
{
    if (NPCChatWidgetInstance)
    {
        // 위젯의 통합 함수 호출 (메시지 추가 + 시간 표시 + 상태 갱신)
        NPCChatWidgetInstance->AddChatMessage(TEXT("NPC: ") + InReply, ResponseTimeSeconds);
    }
}

void AC1PlayerController::HandleGreetingResponse(const FString& InGreeting, float ResponseTimeSeconds)
{
    if (NPCChatWidgetInstance)
    {
        NPCChatWidgetInstance->AddChatMessage(TEXT("NPC: ") + InGreeting, ResponseTimeSeconds);
    }
}

void AC1PlayerController::HandleServerHealthChecked(bool bIsConnected)
{
    if (NPCChatWidgetInstance)
    {
        NPCChatWidgetInstance->SetServerStatusText(bIsConnected ? TEXT("Online") : TEXT("Offline"));
    }
}

void AC1PlayerController::RequestServerHealthCheck()
{
    if (LLMManagerInstance) LLMManagerInstance->CheckServerHealth();
}

void AC1PlayerController::UpdateClockText()
{
    if (NPCChatWidgetInstance)
    {
        NPCChatWidgetInstance->SetTimeText(FDateTime::Now().ToString(TEXT("%H:%M:%S")));
    }
}

void AC1PlayerController::CloseNPCChat()
{
    // 1. 위젯 제거 및 정리
    if (NPCChatWidgetInstance)
    {
        NPCChatWidgetInstance->RemoveFromParent();
        NPCChatWidgetInstance = nullptr;
    }

    // 2. 모든 무시 설정 강제 해제
    SetIgnoreLookInput(false);
    SetIgnoreMoveInput(false);

    // 3. 입력 모드를 Game Only로 초기화
    FInputModeGameOnly InputMode;
    // 이 옵션이 중요합니다: 마우스 클릭 시 즉시 뷰포트로 포커스를 가져옵니다.
    InputMode.SetConsumeCaptureMouseDown(true);
    SetInputMode(InputMode);

    // 4. 커서 숨기기
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;

    // UI에 뺏겼던 마우스 제어권을 즉시 게임 엔진이 회수
    FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

bool AC1PlayerController::IsShopOpen() const
{
	return (ShopDialogueWidgetInstance && ShopDialogueWidgetInstance->IsInViewport())
		|| (ShopBuyWidgetInstance && ShopBuyWidgetInstance->IsInViewport())
		|| (ShopSellWidgetInstance && ShopSellWidgetInstance->IsInViewport());
}

void AC1PlayerController::ApplyShopInputMode(UUserWidget* FocusWidget)
{
	if (!FocusWidget) return;

	FInputModeGameAndUI InputMode;
	TSharedPtr<SWidget> CachedWidget = FocusWidget->GetCachedWidget();
	if (CachedWidget.IsValid())
	{
		InputMode.SetWidgetToFocus(CachedWidget);
	}

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
	bShowMouseCursor = true;

	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
}

void AC1PlayerController::RestoreGameInputMode()
{
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

void AC1PlayerController::OpenShopModeSelect()
{
	if (IsChatOpen())
	{
		return;
	}

	if (!ShopDialogueWidgetInstance && ShopDialogueWidgetClass)
	{
		ShopDialogueWidgetInstance = CreateWidget<UShopDialogueWidget>(this, ShopDialogueWidgetClass);
	}

	if (!ShopDialogueWidgetInstance)
	{
		return;
	}

	if (ShopBuyWidgetInstance)
	{
		ShopBuyWidgetInstance->RemoveFromParent();
	}

	if (ShopSellWidgetInstance)
	{
		ShopSellWidgetInstance->RemoveFromParent();
	}

	if (!ShopDialogueWidgetInstance->IsInViewport())
	{
		ShopDialogueWidgetInstance->AddToViewport(20);
	}

	ApplyShopInputMode(ShopDialogueWidgetInstance);
}

void AC1PlayerController::OpenShopBuy()
{
	if (!ShopBuyWidgetInstance && ShopBuyWidgetClass)
	{
		ShopBuyWidgetInstance = CreateWidget<UShopBuyWidget>(this, ShopBuyWidgetClass);
	}

	if (!ShopBuyWidgetInstance)
	{
		return;
	}

	if (ShopDialogueWidgetInstance)
	{
		ShopDialogueWidgetInstance->RemoveFromParent();
	}

	if (ShopSellWidgetInstance)
	{
		ShopSellWidgetInstance->RemoveFromParent();
	}

	if (!ShopBuyWidgetInstance->IsInViewport())
	{
		ShopBuyWidgetInstance->AddToViewport(21);
	}

	ApplyShopInputMode(ShopBuyWidgetInstance);
}

void AC1PlayerController::OpenShopSell()
{
    if (!ShopSellWidgetInstance && ShopSellWidgetClass)
    {
        ShopSellWidgetInstance = CreateWidget<UShopSellWidget>(this, ShopSellWidgetClass);
    }

    if (!ShopSellWidgetInstance)
    {
        return;
    }

    if (ShopDialogueWidgetInstance)
    {
        ShopDialogueWidgetInstance->RemoveFromParent();
    }

    if (ShopBuyWidgetInstance)
    {
        ShopBuyWidgetInstance->RemoveFromParent();
    }

    if (!ShopSellWidgetInstance->IsInViewport())
    {
        ShopSellWidgetInstance->AddToViewport(21);
    }

    ApplyShopInputMode(ShopSellWidgetInstance);
}

void AC1PlayerController::CloseShopUI()
{
    if (ShopDialogueWidgetInstance)
    {
        ShopDialogueWidgetInstance->RemoveFromParent();
    }

    if (ShopBuyWidgetInstance)
    {
        ShopBuyWidgetInstance->RemoveFromParent();
    }

    if (ShopSellWidgetInstance)
    {
        ShopSellWidgetInstance->RemoveFromParent();
    }

    RestoreGameInputMode();
}