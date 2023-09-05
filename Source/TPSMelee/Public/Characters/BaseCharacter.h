// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interface_Character.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;
class AWeapon;
class UMotionWarpingComponent;

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
	void DisableMeshCollision();
	void DisableCapsule();

	/* Animation */
	FVector CalculateLocationWithOffset();
	void PlayHitReactMontage(const FName& Section);
	
	/**
	 * Variables
	 */

	/* Combat */
	UPROPERTY(EditDefaultsOnly,	Category = "Combat")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DeathMontage;

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

protected:
	/**
	 * Functions
	 */
	
	/* Combat */
	void SpawnWeapon(FName SocketName);
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	void HandleDamage(float DamageAmount);
	bool IsAlive();
	virtual void Die();

	/* Animation */
	void PlayMontage(UAnimMontage* Montage);

	/**
	 * Variables
	 */

	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WarpTargetName;

	/* Combat */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	/* Animation */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxWarpDistance { 300.f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float WarpOffset { 75.f };

public:
	/**
	 * Functions
	 */

	/* Combat */
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/* Animation */
	void SetWarpTarget();
	void ClearWarpTarget();

};
