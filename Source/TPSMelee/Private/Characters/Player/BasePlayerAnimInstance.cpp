// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/BasePlayerAnimInstance.h"
#include "Characters/Player/BasePlayer.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBasePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BasePlayer = Cast<ABasePlayer>(TryGetPawnOwner());
	if(BasePlayer)
	{
		PlayerMovementComponent = BasePlayer->GetCharacterMovement();
	}
}

void UBasePlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if(BasePlayer && PlayerMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovementComponent->Velocity);
		Direction = UKismetAnimationLibrary::CalculateDirection(PlayerMovementComponent->Velocity, BasePlayer->GetActorRotation());
		bIsFalling = PlayerMovementComponent->IsFalling();
		bDidJump = PlayerMovementComponent->Velocity.Z > 0;
		bIsAccelerating = PlayerMovementComponent->GetCurrentAcceleration().Length() > 0;
		WeaponState = BasePlayer->GetWeaponState();
		CombatState = BasePlayer->GetCombatState();
		bIsFullBody = BasePlayer->IsDodging() || BasePlayer->IsAttacking();
	}
}
