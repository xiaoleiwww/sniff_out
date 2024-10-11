// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OdorJsonSender.generated.h"

UCLASS()
class ODORMAP_API UOdorJsonSender : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintPure, Category = "OdorJson")
	static int SendJson(FString URL, FString UUID, FString Event);
};