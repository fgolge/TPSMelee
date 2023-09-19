// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemy.h"

#include "Items/Weapons/Weapon.h"

ABaseEnemy::ABaseEnemy()
{
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	PatrolCenter = GetActorLocation();
	
	Tags.Add("Enemy");
	SpawnWeapon(WeaponSocketName);
}

void ABaseEnemy::Die()
{
	Super::Die();

	SetLifeSpan(TimeToDestroyDeadActor);
}

void ABaseEnemy::Destroyed()
{
	Super::Destroyed();

	if(Weapon)
	{
		Weapon->Destroy();
	}
}

void ABaseEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if(IsAlive() && !bIsAttacking && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}

	Super::GetHit_Implementation(ImpactPoint, Hitter);
}

void ABaseEnemy::SetEnemySpeedState(EEnemySpeedState NewState)
{
	SpeedState = NewState;

	switch (NewState)
	{
	case EEnemySpeedState::EESS_PatrolSpeed:
		SetSpeed(PatrolWalkSpeed);
		break;
	case EEnemySpeedState::EESS_InvestigateSpeed:
		SetSpeed(InvestigateWalkSpeed);
		break;
	case EEnemySpeedState::EESS_ChaseSpeed:
		SetSpeed(ChaseWalkSpeed);
		break;
	case EEnemySpeedState::EESS_EngagedSpeed:
		SetSpeed(EngagedWalkSpeed);
		break;
	default:
		SetSpeed(PatrolWalkSpeed);
		break;
	}
}

bool ABaseEnemy::GetIsAttacking()
{
	return bIsAttacking;
}

void ABaseEnemy::SetIsAttacking(bool Value)
{
	bIsAttacking = Value;
}
