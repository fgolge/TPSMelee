// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateLocationOfTarget.generated.h"

/**
 * 
 */
UCLASS()
class TPSMELEE_API UBTService_UpdateLocationOfTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdateLocationOfTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	FBlackboardKeySelector Location;

	UPROPERTY(EditAnywhere, Category = "Default")
	FBlackboardKeySelector TargetActor;
};
