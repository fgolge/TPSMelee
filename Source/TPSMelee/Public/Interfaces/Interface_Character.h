// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Character.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_Character : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPSMELEE_API IInterface_Character
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
