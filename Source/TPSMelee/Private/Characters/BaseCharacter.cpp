// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "ActorComponents/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void ABaseCharacter::SpawnWeapon()
{
	UWorld* World = GetWorld();
	if(World)
	{
		Weapon = World->SpawnActor<AWeapon>(WeaponClass);
		if(Weapon)
		{
			Weapon->Equip(GetMesh(), WeaponSocketName, this, this);
		}
	}
}

void ABaseCharacter::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if(IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else
	{
		Die();
	}

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::DirectionalHitReact(const FVector& HitterLocation)
{
	const FVector Forward=GetActorForwardVector();
	
	const FVector ImpactLowered(HitterLocation.X,HitterLocation.Y,GetActorLocation().Z);
	const FVector ToHit=(ImpactLowered-GetActorLocation()).GetSafeNormal();

	const double CosTheta=FVector::DotProduct(Forward,ToHit);
	double Theta=FMath::Acos(CosTheta);
	Theta=FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct=FVector::CrossProduct(Forward,ToHit);
	if(CrossProduct.Z<0)
	{
		Theta*=-1.f;
	}

	FName Section("FromBack");

	if(Theta >= -45.f && Theta < 45.f)
	{
		Section=FName("FromFront");
	}
	else if(Theta >= -135.f && Theta <- 45.f)
	{
		Section=FName("FromLeft");
	}
	else if(Theta >= 45.f && Theta < 135.f)
	{
		Section=FName("FromRight");
	}

	//PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
}

void ABaseCharacter::SetSpeed(const float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if(Weapon && Weapon->GetWeaponBox())
	{
		Weapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		Weapon->IgnoredActors.Empty();
	}
}

bool ABaseCharacter::CanAttack()
{
	return true;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::Attack()
{
	if(AttackMontage)
	{
		PlayMontage(AttackMontage);
	}
}