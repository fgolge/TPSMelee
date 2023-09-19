// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyStates.h"
#include "BaseEnemyAnimInstance.generated.h"

class ABaseEnemy;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class TPSMELEE_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

private:
	/**
	 * Variables
	 */

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	TObjectPtr<ABaseEnemy> BaseEnemy;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bIsDead;
	
	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	EEnemyActionState EnemyActionState;
};
