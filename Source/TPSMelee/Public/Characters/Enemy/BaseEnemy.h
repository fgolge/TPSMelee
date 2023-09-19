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
	EEnemySpeedState SpeedState { EEnemySpeedState::EESS_PatrolSpeed };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float PatrolWalkSpeed { 150.f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InvestigateWalkSpeed { 175.f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ChaseWalkSpeed { 300.f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float EngagedWalkSpeed { 100.f };

	/* AI */
	FVector PatrolCenter;
	bool bIsAttacking { false };
	
protected:
	/**
	 * Functions
	 */

	/* Combat */
	virtual void Die() override;
	virtual void Destroyed() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

public:
	/**
	 * Functions
	 */

	/* States */
	UFUNCTION(BlueprintCallable)
	void SetEnemySpeedState(EEnemySpeedState NewState);

	FORCEINLINE FVector GetPatrolCenter() const { return PatrolCenter; }

	/**
	 * Variables
	 */

	/* AI */
	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool Value);
};
