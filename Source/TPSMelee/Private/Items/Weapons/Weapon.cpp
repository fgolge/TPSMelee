// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Interfaces/Interface_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	if(ItemMesh)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);
		ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	}
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsActorSameType(OtherActor)) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if(BoxHit.GetActor())
	{
		if(IsActorSameType(BoxHit.GetActor())) return;
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(),
			this, UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);
	}
}

void AWeapon::BoxTrace(FHitResult& HitResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for(AActor* Actor : IgnoredActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	ActorsToIgnore.AddUnique(GetOwner());

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceEnd->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowDebugs ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true);

	IgnoredActors.AddUnique(HitResult.GetActor());
}

bool AWeapon::IsActorSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult BoxHit)
{
	IInterface_Character* CharacterInterface = Cast<IInterface_Character>(BoxHit.GetActor());
	if(CharacterInterface)
	{
		CharacterInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}