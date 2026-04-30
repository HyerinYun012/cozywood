#include "NetworkComponent.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

UNetworkComponent::UNetworkComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // 네트워크 부서는 틱이 필요 없음
}

void UNetworkComponent::SendEmotionToServer(FString EmotionKeyword)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UNetworkComponent::OnSendEmotionComplete);

	Request->SetURL(ServerApiUrl); // 실제 서버 주소
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	// 감정 키워드 JSON 포장
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("emotion", EmotionKeyword);

	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();

	UE_LOG(LogTemp, Log, TEXT("네트워크 부서: 감정 데이터 서버로 발송 중... (%s)"), *EmotionKeyword);
}

void UNetworkComponent::OnSendEmotionComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	bool bIsOk = false;

	if (bWasSuccessful && Response.IsValid())
	{
		// HTTP 상태 코드가 200번대(성공)인지 확인
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			bIsOk = true;
			UE_LOG(LogTemp, Log, TEXT("네트워크 부서: 전송 성공! 응답 = %s"), *Response->GetContentAsString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("네트워크 부서: 서버 통신 실패!"));
	}

	// UI 블루프린트 쪽으로 성공/실패 여부를 Broadcast
	OnServerResponseEvent.Broadcast(bIsOk);
}