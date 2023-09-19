// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSMELEE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;
	
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;
	
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate;

public:
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetStaminaPercent();
	void RegenStamina(float DeltaTime);
	bool IsAlive();
	float GetHealthPercent();
};
