// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interface_Character.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;
class AWeapon;

UCLASS()
class TPSMELEE_API ABaseCharacter : public ACharacter, public IInterface_Character
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * Functions
	 */

	/* Combat */
	void DirectionalHitReact(const FVector& HitterLocation);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	
protected:
	/**
	 * Functions
	 */

	/* Combat */
	virtual bool CanAttack();
	bool IsAlive();
	virtual void GetHit_Implementation(const FVector& ImpactPoint ,AActor* Hitter) override;
	void Die();

	UFUNCTION(BlueprintCallable)
	virtual void Attack();
	
	/* Inventory */
	void SpawnWeapon();

	/* Animation */
	virtual void PlayMontage(UAnimMontage* Montage);

	/* Movement */
	UFUNCTION(BlueprintCallable)
	void SetSpeed(const float Value);
	
	/**
	 * Variables
	 */

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeComponent> Attributes;
	
	/* Combat */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> Weapon;

	/* Inventory */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponSocketName;

	/* Montages */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

public:
	/**
	 * Functions
	 */

	/* Anim Notifies */
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

};
