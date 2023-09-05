// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/BasePlayer.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABasePlayer::ABasePlayer()
{
	SetupCamera();
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon(SheathSocketName);
	AddMappingContext();
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	EngagedCameraSettings(false);
	DistanceWeight = 1 - CentricityWeight;
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TargetActor && Controller && ShouldLockOn())
	{
		RotateCameraToTarget(DeltaTime);
	}
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayer::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ABasePlayer::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ABasePlayer::ReachForWeapon);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ABasePlayer::Dodge);
		EnhancedInputComponent->BindAction(TargetLockOnAction, ETriggerEvent::Started, this, &ABasePlayer::LockOnToTarget);
	}
}

void ABasePlayer::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

	MovementValue = Value;
}

void ABasePlayer::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ABasePlayer::Jump()
{
	if(IsUnoccupied())
	{
		Super::Jump();
	}
}

void ABasePlayer::Falling()
{
	Super::Falling();

	StopAnimMontage();
	ResetAttackIndex();
	SetActionState(EActionState::EAS_Falling);
}

void ABasePlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::LockOnToTarget()
{
	if(!IsEquipped()) return;

	if(IsEngaged())
	{
		DisengageFromTarget();
		TargetActor = nullptr;
	}
	else
	{
		FindTargetToLockOn();

		if(TargetActor)
		{
			EngageToTarget();
		}
	}
}

/* Starts equipping action */
void ABasePlayer::ReachForWeapon()
{
	if(!CanEquip()) return;

	SetActionState(EActionState::EAS_Equipping);
	bIsUpperBody = true;

	if(!IsEquipped())
	{
		if(EquipMontage)
		{
			PlayMontage(EquipMontage);
		}
	}
	else
	{
		if(UnequipMontage)
		{
			PlayMontage(UnequipMontage);
		}
	}
}

/* Once the character reaches the weapon, it sets states and attaches mesh to the socket */
void ABasePlayer::EquipWeapon()
{
	if(!IsEquipped())
	{
		SetWeaponState(EWeaponState::EWS_Equipped);
		AttachWeaponToSocket(WeaponSocketName);

		if(IsEngaged()) EngagedCameraSettings(false);
		SetCombatState(ECombatState::ECS_Free);
		TargetActor = nullptr;
	}
	else
	{
		SetWeaponState(EWeaponState::EWS_Unequipped);
		AttachWeaponToSocket(SheathSocketName);

		if(IsEngaged()) EngagedCameraSettings(true);
	}
}

void ABasePlayer::FinishEquipping()
{
	bIsUpperBody = false;
	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::SaveAttack()
{
	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::ResetCombo()
{
	ResetAttackIndex();
	bIsFullBody = false;
}

void ABasePlayer::DodgeEnd()
{
	if(IsEngaged()) bUseControllerRotationYaw = true;
	
	SetActionState(EActionState::EAS_Unoccupied);
	bIsFullBody = false;
}

void ABasePlayer::Attack()
{
	if(!CanAttack()) return;
	
	if(ComboMontages[AttackIndex])
	{
		//SetActorRotation(FRotator(0.f, GetController()->GetControlRotation().Yaw, 0.f));
		SetWarpTarget();
		bIsFullBody = true;
		SetActionState(EActionState::EAS_Attacking);
		PlayMontage(ComboMontages[AttackIndex]);
		SetAttackIndex();
	}
}

void ABasePlayer::Dodge()
{
	if(!CanDodge()) return;

	if(IsEngaged()) bUseControllerRotationYaw = false;
	
	RotateActorForDodge();
	ResetAttackIndex();
	bIsFullBody = true;
	SetActionState(EActionState::EAS_Dodge);
	if(DodgeMontage) PlayMontage(DodgeMontage);
}

void ABasePlayer::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
}

void ABasePlayer::SetCombatState(ECombatState State)
{
	CombatState = State;
	
	switch(State)
	{
	case ECombatState::ECS_Engaged:
		GetCharacterMovement()->MaxWalkSpeed = EngagedWalkSpeed;
		break;
	case ECombatState::ECS_Free:
		GetCharacterMovement()->MaxWalkSpeed = FreeWalkSpeed;
		break;
	default:
			GetCharacterMovement()->MaxWalkSpeed = FreeWalkSpeed;
		break;
	}
}

void ABasePlayer::SetActionState(EActionState State)
{
	ActionState = State;
}

bool ABasePlayer::IsEquipped()
{
	return WeaponState == EWeaponState::EWS_Equipped;
}

bool ABasePlayer::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ABasePlayer::IsEngaged()
{
	return CombatState == ECombatState::ECS_Engaged;
}

void ABasePlayer::SetAttackIndex()
{
	if(AttackIndex < 2)
	{
		AttackIndex++;
	}
	else if(AttackIndex >= 2)
	{
		AttackIndex = 0;
	}
}

void ABasePlayer::ResetAttackIndex()
{
	AttackIndex = 0;
}

void ABasePlayer::AttachWeaponToSocket(FName SocketName)
{
	if(Weapon)
	{
		Weapon->AttachMeshToSocket(GetMesh(), SocketName);
	}
}

bool ABasePlayer::CanEquip()
{
	return Weapon && (IsUnoccupied());
}

bool ABasePlayer::CanAttack()
{
	return IsEquipped() && (IsUnoccupied());
}

bool ABasePlayer::CanDodge()
{
	return IsUnoccupied();
}

void ABasePlayer::RotateActorForDodge()
{
	if(Controller)
	{
		FVector DesiredDirection = FVector(MovementValue.Get<FVector>().Y, MovementValue.Get<FVector>().X, 0);
		FRotator ControllerRotation = Controller->GetControlRotation();
		FRotator DesiredRotation = FRotator(0, DesiredDirection.Rotation().Yaw + ControllerRotation.Yaw, 0);
		SetActorRotation(DesiredRotation);
	}
}

void ABasePlayer::EngagedCameraSettings(bool Value)
{
	GetCharacterMovement()->bOrientRotationToMovement = !Value;
	GetCharacterMovement()->bUseControllerDesiredRotation = Value;
	bUseControllerRotationYaw = Value;
}

void ABasePlayer::FindTargetToLockOn()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);

	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation() + (Camera->GetForwardVector().GetSafeNormal2D() * TraceExtentMultiplier);

	UKismetSystemLibrary::SphereTraceMulti(
		this,
		Start,
		End,
		TargetSearchRadius,
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowTargetSearchDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		true);

	float TotalLockOnCost = 0.f;

	if(HitResults.Num() > 0)
	{
		for(FHitResult OutHit : HitResults)
		{
			if(OutHit.GetActor() && OutHit.GetActor()->ActorHasTag(TEXT("Enemy")))
			{
				float Centricity = CalculateCentricityCost(OutHit) * CentricityWeight;
				float Distance = CalculateDistanceCost(OutHit) * DistanceWeight;
				if(Centricity + Distance > TotalLockOnCost && IsInFOV(CalculateCentricityCost(OutHit)))
				{
					TotalLockOnCost = Centricity + Distance;
					TargetActor = OutHit.GetActor();
				}
			}
		}
	}
}

