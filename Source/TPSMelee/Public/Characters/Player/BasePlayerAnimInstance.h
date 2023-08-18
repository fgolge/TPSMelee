// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerStates.h"
#include "BasePlayerAnimInstance.generated.h"

class ABasePlayer;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class TPSMELEE_API UBasePlayerAnimInstance : public UAnimInstance
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
	TObjectPtr<ABasePlayer> BasePlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterMovementComponent> PlayerMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bDidJump;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bIsAccelerating;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bIsFullBody;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	EWeaponState WeaponState;
	
	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	ECombatState CombatState;
};
