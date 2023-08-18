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

	/* Movement */
	bool ShouldSprint();
	void Run();
	
	/* Other */
	void SetupCamera();
	void AddMappingContext();

	/* Combat */
	void RotateActorForDodge();
	void ResetAttackIndex();
	void LockOnToTarget();
	void DisengageFromTarget();
	void EngageToTarget();
	void FindTargetToLockOn();
	void SetAttackIndex();
	FRotator GetLockOnRotation(float DeltaTime);
	bool ShouldLockOn();
	float CalculateCentricityCost(FHitResult& HitResult);
	float CalculateDistanceCost(FHitResult& HitResult);
	bool IsInFOV(float DotOfVector);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAttackIndex() const { return AttackIndex; }
	
	/**
	 * Variables
	 */

	/* Combat */
	int32 AttackIndex = 0;
	bool bCanAttack = true;
	FRotator LockedTargetRotation;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LockInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess, ClampMin = 0, ClampMax = 1))
	float CentricityWeight = .5f;
	
	float DistanceWeight;

	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxDistanceToTarget = 1000.f;
	
	UPROPERTY()
	TObjectPtr<AActor> LockedTarget;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float SphereTraceRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bShowDebugs = true;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceExtentMultiplier = 500.f;

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> Boom;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	/* Animation */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> EquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> UnequipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> Combo;

	/* Movement */
	bool bIsRunning;
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float EquippedSprintSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float EquippedRunSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AttackingWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float UnarmedSprintSpeed = 750.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float UnarmedRunSpeed = 500.f;

	/* Inventory */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName SheathSocketName;

protected:
	/**
	 * Functions
	 */

	/* Callbacks for Input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void Sprint();
	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;
	

	/* Animation */
	UFUNCTION(BlueprintCallable)
	virtual void SaveAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetCombo();

	UFUNCTION(BlueprintCallable)
	virtual void AttachWeaponToHand();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	virtual void FinishEquipping();
	
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	/* Combat */
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void Equip();
	virtual bool CanEquip();
	virtual void CancelAttack();
	virtual void Dodge();
	virtual void SetEngagedControls(bool Value);
	virtual void SetAttackingControls(bool Value);

	/* States */
	void SetActionState(EActionState State);
	void SetSpeedState(ESpeedState State);
	void SetWeaponState(EWeaponState State);
	bool IsUnoccupied();
	bool IsEquipped();
	bool IsRunning();
	bool IsSprinting();
	void SetCombatState(ECombatState State);
	bool IsEngaged();
	ESpeedState SpeedState;
	ESpeedState TempSpeedState;
	
	/**
	 * Variables
	 */

	/* Input Context and Actions */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> TargetLockOnAction;

	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<UInputAction> DodgeAction;

	/* States */
	EActionState ActionState = EActionState::EAS_Unoccupied;
	EWeaponState WeaponState = EWeaponState::EWS_Unequipped;
	ECombatState CombatState = ECombatState::ECS_Free;

	/* Combat */
	FInputActionValue MovementValue;

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

public:
	// Debug
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ESpeedState GetSpeedState() const {return SpeedState;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EActionState GetActionState() const {return ActionState;}

	/**
	 * Functions
	 */

	/* States */
	bool IsDodging();
	bool IsAttacking();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponState GetWeaponState() const {return WeaponState;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECombatState GetCombatState() const {return CombatState;}
	
};
