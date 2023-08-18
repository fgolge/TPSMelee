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
	PrimaryActorTick.bCanEverTick = true;
	
	SetupCamera();

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	AddMappingContext();
	SetSpeedState(ESpeedState::ESS_Running);

	if(Weapon)
	{
		Weapon->AttachMeshToSocket(GetMesh(), SheathSocketName);
	}

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	SetEngagedControls(false);
	DistanceWeight = 1 - CentricityWeight;
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(LockedTarget && PlayerController && ShouldLockOn())
	{
		const FRotator ControlRot = GetController()->GetControlRotation();
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(CameraManager->GetCameraLocation(), LockedTarget->GetActorLocation());
		const FRotator InterpRot = FMath::RInterpTo(ControlRot, LookAtRot, DeltaTime, LockInterpSpeed);
		const FRotator NewPlayerRot = FRotator(ControlRot.Pitch, InterpRot.Yaw, ControlRot.Roll);

		PlayerController->SetControlRotation(NewPlayerRot);
	}
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABasePlayer::Jump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABasePlayer::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayer::Run);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ABasePlayer::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ABasePlayer::Equip);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ABasePlayer::Dodge);
		EnhancedInputComponent->BindAction(TargetLockOnAction, ETriggerEvent::Triggered, this, &ABasePlayer::LockOnToTarget);
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

		CancelAttack();
	}
}

void ABasePlayer::Sprint()
{
	if(IsAttacking()) TempSpeedState = ESpeedState::ESS_Sprinting;
	else if(ShouldSprint()) SetSpeedState(ESpeedState::ESS_Sprinting);
}

void ABasePlayer::Falling()
{
	Super::Falling();

	SetActionState(EActionState::EAS_Falling);
}

void ABasePlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::SaveAttack()
{
	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::ResetCombo()
{
	SetActionState(EActionState::EAS_Unoccupied);
	if(SpeedState <= TempSpeedState) SetSpeedState(TempSpeedState);
	ResetAttackIndex();
}

void ABasePlayer::AttachWeaponToHand()
{
	if(Weapon)
	{
		Weapon->AttachMeshToSocket(GetMesh(), WeaponSocketName);
	}
}

void ABasePlayer::AttachWeaponToBack()
{
	if(Weapon)
	{
		Weapon->AttachMeshToSocket(GetMesh(), SheathSocketName);
	}
}

void ABasePlayer::FinishEquipping()
{
	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::DodgeEnd()
{
	if(IsEngaged())
	{
		bUseControllerRotationYaw = true;
	}
	
	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::Attack()
{
	if(!IsEquipped()) return;
	
	if(CanAttack() && Combo[AttackIndex])
	{
		//if(!IsEngaged()) SetEngagedControls(true);
		SetActionState(EActionState::EAS_Attacking);
		if(SpeedState != ESpeedState::ESS_Attacking) TempSpeedState = SpeedState;
		SetSpeedState(ESpeedState::ESS_Attacking);
		AttackMontage = Combo[AttackIndex];
		if(!IsEquipped()) return;
		Super::Attack();
		SetAttackIndex();
	}
}

void ABasePlayer::Equip()
{
	if(!CanEquip()) return;

	CancelAttack();
	SetActionState(EActionState::EAS_Equipping);
	
	if(!IsEquipped())
	{
		SetWeaponState(EWeaponState::EWS_Equipped);
		if(IsEngaged()) SetEngagedControls(false);
		SetCombatState(ECombatState::ECS_Free);
		LockedTarget = nullptr;
		if(EquipMontage)
		{
			PlayMontage(EquipMontage);

			if(IsSprinting()) SetSpeedState(ESpeedState::ESS_Sprinting);
			if(IsRunning()) SetSpeedState(ESpeedState::ESS_Running);
		}
	}
	else
	{
		SetWeaponState(EWeaponState::EWS_Unequipped);
		if(IsEngaged()) SetEngagedControls(true);
		if(UnequipMontage)
		{
			PlayMontage(UnequipMontage);
			
			if(IsSprinting()) SetSpeedState(ESpeedState::ESS_Sprinting);
			else if(IsRunning()) SetSpeedState(ESpeedState::ESS_Running);
		}
	}
}

bool ABasePlayer::CanEquip()
{
	return IsUnoccupied();
}

void ABasePlayer::CancelAttack()
{
	ResetAttackIndex();
}

void ABasePlayer::Dodge()
{
	if(IsUnoccupied() && IsEngaged())
	{
		bUseControllerRotationYaw = false;
	}
	if(!IsUnoccupied()) return;
	RotateActorForDodge();
	CancelAttack();
	SetActionState(EActionState::EAS_Dodge);
	SetSpeedState(ESpeedState::ESS_Running);
	if(DodgeMontage)
	{
		PlayMontage(DodgeMontage);
	}
}

void ABasePlayer::SetEngagedControls(bool Value)
{
	GetCharacterMovement()->bOrientRotationToMovement = !Value;
	GetCharacterMovement()->bUseControllerDesiredRotation = Value;
	bUseControllerRotationYaw = Value;
}

void ABasePlayer::SetAttackingControls(bool Value)
{
	GetCharacterMovement()->bOrientRotationToMovement = !Value;
	GetCharacterMovement()->bUseControllerDesiredRotation = Value;
	bUseControllerRotationYaw = Value;
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

void ABasePlayer::SetActionState(EActionState State)
{
	ActionState = State;
}

void ABasePlayer::SetSpeedState(ESpeedState State)
{
	SpeedState = State;
	
	if(IsEquipped())
	{
		switch(State)
		{
		case ESpeedState::ESS_Running:
			SetSpeed(EquippedRunSpeed);
			break;
		case ESpeedState::ESS_Sprinting:
			SetSpeed(EquippedSprintSpeed);
			break;
		case ESpeedState::ESS_Attacking:
			SetSpeed(AttackingWalkSpeed);
			break;
		default:
			SetSpeed(EquippedRunSpeed);
			break;
		}
	}
	else
	{
		switch(State)
		{
		case ESpeedState::ESS_Running:
			SetSpeed(UnarmedRunSpeed);
			break;
		case ESpeedState::ESS_Sprinting:
			SetSpeed(UnarmedSprintSpeed);
			break;
		default:
			SetSpeed(UnarmedRunSpeed);
			break;
		}
	}
	
}

void ABasePlayer::LockOnToTarget()
{
	if(!IsEquipped()) return;
	
	if(IsEngaged())
	{
		DisengageFromTarget();
		LockedTarget = nullptr;
	}
	else
	{
		FindTargetToLockOn();

		if(LockedTarget)
		{
			EngageToTarget();
		}
	}
}

void ABasePlayer::DisengageFromTarget()
{
	SetCombatState(ECombatState::ECS_Free);
	SetEngagedControls(false);
}

void ABasePlayer::EngageToTarget()
{
	SetCombatState(ECombatState::ECS_Engaged);
	if(IsEquipped()) SetEngagedControls(true);
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
		SphereTraceRadius,
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowDebugs ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
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
					LockedTarget = OutHit.GetActor();
				}
					
				/* Debug */
				/*
				const FString CostString = FString::SanitizeFloat(Centricity + Distance);
				const FString DotString = FString::SanitizeFloat(CalculateCentricityCost(OutHit));
				UKismetSystemLibrary::DrawDebugString(this, FVector(0.f, 0.f, -50.f), CostString, OutHit.GetActor(), FLinearColor::White, 5.f);
				UKismetSystemLibrary::DrawDebugString(this, FVector(0.f, 0.f, 50.f), DotString, OutHit.GetActor(), FLinearColor::White, 5.f);
				*/
			}
		}
	}
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

FRotator ABasePlayer::GetLockOnRotation(float DeltaTime)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), LockedTarget->GetActorLocation());
	LockedTargetRotation = FRotator(LookAtRotation.Pitch, LookAtRotation.Yaw, 0.f);
	return FMath::RInterpTo(Camera->GetComponentRotation(), LockedTargetRotation, DeltaTime, 30.f);
}

