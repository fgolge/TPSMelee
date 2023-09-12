// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyStates.h"
#include "Characters/BaseCharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TPSMELEE_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	
protected:
	virtual void BeginPlay() override;

private:
	/**
	 * Variables
	 */

	/* Combat */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float TimeToDestroyDeadActor { 8.f };

	/* States */
	EEnemyActionState ActionState { EEnemyActionState::EEAS_Patrolling };
	EEnemySpeedState SpeedState { EEnemySpeedState::EESS_PatrolSpeed };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float PatrolWalkSpeed { 150.f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InvestigateWalkSpeed { 175.f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ChaseWalkSpeed { 300.f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AttackWalkSpeed { 100.f };
	
	
protected:
	/**
	 * Functions
	 */

	/* Combat */
	virtual void Die() override;

public:
	/**
	 * Functions
	 */

	/* States */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EEnemyActionState GetEnemyActionState() const { return ActionState; }

	void SetEnemyActionState(EEnemyActionState NewState);

	UFUNCTION(BlueprintCallable)
	void SetEnemySpeedState(EEnemySpeedState NewState);

};
