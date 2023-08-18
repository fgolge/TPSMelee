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

	

	if(TargetEnemy)
	{
		DistanceToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), TargetEnemy->GetActorLocation());

		if(DistanceToTarget < AttackDistance)
		{
			SetBlackboardState(EEnemyActionState::EEAS_Attacking);
		}
		else
		{
			SetBlackboardState(EEnemyActionState::EEAS_Chasing);
		}
	}
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledPawn = Cast<ABaseEnemy>(GetPawn());

	if(AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::PlayerSpotted);
	}
	
	if(BlackboardComponent && BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

	SetBlackboardState(EEnemyActionState::EEAS_Patrolling);
	UE_LOG(LogTemp, Warning, TEXT("Set blackboard state to patrolling"));
	SetGenericTeamId(FGenericTeamId(1));
}

void ABaseAIController::PlayerSpotted(AActor* Actor, FAIStimulus Stimulus)
{
	if(!Actor) return;
	if(!IsHostile(Actor)) return;
	
	if(!BlackboardComponent) return;
	
	if(ShouldChase(Stimulus))
	{
		Chase(Actor);
	}
	else if(ShouldInvestigate())
	{
		Investigate(Actor, Stimulus);
	}
}

void ABaseAIController::Chase(AActor* Actor)
{
	TargetEnemy = Actor;
	BlackboardComponent->SetValueAsObject(TargetActorKeyName, Actor);
	SetBlackboardState(EEnemyActionState::EEAS_Chasing);
}

void ABaseAIController::Investigate(AActor* Actor, FAIStimulus Stimulus)
{
	LastSeenLocation = Stimulus.StimulusLocation;
	BlackboardComponent->SetValueAsObject(TargetActorKeyName, nullptr);
	BlackboardComponent->SetValueAsVector(InvestigationKeyName, LastSeenLocation);
	SetBlackboardState(EEnemyActionState::EEAS_Investigating);
	TargetEnemy = nullptr;
	Actor = nullptr;
	DistanceToTarget = 99999.f;
}

bool ABaseAIController::ShouldChase(FAIStimulus Stimulus)
{
	return Stimulus.WasSuccessfullySensed();
}

bool ABaseAIController::ShouldInvestigate()
{
	return DistanceToTarget > MaxDistanceToTarget;
}

void ABaseAIController::SetBlackboardState(EEnemyActionState NewState)
{
	if(BlackboardComponent && ControlledPawn)
	{
		ControlledPawn->SetEnemyActionState(NewState);
		BlackboardComponent->SetValueAsEnum(ActionStateKeyName, static_cast<uint8>(NewState));
	}
}

UBlackboardComponent* ABaseAIController::GetEnemyBlackboard() const
{
	return BlackboardComponent;
}


bool ABaseAIController::IsHostile(AActor* Actor)
{
	if(GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Hostile)
	{
		return true;
	}
	return false;
}

ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other)
{
	if(const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		const IGenericTeamAgentInterface* TeamAgent = IsTeamAgent(OtherPawn);
		if(TeamAgent)
		{
			FGenericTeamId TeamId = TeamAgent->GetGenericTeamId();
			if(TeamId == 0)
			{
				return ETeamAttitude::Hostile;
			}
			
			return ETeamAttitude::Friendly;
		}
		
		return ETeamAttitude::Friendly;
	}

	return ETeamAttitude::Friendly;
}

IGenericTeamAgentInterface* ABaseAIController::IsTeamAgent(const APawn* OtherPawn)
{
	if(OtherPawn)
	{
		IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
		if(TeamAgentInterface)
		{
			return TeamAgentInterface;
		}

		return nullptr;
	}
	
	return nullptr;
}
