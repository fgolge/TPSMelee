// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/BaseEnemy.h"
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

	if(AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::TargetSpotted);
	}

	if(BlackboardComponent && BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

	SetBlackboardState(EEnemyActionState::EEAS_Patrolling);
}

void ABaseAIController::TargetSpotted(AActor* Actor, FAIStimulus Stimulus)
{
	if(!Actor) return;
	if(!Actor->IsA(ABaseCharacter::StaticClass())) return;
	if(!BlackboardComponent) return;

	if(ShouldChase(Stimulus))
	{
		ControlledPawn->SetTargetActor(Actor);
		Chase(Actor);
	}
	else if(DistanceToTarget > BackwardSensingDistance)
	{
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Investigate();
	}
	
}

void ABaseAIController::Chase(AActor* Actor)
{
	if(Actor)
	{
		SetTargetActor(Actor);
		SetBlackboardState(EEnemyActionState::EEAS_Chasing);
		GetWorldTimerManager().SetTimer(DistanceTimer, this, &ABaseAIController::ChaseTimerHandler, TimerForDistanceCheck, true);
	}
}

void ABaseAIController::Investigate()
{
	SetBlackboardState(EEnemyActionState::EEAS_Investigating);
	SetTargetActor(nullptr);
}

void ABaseAIController::Engage()
{
	GetWorldTimerManager().SetTimer(DistanceTimer, this, &ABaseAIController::EngageTimerHandler, TimerForDistanceCheck,true);
	SetBlackboardState(EEnemyActionState::EEAS_Attacking);
}

bool ABaseAIController::ShouldChase(FAIStimulus Stimulus)
{
	return Stimulus.WasSuccessfullySensed();
}

void ABaseAIController::SetBlackboardState(EEnemyActionState NewState)
{
	if(BlackboardComponent && ControlledPawn)
	{
		ControlledPawn->SetEnemyActionState(NewState);
		BlackboardComponent->SetValueAsEnum(ActionStateKeyName, static_cast<uint8>(NewState));
	}
}

void ABaseAIController::ChaseTimerHandler()
{
	DistanceToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	
	if(DistanceToTarget < ChaseToEngageDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Engaged"));
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Engage();
	}
	else if(DistanceToTarget > MaxChaseDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Investigating"));
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Investigate();
	}
}

void ABaseAIController::EngageTimerHandler()
{
	DistanceToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	
	if(TargetActor && DistanceToTarget > MaxEngageDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Back To Chasing"));
		GetWorldTimerManager().ClearTimer(DistanceTimer);
		Chase(TargetActor);
	}
	else if(DistanceToTarget < MinEngageDistance)
	{
		// Fall back
		UE_LOG(LogTemp, Warning, TEXT("Falling Back"));
	}
}

void ABaseAIController::SetTargetActor(AActor* Actor)
{
	if(Actor)
	{
		TargetActor = Actor;
		BlackboardComponent->SetValueAsObject(TargetActorKeyName, Actor);
	}
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
