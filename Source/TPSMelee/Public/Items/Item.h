// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class TPSMELEE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	/**
	 * Variables
	 */

	/* Components */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;
};
