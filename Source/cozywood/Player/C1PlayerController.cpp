#include "C1PlayerController.h"
#include "Framework/Application/SlateApplication.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "../UserInterface/Shop/ShopModeSelectWidget.h"
#include "../UserInterface/Shop/ShopBuyWidget.h"
#include "../UserInterface/Shop/ShopSellWidget.h"
#include "../Character/C1Character.h"
#include "../Components/InventoryComponent.h"
#include "../Components/EconomyComponent.h"
#include "../Items/ItemBase.h"

void AC1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AC1PlayerController::HandleEscapeKey);
}

void AC1PlayerController::HandleEscapeKey()
{
	if (IsShopOpen()) { CloseShopUI(); return; }
}

void AC1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

}


bool AC1PlayerController::IsShopOpen() const
{
	return (ShopModeSelectWidgetInstance && ShopModeSelectWidgetInstance.Get()->IsInViewport())
		|| (ShopBuyWidgetInstance        && ShopBuyWidgetInstance.Get()->IsInViewport())
		|| (ShopSellWidgetInstance       && ShopSellWidgetInstance.Get()->IsInViewport());
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
}

void AC1PlayerController::RestoreGameInputMode()
{
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

	// C4244 fix: GetTimeSeconds()는 double 반환 → float으로 캐스트
	float Elapsed = GetWorld() ? static_cast<float>(GetWorld()->GetTimeSeconds()) - ShopLastCloseTime : 999.f;
	if (Elapsed < 0.3f) { UE_LOG(LogTemp, Warning, TEXT("[OpenShopModeSelect] blocked: cooldown %.2f"), Elapsed); return; }

	UE_LOG(LogTemp, Warning, TEXT("[OpenShopModeSelect] WidgetClass=%s  WidgetInstance=%s"),
		ShopModeSelectWidgetClass ? TEXT("valid") : TEXT("NULL"),
		ShopModeSelectWidgetInstance ? TEXT("exists") : TEXT("NULL"));

	if (!ShopModeSelectWidgetInstance && ShopModeSelectWidgetClass)
	{
		// C2672 fix: TSubclassOf<UShopModeSelectWidget> → TSubclassOf<UUserWidget> 명시적 변환
		TSubclassOf<UUserWidget> WClass = ShopModeSelectWidgetClass;
		ShopModeSelectWidgetInstance = CreateWidget<UShopModeSelectWidget>(this, WClass);
	}

	if (!ShopModeSelectWidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[OpenShopModeSelect] widget creation failed — ShopModeSelectWidgetClass not assigned in BP?"));
		return;
	}

	// C2039 fix: .Get() 사용
	if (ShopBuyWidgetInstance)  ShopBuyWidgetInstance.Get()->RemoveFromParent();
	if (ShopSellWidgetInstance && ShopSellWidgetInstance->IsInViewport()) ShopSellWidgetInstance.Get()->RemoveFromParent();

	if (!ShopModeSelectWidgetInstance.Get()->IsInViewport())
	{
		ShopModeSelectWidgetInstance.Get()->AddToViewport(20);
	}

	// C2664 fix: .Get()으로 UUserWidget* 변환
	ApplyShopInputMode(ShopModeSelectWidgetInstance.Get());
}

void AC1PlayerController::OpenShopBuy()
{
	if (!ShopBuyWidgetInstance && ShopBuyWidgetClass)
	{
		TSubclassOf<UUserWidget> WClass = ShopBuyWidgetClass;
		ShopBuyWidgetInstance = CreateWidget<UShopBuyWidget>(this, WClass);
	}

	if (!ShopBuyWidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenShopBuy: ShopBuyWidgetClass가 Blueprint에 할당되지 않았습니다"));
		RestoreGameInputMode();
		return;
	}

	if (ShopModeSelectWidgetInstance) ShopModeSelectWidgetInstance.Get()->RemoveFromParent();
	if (ShopSellWidgetInstance)       ShopSellWidgetInstance.Get()->RemoveFromParent();

	if (!ShopBuyWidgetInstance.Get()->IsInViewport())
	{
		ShopBuyWidgetInstance.Get()->AddToViewport(21);
	}

	ShopBuyWidgetInstance.Get()->RebuildShopList();
	ApplyShopInputMode(ShopBuyWidgetInstance.Get());
}

