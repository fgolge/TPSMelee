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

	/* Animation */
	FVector CalculateLocationWithOffset();
	
	/**
	 * Variables
	 */

	/* Combat */
	UPROPERTY(EditAnywhere,	Category = "Combat")
	TSubclassOf<AWeapon> WeaponClass;

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

protected:
	/**
	 * Functions
	 */
	
	/* Combat */
	void SpawnWeapon(FName SocketName);

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

	/* Animation */
	void SetWarpTarget();
	void ClearWarpTarget();

};
