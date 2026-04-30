#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "NetworkComponent.generated.h"

// 서버 통신 결과를 UI 블루프린트에 알려주기 위한 Delegate 설치
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServerResponse, bool, bWasSuccessful);

UCLASS(ClassGroup = (Network), meta = (BlueprintSpawnableComponent))
class COZYWOOD_API UNetworkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNetworkComponent();

	// UI에서 버튼 누를 때 호출할 함수
	UFUNCTION(BlueprintCallable, Category = "Network")
	void SendEmotionToServer(FString EmotionKeyword);

	// 블루프린트에서 이벤트로 연결할 수 있는 핀
	UPROPERTY(BlueprintAssignable, Category = "Network|Event")
	FOnServerResponse OnServerResponseEvent;

	// 에디터에서 직접 주소를 바꿀 수 있도록 열어둔 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|Settings")
	FString ServerApiUrl = TEXT("http://127.0.0.1:8000/api/emotion");

private:
	// 내부적으로 서버 응답을 처리할 콜백 함수
	void OnSendEmotionComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};