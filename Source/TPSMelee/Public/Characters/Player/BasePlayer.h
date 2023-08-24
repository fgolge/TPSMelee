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

	/* Combat */
	void ReachForWeapon();
	void Attack();
	void SetAttackIndex();
	void ResetAttackIndex();
	bool CanEquip();
	bool CanAttack();
	
	/* Animation */
	void PlayMontage(UAnimMontage* Montage);
	void AttachWeaponToSocket(FName SocketName);
	virtual void EquipWeapon_Implementation() override;
	virtual void FinishEquipping_Implementation() override;
	virtual void SaveAttack_Implementation() override;
	virtual void ResetCombo_Implementation() override;

	/* States */
	void SetWeaponState(EWeaponState State);
	void SetCombatState(ECombatState State);
	void SetActionState(EActionState State);
	void SetSpeedState(ESpeedState State);
	bool IsEquipped();
	bool IsUnoccupied();
	bool IsBlocking();
	
	/* Other */
	void SetupCamera();
	void AddMappingContext();

	/**
	 * Variables
	 */

	/* Combat */
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

	FInputActionValue MovementValue;

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> Boom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	/* States */
	EWeaponState WeaponState { EWeaponState::EWS_Unequipped };
	ECombatState CombatState { ECombatState::ECS_Free };
	EActionState ActionState { EActionState::EAS_Unoccupied };
	ESpeedState SpeedState { ESpeedState::ESS_Running };

	/* Animation */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> ComboMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> UnequipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName SheathSocketName;

	bool bIsFullBody { false };
	bool bIsUpperBody { false };

public:
	/**
	 * Functions
	 */

	/* States */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponState GetWeaponState() const { return WeaponState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ESpeedState GetSpeedState() const { return SpeedState; }

	bool IsFullBody();
	bool IsUpperBody();
	bool IsAttacking() const;
	bool IsDodging() const;
	bool IsEquipping() const;

};
