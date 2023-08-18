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

	/* Movement */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float PatrolWalkSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InvestigateWalkSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ChaseWalkSpeed = 450.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AttackWalkSpeed = 250.f;
	

protected:
	/**
	 * Variables
	 */
	
	/* States */
	EEnemyActionState EnemyActionState = EEnemyActionState::EEAS_NoState;
	EEnemySpeedState EnemySpeedState = EEnemySpeedState::EESS_PatrolSpeed;

public:
	FORCEINLINE EEnemyActionState GetEnemyActionState() const { return EnemyActionState; }
	void SetEnemyActionState(EEnemyActionState NewState);
	void SetEnemySpeedState(EEnemySpeedState NewState);
};
