#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "LLMManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatResponse, const FString&, Reply, float, ResponseTimeSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGreetingResponse, const FString&, Greeting, float, ResponseTimeSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServerHealthChecked, bool, bIsConnected);

UCLASS()
class COZYWOOD_API ALLMManager : public AActor
{
    GENERATED_BODY()

public:
    ALLMManager();

    UPROPERTY(BlueprintAssignable, Category = "LLM")
    FOnChatResponse OnChatResponse;

    UPROPERTY(BlueprintAssignable, Category = "LLM")
    FOnGreetingResponse OnGreetingResponse;

    UPROPERTY(BlueprintAssignable, Category = "LLM")
    FOnServerHealthChecked OnServerHealthChecked;

    UFUNCTION(BlueprintCallable, Category = "LLM")
    void SendChatMessage(const FString& NpcId, const FString& PlayerId, const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "LLM")
    void RequestGreeting(const FString& NpcId, const FString& PlayerId);

    UFUNCTION(BlueprintCallable, Category = "LLM")
    void CheckServerHealth();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM")
    FString ServerBaseUrl = TEXT("http://127.0.0.1:8000");


private:
    void OnChatResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void OnGreetingResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void OnHealthResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    double RequestStartTimeSeconds;
    FString PendingRequestType;
};