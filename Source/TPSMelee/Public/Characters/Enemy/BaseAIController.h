// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyStates.h"
#include "BaseAIController.generated.h"

class ABaseEnemy;
class UBehaviorTreeComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

/**
 * 
 */
UCLASS()
class TPSMELEE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;


};