void ABasePlayer::EngageToTarget()
{
	SetCombatState(ECombatState::ECS_Engaged);
	if(IsEquipped()) EngagedCameraSettings(true);
}

void ABasePlayer::DisengageFromTarget()
{
	SetCombatState(ECombatState::ECS_Free);
	ClearWarpTarget();
	EngagedCameraSettings(false);
}

float ABasePlayer::CalculateCentricityCost(FHitResult& HitResult)
{
	const FVector CamToTargetVector = UKismetMathLibrary::GetDirectionUnitVector(CameraManager->GetCameraLocation(), HitResult.GetActor()->GetActorLocation());
	const float DotOfVector = FVector::DotProduct(CamToTargetVector, CameraManager->GetActorForwardVector());
	return DotOfVector;
}

float ABasePlayer::CalculateDistanceCost(FHitResult& HitResult)
{
	float Distance = FVector::Distance(GetActorLocation(), HitResult.GetActor()->GetActorLocation());
	return 1 - (Distance / MaxDistanceToTarget);
}

bool ABasePlayer::IsInFOV(float DotOfVector)
{
	const float AngleOfVector = UKismetMathLibrary::DegAcos(DotOfVector);
	const float FovBounds = (CameraManager->GetFOVAngle() * 0.5f) - 10.f;
	return UKismetMathLibrary::InRange_FloatFloat(AngleOfVector, 0.f, FovBounds);
}

bool ABasePlayer::ShouldLockOn()
{
	if(TargetActor)
	{
		float DistanceToTarget = FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation());

		if(DistanceToTarget < MaxDistanceToTarget)
		{
			return true;
		}

		DisengageFromTarget();
		TargetActor = nullptr;
		return false;
	}

	return false;
}

void ABasePlayer::RotateCameraToTarget(float DeltaTime)
{
	const FRotator ControlRot = GetController()->GetControlRotation();
	const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(CameraManager->GetCameraLocation(), TargetActor->GetActorLocation());
	const FRotator InterpRot = FMath::RInterpTo(ControlRot, LookAtRot, DeltaTime, LockInterpSpeed);
	const FRotator NewControlRot = FRotator(ControlRot.Pitch, InterpRot.Yaw, ControlRot.Roll);

	Controller->SetControlRotation(NewControlRot);
}

void ABasePlayer::SetupCamera()
{
	Boom=CreateDefaultSubobject<USpringArmComponent>(TEXT("Boom"));
	Boom->SetupAttachment(GetRootComponent());
	Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Boom);
	Boom->bUsePawnControlRotation = true;
}

void ABasePlayer::AddMappingContext()
{
	if(const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

bool ABasePlayer::IsFullBody()
{
	return bIsFullBody;
}

bool ABasePlayer::IsUpperBody()
{
	return bIsUpperBody;
}

bool ABasePlayer::IsAttacking() const
{
	return ActionState == EActionState::EAS_Attacking;
}

bool ABasePlayer::IsDodging() const
{
	return ActionState == EActionState::EAS_Dodge;
}

bool ABasePlayer::IsEquipping() const
{
	return ActionState == EActionState::EAS_Equipping;
}
