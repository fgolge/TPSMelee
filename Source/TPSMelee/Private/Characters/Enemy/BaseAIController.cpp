// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ABaseAIController::ABaseAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SetPerceptionComponent(*AIPerceptionComponent);
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sense Sight"));
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = false;
	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->SetDominantSense(AISenseConfigSight->GetSenseImplementation());
	
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bDrawBoundaries && ControlledPawn)
	{
		DrawBoundaries();
	}
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledPawn = Cast<ABaseEnemy>(GetPawn());

	if(ControlledPawn)
	{
		ControlledPawn->OnCharacterDied.BindUObject(this, &ABaseAIController::HandleEnemyDeath);
	}

	if(AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::TargetSpotted);
	}

	if(BlackboardComponent && BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

	Patrol();
}

void ABaseAIController::TargetSpotted(AActor* Actor, FAIStimulus Stimulus)
{
	if(!Actor) return;
	if(!Actor->IsA(ABaseCharacter::StaticClass())) return;
	if(!BlackboardComponent) return;
	//if(IsAttacking()) return;

	if(ShouldChase(Stimulus))
	{
		Chase(Actor);
	}
	else if(DistanceToTarget > BackwardSensingDistance)
	{
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Investigate();
	}
}

void ABaseAIController::Patrol()
{
	DistanceToTarget = 9999999.f;
	ControlledPawn->SetEnemySpeedState(EEnemySpeedState::EESS_PatrolSpeed);
	SetActionState(EEnemyActionState::EEAS_Patrolling);
}

void ABaseAIController::Chase(AActor* Actor)
{
	if(Actor)
	{
		SetTargetActor(Actor);
		ControlledPawn->SetEnemySpeedState(EEnemySpeedState::EESS_ChaseSpeed);
		SetActionState(EEnemyActionState::EEAS_Chasing);
		GetWorldTimerManager().SetTimer(DistanceTimer, this, &ABaseAIController::ChaseTimerHandler, TimeRateForDistanceCheck, true);
	}
}

void ABaseAIController::Investigate()
{
	ControlledPawn->SetEnemySpeedState(EEnemySpeedState::EESS_InvestigateSpeed);
	SetActionState(EEnemyActionState::EEAS_Investigating);
	SetTargetActor(nullptr);
	ControlledPawn->ClearTargetActor();
	FocusOnTarget(false);
}

void ABaseAIController::Engage()
{
	GetWorldTimerManager().SetTimer(DistanceTimer, this, &ABaseAIController::EngageTimerHandler, TimeRateForDistanceCheck,true);
	FocusOnTarget(true);
	ControlledPawn->SetEnemySpeedState(EEnemySpeedState::EESS_EngagedSpeed);
	SetActionState(EEnemyActionState::EEAS_Engaged);
	SetEngagedState(EEnemyEngagedState::EEAS_Strafe);
}

void ABaseAIController::ChaseTimerHandler()
{
	DistanceToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	
	if(ShouldEngage())
	{
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Engage();
	}
	else if(ShouldInvestigate())
	{
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Investigate();
	}
	if(TargetActor->ActorHasTag(FName("Dead")))
	{
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		FocusOnTarget(false);
		Patrol();
	}
}

void ABaseAIController::EngageTimerHandler()
{
	DistanceToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());

	if(IsAttacking()) return;
	
	if(TargetActor && ShouldDisengage())
	{
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Chase(TargetActor);
		FocusOnTarget(false);
	}
	else if(ShouldFallBack())
	{
		SetEngagedState(EEnemyEngagedState::EEAS_Strafe);
	}
	if(TargetActor->ActorHasTag(FName("Dead")))
	{
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		FocusOnTarget(false);
		Patrol();
	}
}

void ABaseAIController::SetTargetActor(AActor* Actor)
{
	if(Actor)
	{
		TargetActor = Actor;
		ControlledPawn->SetTargetActor(Actor);
		BlackboardComponent->SetValueAsObject(TargetActorKeyName, Actor);
	}
}

void ABaseAIController::FocusOnTarget(bool bShouldFocus)
{
	if(bShouldFocus && TargetActor)
	{
		SetEnemyControl(true);
		SetFocus(TargetActor);
	}
	else
	{
		SetEnemyControl(false);
		ClearFocus(EAIFocusPriority::Default);
	}
}

