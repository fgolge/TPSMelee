// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "ActorComponents/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/BoxComponent.h"
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
			Weapon->Equip(GetMesh(), SocketName, this, this, WeaponDamage);
		}
	}
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if(!IsAlive())
	{
		Die();
	}

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	// hit sound
	// hit particles
	// camera shake
}

void ABaseCharacter::Die()
{
	PlayMontage(DeathMontage);
	DisableMeshCollision();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ABaseCharacter::SetSpeed(const float Value)
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = Value;
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactCoplanar(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactCoplanar - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward,ToHit);
	if(CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromLeft");
	if(Theta >= -45.f && Theta <= 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::SetWarpTarget(float MaxWarpDistance)
{
	if(MotionWarpingComponent && TargetActor)
	{
		const FVector TargetLocation = CalculateLocationWithOffset(MaxWarpDistance);
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, TargetLocation);
	}
}

void ABaseCharacter::ClearWarpTarget()
{
	if(MotionWarpingComponent)
	{
		MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
	}
}

FVector ABaseCharacter::GetWarpTargetLocation()
{
	if(MotionWarpingComponent)
	{
		const FMotionWarpingTarget* WarpingTarget = MotionWarpingComponent->FindWarpTarget(WarpTargetName);
		if(WarpingTarget)
		{
			return WarpingTarget->Location;
		}

		return FVector::ZeroVector;
	}
	
	return FVector();
}

float ABaseCharacter::GetHealthPercent()
{
	if(Attributes)
	{
		return Attributes->GetHealthPercent();
	}

	return 0.f;
}

FVector ABaseCharacter::CalculateLocationWithOffset(float MaxWarpDistance)
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

		return GetActorLocation() + WarpDistance;
	}
	
	return FVector();
}

void ABaseCharacter::PlayHitReactMontage(const FName& Section)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(Section, HitReactMontage);
	}
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if(Weapon && Weapon->GetWeaponBox())
	{
		Weapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		Weapon->IgnoredActors.Empty();
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	return DamageAmount;
}

void ABaseCharacter::SetTargetActor(AActor* Actor)
{
	TargetActor = Actor;
}

void ABaseCharacter::ClearTargetActor()
{
	TargetActor = nullptr;
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if(Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::DisableMeshCollision()
{
	if(GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseCharacter::DisableCapsule()
{
	if(GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
