// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FinishInvestigation.h"

#include "Characters/Enemy/BaseAIController.h"

UBTTask_FinishInvestigation::UBTTask_FinishInvestigation()
{
	NodeName = TEXT("Finish Investigation");
}

EBTNodeResult::Type UBTTask_FinishInvestigation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if(!AIController) return EBTNodeResult::Failed;

	AIController->Patrol();

	return EBTNodeResult::Succeeded;
}
