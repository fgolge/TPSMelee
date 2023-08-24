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
	 * Variables
	 */

	/* Combat */
	UPROPERTY(EditAnywhere,	Category = "Combat")
	TSubclassOf<AWeapon> WeaponClass;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeComponent> Attributes;

	/* Combat */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> Weapon;

	/* Animation */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName;

};
