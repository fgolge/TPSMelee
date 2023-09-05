// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PlayerStates.h"
#include "Characters/BaseCharacter.h"
#include "BasePlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class TPSMELEE_API ABasePlayer : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABasePlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * Functions
	 */

	/* Input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;
	void LockOnToTarget();

	/* Combat */
	void ReachForWeapon();
	void Attack();
	void Dodge();
	void SetAttackIndex();
	void ResetAttackIndex();
	bool CanEquip();
	bool CanAttack();
	bool CanDodge();
	void RotateActorForDodge();
	void EngagedCameraSettings(bool Value);
	void FindTargetToLockOn();
	void EngageToTarget();
	void DisengageFromTarget();
	float CalculateCentricityCost(FHitResult& HitResult);
	float CalculateDistanceCost(FHitResult& HitResult);
	bool IsInFOV(float DotOfVector);
	bool ShouldLockOn();
	void RotateCameraToTarget(float DeltaTime);
	
	/* Animation */
	void AttachWeaponToSocket(FName SocketName);

	/* States */
	void SetWeaponState(EWeaponState State);
	void SetCombatState(ECombatState State);
	void SetActionState(EActionState State);
	bool IsEquipped();
	bool IsUnoccupied();
	
	/* Other */
	void SetupCamera();
	void AddMappingContext();

	/**
	 * Variables
	 */

	/* Combat */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float LockInterpSpeed { 5.f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxDistanceToTarget { 1000.f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceExtentMultiplier { 500.f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float TargetSearchRadius { 100.f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bShowTargetSearchDebug { true };

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess, ClampMin = 0, ClampMax = 1))
	float CentricityWeight { .5f };

	float DistanceWeight;
	int32 AttackIndex { 0 };
	
	/* Input */
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TargetLockOnAction;
	
	FInputActionValue MovementValue;

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> Boom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;

	/* States */
	EWeaponState WeaponState { EWeaponState::EWS_Unequipped };
	ECombatState CombatState { ECombatState::ECS_Free };
	EActionState ActionState { EActionState::EAS_Unoccupied };

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
	float EngagedWalkSpeed { 400.f };
	
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
	float FreeWalkSpeed { 600.f };
	
	/* Animation */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> ComboMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> UnequipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName SheathSocketName;

	bool bIsFullBody { false };
	bool bIsUpperBody { false };

public:
	/**
	 * Functions
	 */

	/* Combat */
	void EquipWeapon();
	void FinishEquipping();
	void SaveAttack();
	void ResetCombo();
	void DodgeEnd();

	/* States */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponState GetWeaponState() const { return WeaponState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	bool IsFullBody();
	bool IsUpperBody();
	bool IsAttacking() const;
	bool IsDodging() const;
	bool IsEquipping() const;
	bool IsEngaged();

};
