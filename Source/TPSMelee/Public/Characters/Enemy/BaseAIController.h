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

private:
	/**
	 * Functions
	 */

	/* Combat */
	bool ShouldChase(FAIStimulus Stimulus);
	void Investigate();
	bool ShouldInvestigate();
	void ChaseTimerHandler();
	void EngageTimerHandler();
	void Engage();
	bool ShouldEngage();
	bool ShouldDisengage();
	bool ShouldFallBack();
	void SetTargetActor(AActor* Actor);
	void FocusOnTarget(bool bShouldFocus);
	void SetEnemyControl(bool bIsFocused);
	bool IsAttacking();
	void HandleEnemyDeath();
	
	/* Debug */
	void DrawBoundaries() const;
	
	/**
	 * Variables
	 */
	/* Combat */
	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	float DistanceToTarget { 9999999.f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float TimeRateForDistanceCheck { .1f };

	UPROPERTY(EditAnywhere, Category = "Combat | Chasing")
	float ChaseToEngageDistance { 300.f };

	UPROPERTY(EditAnywhere, Category = "Combat | Chasing")
	float MaxChaseDistance { 1500.f };

	UPROPERTY(EditAnywhere, Category = "Combat | Chasing")
	float BackwardSensingDistance { 500.f };

	UPROPERTY(EditAnywhere, Category = "Combat | Engage")
	float MaxEngageDistance { 200.f };

	UPROPERTY(EditAnywhere, Category = "Combat | Engage")
	float MinEngageDistance { 400.f };

	FTimerHandle DistanceTimer { FTimerHandle() };
	
	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> AISenseConfigSight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	/* Blackboard */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKeyName {FName("TargetActor")};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName ActionStateKeyName {FName("ActionState")};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName EngagedStateKeyName {FName("EngagedState")};

	/* State */
	EEnemyActionState ActionState { EEnemyActionState::EEAS_Patrolling };
	EEnemyEngagedState EngagedState { EEnemyEngagedState::EEAS_Strafe };

	/* Other */
	UPROPERTY()
	TObjectPtr<ABaseEnemy> ControlledPawn;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	/* Debug */
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawBoundaries { false };

public:
	/**
	 * Functions
	 */

	/* Combat */
	void Chase(AActor* Actor);
	bool IsStrafing();
	
	UFUNCTION()
	void TargetSpotted(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable)
	void Patrol();

	/* Blackboard */
	UFUNCTION(BlueprintCallable)
	void SetActionState(EEnemyActionState NewState);

	UFUNCTION(BlueprintCallable)
	void SetEngagedState(EEnemyEngagedState NewState);

	UFUNCTION(BlueprintCallable)
	float GetDistanceToTarget();

	/* Debug */
	UFUNCTION(BlueprintCallable)
	EEnemyActionState GetActionState();
	
	UFUNCTION(BlueprintCallable)
	EEnemyEngagedState GetEngagedState();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetTargetName() const { return TargetActor.GetFName(); }

};