void AC1PlayerController::OpenShopSell()
{
	if (!ShopSellWidgetInstance && ShopSellWidgetClass)
	{
		TSubclassOf<UUserWidget> WClass = ShopSellWidgetClass;
		ShopSellWidgetInstance = CreateWidget<UShopSellWidget>(this, WClass);
	}

	if (!ShopSellWidgetInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenShopSell: ShopSellWidgetClass가 Blueprint에 할당되지 않았습니다"));
		RestoreGameInputMode();
		return;
	}

	if (ShopModeSelectWidgetInstance) ShopModeSelectWidgetInstance.Get()->RemoveFromParent();
	if (ShopBuyWidgetInstance)        ShopBuyWidgetInstance.Get()->RemoveFromParent();

	if (!ShopSellWidgetInstance.Get()->IsInViewport())
	{
		ShopSellWidgetInstance.Get()->AddToViewport(21);
	}

	ShopSellWidgetInstance.Get()->RebuildSellList();
	ApplyShopInputMode(ShopSellWidgetInstance.Get());
}

void AC1PlayerController::CloseShopUI()
{
	if (ShopModeSelectWidgetInstance)
	{
		ShopModeSelectWidgetInstance.Get()->RemoveFromParent();
		ShopModeSelectWidgetInstance = nullptr;
	}
	if (ShopBuyWidgetInstance)
	{
		ShopBuyWidgetInstance.Get()->RemoveFromParent();
		ShopBuyWidgetInstance = nullptr;
	}
	if (ShopSellWidgetInstance)
	{
		ShopSellWidgetInstance.Get()->RemoveFromParent();
		ShopSellWidgetInstance = nullptr;
	}

	RestoreGameInputMode();
	ShopLastCloseTime = GetWorld() ? static_cast<float>(GetWorld()->GetTimeSeconds()) : -1.0f;
}

bool AC1PlayerController::BuyItem(FName ItemID, int32 Price, int32 Quantity)
{
	if (ItemID.IsNone() || Quantity <= 0) return false;

	AC1Character* OwnerChar = Cast<AC1Character>(GetPawn());
	if (!OwnerChar) return false;

	UEconomyComponent* Economy     = OwnerChar->GetEconomyComp();
	UInventoryComponent* Inventory = OwnerChar->GetInventory();
	if (!Economy || !Inventory) return false;

	if (!Economy->SpendMoney(Price)) return false;

	Inventory->AddStartingItemByID(ItemID, Quantity);
	UE_LOG(LogTemp, Log, TEXT("구매 완료: %s x%d (-%d G)"), *ItemID.ToString(), Quantity, Price);
	return true;
}

bool AC1PlayerController::SellItem(UItemBase* ItemToSell, int32 SellPrice, int32 Quantity)
{
	if (!ItemToSell || Quantity <= 0) return false;
	if (ItemToSell->Quantity < Quantity) return false;

	AC1Character* OwnerChar = Cast<AC1Character>(GetPawn());
	if (!OwnerChar) return false;

	UEconomyComponent* Economy     = OwnerChar->GetEconomyComp();
	UInventoryComponent* Inventory = OwnerChar->GetInventory();
	if (!Economy || !Inventory) return false;

	Inventory->RemoveAmountOfItem(ItemToSell, Quantity);
	Economy->AddMoney(SellPrice * Quantity);
	UE_LOG(LogTemp, Log, TEXT("판매 완료: %s x%d (+%d G)"), *ItemToSell->ItemID.ToString(), Quantity, SellPrice * Quantity);
	return true;
}
