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
	
protected:
	/**
	 * Functions
	 */

	/* Combat */
	virtual void Die() override;

};