bool ABasePlayer::ShouldLockOn()
{
	if(LockedTarget)
	{
		float DistanceToTarget = FVector::Distance(GetActorLocation(), LockedTarget->GetActorLocation());

		if(DistanceToTarget < MaxDistanceToTarget)
		{
			return true;
		}

		DisengageFromTarget();
		LockedTarget = nullptr;
		return false;
	}

	return false;
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
	return 1 - (Distance/MaxDistanceToTarget);
}

bool ABasePlayer::IsInFOV(float DotOfVector)
{
	const float AngleOfVector = UKismetMathLibrary::DegAcos(DotOfVector);
	const float FovBounds = (CameraManager->GetFOVAngle() * 0.5f) - 10.f;
	return UKismetMathLibrary::InRange_FloatFloat(AngleOfVector, 0.f, FovBounds);
}

void ABasePlayer::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
}

bool ABasePlayer::CanAttack()
{
	return IsUnoccupied();
}

bool ABasePlayer::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ABasePlayer::IsEquipped()
{
	return WeaponState == EWeaponState::EWS_Equipped;
}

bool ABasePlayer::IsRunning()
{
	return SpeedState == ESpeedState::ESS_Running;
}

bool ABasePlayer::IsSprinting()
{
	return SpeedState == ESpeedState::ESS_Sprinting;
}

void ABasePlayer::SetCombatState(ECombatState State)
{
	CombatState = State;
}

bool ABasePlayer::IsEngaged()
{
	return CombatState == ECombatState::ECS_Engaged;
}

bool ABasePlayer::IsAttacking()
{
	return ActionState == EActionState::EAS_Attacking;
}

bool ABasePlayer::IsDodging()
{
	return ActionState == EActionState::EAS_Dodge;
}

bool ABasePlayer::ShouldSprint()
{
	if(IsEquipped())
	{
		return GetCharacterMovement()->MaxWalkSpeed < EquippedSprintSpeed;
	}

	return GetCharacterMovement()->MaxWalkSpeed < UnarmedSprintSpeed;
}

void ABasePlayer::Run()
{
	if(IsAttacking()) TempSpeedState = ESpeedState::ESS_Running;
	else SetSpeedState(ESpeedState::ESS_Running);
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
	PlayerController = Cast<APlayerController>(Controller);
	
	if(PlayerController)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ABasePlayer::ResetAttackIndex()
{
	AttackIndex = 0;
}
