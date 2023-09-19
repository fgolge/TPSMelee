// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FinishInvestigation.generated.h"

/**
 * 
 */
UCLASS()
class TPSMELEE_API UBTTask_FinishInvestigation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FinishInvestigation();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
