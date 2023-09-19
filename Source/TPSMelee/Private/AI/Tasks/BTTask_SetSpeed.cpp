// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetSpeed.h"

#include "Characters/Enemy/BaseAIController.h"
#include "Characters/Enemy/BaseEnemy.h"

UBTTask_SetSpeed::UBTTask_SetSpeed()
{
	NodeName = TEXT("Set Speed");
}

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if(!BaseEnemy) return EBTNodeResult::Failed;

	BaseEnemy->SetEnemySpeedState(SpeedState);
	return EBTNodeResult::Succeeded;
}
