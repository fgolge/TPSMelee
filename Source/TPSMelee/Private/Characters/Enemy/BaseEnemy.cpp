// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Enemy");
}

void ABaseEnemy::SetEnemyActionState(EEnemyActionState NewState)
{
	EnemyActionState = NewState;
}

void ABaseEnemy::SetEnemySpeedState(EEnemySpeedState NewState)
{
	EnemySpeedState = NewState;

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