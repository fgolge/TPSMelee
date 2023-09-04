// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "ActorComponents/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::SpawnWeapon(FName SocketName)
{
	UWorld* World = GetWorld();
	if(World)
	{
		Weapon = World->SpawnActor<AWeapon>(WeaponClass);
		if(Weapon)
		{
			Weapon->Equip(GetMesh(), SocketName, this, this);
		}
	}
}

void ABaseCharacter::SetWarpTarget()
{
	if(MotionWarpingComponent && TargetActor)
	{
		const FVector TargetLocation = CalculateLocationWithOffset();
		const FTransform NewTransform = FTransform(GetActorRotation(), TargetLocation, GetActorScale());
		
		//MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(WarpTargetName, NewTransform);
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, TargetLocation);
		
		float DistToTarget = (TargetLocation - GetActorLocation()).Length();
		UE_LOG(LogTemp, Warning, TEXT("%f"), DistToTarget);
	}
}

void ABaseCharacter::ClearWarpTarget()
{
	if(MotionWarpingComponent)
	{
		MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
	}
}

FVector ABaseCharacter::CalculateLocationWithOffset()
{
	if(TargetActor)
	{
		const FVector Distance = TargetActor->GetActorLocation() - GetActorLocation();
		const FVector Offset = Distance.GetSafeNormal() * WarpOffset;
		FVector WarpDistance = Distance - Offset;
		
		if(WarpDistance.Length() > MaxWarpDistance)
		{
			WarpDistance = Distance.GetSafeNormal() * MaxWarpDistance;
		}

		UE_LOG(LogTemp, Warning, TEXT("Offset Length: %f"), Offset.Length());
		
		return GetActorLocation() + WarpDistance;
	}
	
	return FVector();
}