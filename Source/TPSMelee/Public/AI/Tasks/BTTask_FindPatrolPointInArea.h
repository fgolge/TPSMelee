// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPatrolPointInArea.generated.h"

/**
 * 
 */
UCLASS()
class TPSMELEE_API UBTTask_FindPatrolPointInArea : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolPointInArea();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	UPROPERTY(EditAnywhere, Category = "Default")
	FBlackboardKeySelector PatrolPointKey;

	UPROPERTY(EditAnywhere, Category = "Default")
	float PatrolRadius { 500.f };
	
};
