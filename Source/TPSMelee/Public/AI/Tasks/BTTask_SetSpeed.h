// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Characters/Enemy/EnemyStates.h"
#include "BTTask_SetSpeed.generated.h"

/**
 * 
 */
UCLASS()
class TPSMELEE_API UBTTask_SetSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetSpeed();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = "Default")
	EEnemySpeedState SpeedState;
	
};
