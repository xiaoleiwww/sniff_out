// Fill out your copyright notice in the Description page of Project Settings.
#include "OdorJsonSender.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"

int UOdorJsonSender::SendJson(FString URL, FString UUID, FString Event)
{
    // Create JSON payload
    FString PostData = FString::Printf(TEXT(R"({"uuid": "%s", "event": "%s"})"), *UUID, *Event);

    // Create HTTP request
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetContentAsString(PostData);
    
    // Send the request and handle response
    HttpRequest->OnProcessRequestComplete().BindLambda(
        [](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                // Extract error_code from the JSON response
                FString ResponseContent = Response->GetContentAsString();
                TSharedPtr<FJsonObject> JsonObject;
                TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseContent);

                if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    FString ErrorCode = JsonObject->GetStringField(TEXT("error_code"));
                    // Log the error_code
                    UE_LOG(LogTemp, Log, TEXT("Error Code: %s"), *ErrorCode);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON response."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Request failed or response is invalid."));
            }
        });

    // Send the request
    if (HttpRequest->ProcessRequest())
    {
        return 0; // Return 0 on success
    }

    return -1; // Return -1 on failure
}