void ABaseAIController::HandleEnemyDeath()
{
	UnPossess();
}

void ABaseAIController::SetEnemyControl(bool bIsFocused)
{
	if(ControlledPawn && ControlledPawn->GetCharacterMovement())
	{
		ControlledPawn->GetCharacterMovement()->bOrientRotationToMovement = !bIsFocused;
		ControlledPawn->bUseControllerRotationYaw = bIsFocused;
	}
}

void ABaseAIController::SetActionState(EEnemyActionState NewState)
{
	if(BlackboardComponent && ControlledPawn)
	{
		ActionState = NewState;
		BlackboardComponent->SetValueAsEnum(ActionStateKeyName, static_cast<uint8>(NewState));
	}
}

void ABaseAIController::SetEngagedState(EEnemyEngagedState NewState)
{
	if(BlackboardComponent)
	{
		EngagedState = NewState;
		BlackboardComponent->SetValueAsEnum(EngagedStateKeyName, static_cast<uint8>(NewState));
	}
}

float ABaseAIController::GetDistanceToTarget()
{
	return DistanceToTarget;
}

EEnemyActionState ABaseAIController::GetActionState()
{
	return ActionState;
}

EEnemyEngagedState ABaseAIController::GetEngagedState()
{
	return EngagedState;
}

bool ABaseAIController::ShouldChase(FAIStimulus Stimulus)
{
	return Stimulus.WasSuccessfullySensed();
}

bool ABaseAIController::ShouldInvestigate()
{
	return TargetActor && DistanceToTarget > MaxChaseDistance;
}

bool ABaseAIController::ShouldEngage()
{
	return DistanceToTarget < ChaseToEngageDistance;
}

bool ABaseAIController::ShouldDisengage()
{
	return DistanceToTarget > MaxEngageDistance;
}

bool ABaseAIController::IsAttacking()
{
	return EngagedState == EEnemyEngagedState::EEAS_Attack;
}

bool ABaseAIController::ShouldFallBack()
{
	return DistanceToTarget < MinEngageDistance;
}

bool ABaseAIController::IsStrafing()
{
	return EngagedState == EEnemyEngagedState::EEAS_Strafe;
}

void ABaseAIController::DrawBoundaries() const
{
	FVector CenterLoc = ControlledPawn->GetActorLocation() + FVector(0.f, 0.f, -30.f);
	DrawDebugCircle(GetWorld(), CenterLoc, MinEngageDistance, 128, FColor::Red, false, -1, 0, 1, FVector(1, 0 ,0), FVector(0, 1, 0), false);
	DrawDebugCircle(GetWorld(), CenterLoc, MaxEngageDistance, 128, FColor::Red, false, -1, 0, 1, FVector(1, 0 ,0), FVector(0, 1, 0), false);
	DrawDebugCircle(GetWorld(), CenterLoc, ChaseToEngageDistance, 128, FColor::Green, false, -1, 0, 1, FVector(1, 0 ,0), FVector(0, 1, 0), false);
	DrawDebugCircle(GetWorld(), CenterLoc, 1000.f, 128, FColor::Green, false, -1, 0, 1, FVector(1, 0 ,0), FVector(0, 1, 0), false);
	DrawDebugCircle(GetWorld(), CenterLoc, MaxChaseDistance, 128, FColor::Blue, false, -1, 0, 1, FVector(1, 0 ,0), FVector(0, 1, 0), false);
	FVector LefttEndLine = (ControlledPawn->GetActorForwardVector().RotateAngleAxis(60.f, FVector::ZAxisVector) * MaxChaseDistance) + FVector(0.f, 0.f, -30.f);
	FVector RightEndLine = (ControlledPawn->GetActorForwardVector().RotateAngleAxis(-60.f, FVector::ZAxisVector) * MaxChaseDistance) + FVector(0.f, 0.f, -30.f);
	DrawDebugLine(GetWorld(), CenterLoc, ControlledPawn->GetActorLocation() + LefttEndLine, FColor::Green);
	DrawDebugLine(GetWorld(), CenterLoc, ControlledPawn->GetActorLocation() + RightEndLine, FColor::Green);
}
