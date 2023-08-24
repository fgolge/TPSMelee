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
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABasePlayer::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ABasePlayer::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ABasePlayer::ReachForWeapon);
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
	if(IsUnoccupied() || IsBlocking())
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

/* Once the character reaches the weapon, it sets states and attaches mesh to a socket */
void ABasePlayer::EquipWeapon_Implementation()
{
	if(!IsEquipped())
	{
		SetWeaponState(EWeaponState::EWS_Equipped);
		AttachWeaponToSocket(WeaponSocketName);
	}
	else
	{
		SetWeaponState(EWeaponState::EWS_Unequipped);
		AttachWeaponToSocket(SheathSocketName);
	}
}

void ABasePlayer::FinishEquipping_Implementation()
{
	bIsUpperBody = false;
	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::SaveAttack_Implementation()
{
	SetActionState(EActionState::EAS_Unoccupied);
}

void ABasePlayer::ResetCombo_Implementation()
{
	ResetAttackIndex();
	bIsFullBody = false;
}

void ABasePlayer::Attack()
{
	if(!CanAttack()) return;
	
	if(ComboMontages[AttackIndex])
	{
		bIsFullBody = true;
		SetActionState(EActionState::EAS_Attacking);
		PlayMontage(ComboMontages[AttackIndex]);
		SetAttackIndex();
	}
}

void ABasePlayer::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ABasePlayer::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
}

void ABasePlayer::SetCombatState(ECombatState State)
{
	CombatState = State;
}

void ABasePlayer::SetActionState(EActionState State)
{
	ActionState = State;
}

void ABasePlayer::SetSpeedState(ESpeedState State)
{
	SpeedState = State;
}

bool ABasePlayer::IsEquipped()
{
	return WeaponState == EWeaponState::EWS_Equipped;
}

bool ABasePlayer::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ABasePlayer::IsBlocking()
{
	return ActionState == EActionState::EAS_Blocking;
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
	return Weapon && (IsUnoccupied() || IsBlocking());
}

bool ABasePlayer::CanAttack()
{
	return IsEquipped() && (IsUnoccupied() || IsBlocking());
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
