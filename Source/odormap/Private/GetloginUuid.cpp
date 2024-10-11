// Fill out your copyright notice in the Description page of Project Settings.
#include "GetloginUuid.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"

// Initialize the static member variable
FString UGetloginUuid::StoredUuid = TEXT("");

void UGetloginUuid::RequestUuid(FString URL, FString Username, FString Password)
{
    // Create JSON payload
    FString PostData = FString::Printf(TEXT(R"({"username": "%s", "password": "%s"})"), *Username, *Password);

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
                // Extract UUID from the JSON response
                FString ResponseContent = Response->GetContentAsString();
                TSharedPtr<FJsonObject> JsonObject;
                TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseContent);

                if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    // Save the Uuid to the static member variable
                    UGetloginUuid::StoredUuid = JsonObject->GetStringField(TEXT("uuid"));
                    UE_LOG(LogTemp, Log, TEXT("UUID received: %s"), *UGetloginUuid::StoredUuid);
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
    HttpRequest->ProcessRequest();
}

FString UGetloginUuid::GetStoredUuid()
{
    // Return the stored Uuid
    return StoredUuid;
}