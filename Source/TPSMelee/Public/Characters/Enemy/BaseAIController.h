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
	void Chase(AActor* Actor);
	void Investigate(AActor* Actor, FAIStimulus Stimulus);
	bool ShouldChase(FAIStimulus Stimulus);
	bool ShouldInvestigate();
	bool IsHostile(AActor* Actor);
	IGenericTeamAgentInterface* IsTeamAgent(const APawn* OtherPawn);

	/* Debug */
	UFUNCTION(BlueprintCallable)
	UBlackboardComponent* GetEnemyBlackboard() const;
	
	/**
	 * Variables
	 */

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

	/* Other */
	UPROPERTY()
	TObjectPtr<ABaseEnemy> ControlledPawn;

	UPROPERTY()
	TObjectPtr<AActor> TargetEnemy;

	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	float DistanceToTarget = 999999.f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MaxDistanceToTarget = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackDistance = 200.f;

protected:
	/**
	 * Variables
	 */
	
	/* Blackboard Keys */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKeyName {FName("TargetActor")};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName InvestigationKeyName {FName("LastSeenLocation")};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName ActionStateKeyName {FName("ActionState")};
	
	/* Other */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	FVector LastSeenLocation;
	
public:
	/**
	 * Functions
	 */

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other);
	void SetBlackboardState(EEnemyActionState NewState);
	
	UFUNCTION()
	void PlayerSpotted(AActor* Actor, FAIStimulus Stimulus);
};
