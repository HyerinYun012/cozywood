#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;

UENUM()
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
		ActualAmountAdded(0),
		OperationResult(EItemAddResult::IAR_NoItemAdded),
		ResultMessage(FText::GetEmpty()) {
	};

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText) {
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;

		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;

		return AddedPartialResult;
	};

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message) {
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;

		return AddedAllResult;
	};
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
	class COZYWOOD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInventoryUpdated OnInventoryUpdated;

	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);

	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);

	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetSlotsCapacity() const { return InventorySlotsCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemByActorClass(TSubclassOf<AActor> InClass);

	// ID를 받아서 해당 아이템 객체(UItemBase)을 인벤토리에 추가하는 함수 (로드 및 픽업 시 사용)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddStartingItemByID(FName ItemID, int32 AmountToAdd = 1);

	// 검사하는 데이터 테이블 라이브러리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Initialization")
	class UDataTable* ItemDataTable;

protected:
	// 저장/불러오기에 사용할 슬롯 이름 (GardenSave와 동일한 파일 공유)
	UPROPERTY(EditDefaultsOnly, Category = "Inventory|SaveLoad")
	FString SaveSlotName = TEXT("GardenSaveSlot");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySlotsCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);

private:
	void SaveInventory();
	void LoadInventory();

public:
	UInventoryComponent(const FOnInventoryUpdated& OnInventoryUpdated, const int32& InventorySlotsCapacity, const TArray<TObjectPtr<UItemBase>>& InventoryContents)
		: OnInventoryUpdated(OnInventoryUpdated), InventorySlotsCapacity(InventorySlotsCapacity), InventoryContents(InventoryContents) {};
	bool operator==(const UInventoryComponent& other) const;
};