#include "LLMManager.h"

#include "HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HAL/PlatformTime.h"

ALLMManager::ALLMManager()
{
    PrimaryActorTick.bCanEverTick = false;
    RequestStartTimeSeconds = 0.0;
}

void ALLMManager::BeginPlay()
{
    Super::BeginPlay();
}

void ALLMManager::SendChatMessage(const FString& NpcId, const FString& PlayerId, const FString& Message)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(TEXT("player_id"), PlayerId.IsEmpty() ? TEXT("player001") : PlayerId);
    JsonObject->SetStringField(TEXT("npc_id"), NpcId);
    JsonObject->SetStringField(TEXT("message"), Message);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    RequestStartTimeSeconds = FPlatformTime::Seconds();
    PendingRequestType = TEXT("chat");

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(ServerBaseUrl + TEXT("/chat"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
    Request->SetContentAsString(RequestBody);
    Request->OnProcessRequestComplete().BindUObject(this, &ALLMManager::OnChatResponseReceived);
    Request->ProcessRequest();
}

void ALLMManager::RequestGreeting(const FString& NpcId, const FString& PlayerId)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(TEXT("player_id"), PlayerId.IsEmpty() ? TEXT("player001") : PlayerId);
    JsonObject->SetStringField(TEXT("npc_id"), NpcId);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    RequestStartTimeSeconds = FPlatformTime::Seconds();
    PendingRequestType = TEXT("greeting");

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(ServerBaseUrl + TEXT("/npc_greeting"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
    Request->SetContentAsString(RequestBody);
    Request->OnProcessRequestComplete().BindUObject(this, &ALLMManager::OnGreetingResponseReceived);
    Request->ProcessRequest();
}

void ALLMManager::CheckServerHealth()
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(ServerBaseUrl + TEXT("/health"));
    Request->SetVerb(TEXT("GET"));
    Request->OnProcessRequestComplete().BindUObject(this, &ALLMManager::OnHealthResponseReceived);
    Request->ProcessRequest();
}

void ALLMManager::OnChatResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    const float ResponseTimeSeconds = static_cast<float>(FPlatformTime::Seconds() - RequestStartTimeSeconds);

    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Chat HTTP request failed"));
        OnChatResponse.Broadcast(TEXT("Server Connect Fail"), ResponseTimeSeconds);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Chat Response Code: %d"), Response->GetResponseCode());
    UE_LOG(LogTemp, Warning, TEXT("Chat Response Body: %s"), *Response->GetContentAsString());

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FString Reply;
        if (JsonObject->TryGetStringField(TEXT("response"), Reply) ||
            JsonObject->TryGetStringField(TEXT("reply"), Reply))
        {
            OnChatResponse.Broadcast(Reply, ResponseTimeSeconds);
            return;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("Chat JSON parse failed"));
    OnChatResponse.Broadcast(TEXT("Reply parsing fail"), ResponseTimeSeconds);
}

void ALLMManager::OnGreetingResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    const float ResponseTimeSeconds = static_cast<float>(FPlatformTime::Seconds() - RequestStartTimeSeconds);

    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Greeting HTTP request failed"));
        OnGreetingResponse.Broadcast(TEXT("..."), ResponseTimeSeconds);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Greeting Response Code: %d"), Response->GetResponseCode());
    UE_LOG(LogTemp, Warning, TEXT("Greeting Response Body: %s"), *Response->GetContentAsString());

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    FString Greeting = TEXT("...");
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        JsonObject->TryGetStringField(TEXT("greeting"), Greeting);
    }

    OnGreetingResponse.Broadcast(Greeting, ResponseTimeSeconds);
}

void ALLMManager::OnHealthResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        OnServerHealthChecked.Broadcast(false);
        return;
    }

    const int32 ResponseCode = Response->GetResponseCode();
    OnServerHealthChecked.Broadcast(ResponseCode >= 200 && ResponseCode < 300);
}