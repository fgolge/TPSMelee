// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Enemy");
	SpawnWeapon(WeaponSocketName);
}

void ABaseEnemy::Die()
{
	Super::Die();

	SetLifeSpan(TimeToDestroyDeadActor);
}

void ABaseEnemy::SetEnemyActionState(EEnemyActionState NewState)
{
	ActionState = NewState;
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
	case EEnemySpeedState::EESS_AttackSpeed:
		SetSpeed(AttackWalkSpeed);
		break;
	default:
		SetSpeed(PatrolWalkSpeed);
		break;
	}
}
