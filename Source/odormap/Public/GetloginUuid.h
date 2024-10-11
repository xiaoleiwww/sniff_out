// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GetloginUuid.generated.h"

UCLASS()
class ODORMAP_API UGetloginUuid : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	// Store the received Uuid
	static FString StoredUuid;

public:
	// Sends a login request and stores the Uuid
	UFUNCTION(BlueprintCallable, Category = "OdorJson")
	static void RequestUuid(FString URL, FString Username, FString Password);

	// Gets the stored Uuid
	UFUNCTION(BlueprintPure, Category = "OdorJson")
	static FString GetStoredUuid();
};
