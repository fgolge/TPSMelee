// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_RNGAttack.generated.h"

/**
 * 
 */
UCLASS()
class TPSMELEE_API UBTService_RNGAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_RNGAttack();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

private:
	bool ShouldAttack();

public:
	UPROPERTY(EditAnywhere, Category = "Default")
	FBlackboardKeySelector DistanceToTargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	float RNGFraction { 0.4f };
};
