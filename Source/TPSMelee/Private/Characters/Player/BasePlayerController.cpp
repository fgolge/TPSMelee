// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/BasePlayerController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ABasePlayerController::ABasePlayerController()
{
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source Component"));
	StimuliSourceComponent->bAutoRegister = true;
	StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	TeamId = FGenericTeamId(0);
	SetGenericTeamId(TeamId);
}

FGenericTeamId ABasePlayerController::GetGenericTeamId() const
{
	return TeamId;
}
