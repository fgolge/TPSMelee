// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemyAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/Enemy/BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseEnemy = Cast<ABaseEnemy>(TryGetPawnOwner());
	
	if(BaseEnemy)
	{
		CharacterMovementComponent = BaseEnemy->GetCharacterMovement();
	}
}

void UBaseEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if(BaseEnemy && CharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);
		Direction = UKismetAnimationLibrary::CalculateDirection(CharacterMovementComponent->Velocity, BaseEnemy->GetActorRotation());
		// EnemyActionState = BaseEnemy->GetEnemyActionState();
	}
}