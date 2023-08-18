// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/AttributeComponent.h"

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + (StaminaRegenRate * DeltaTime), 0.f, MaxStamina);
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}
