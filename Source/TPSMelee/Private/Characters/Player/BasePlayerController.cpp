// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/BasePlayerController.h"

#include "Characters/Player/BasePlayer.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ABasePlayerController::ABasePlayerController()
{
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source Component"));
	StimuliSourceComponent->bAutoRegister = true;
	StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	TeamId = FGenericTeamId(0);
	SetGenericTeamId(TeamId);

	ABasePlayer* BasePlayer = Cast<ABasePlayer>(GetPawn());
	if(BasePlayer)
	{
		BasePlayer->OnCharacterDied.BindUObject(this, &ABasePlayerController::HandlePlayerDeath);
	}
}

void ABasePlayerController::HandlePlayerDeath()
{
	if(StimuliSourceComponent)
	{
		StimuliSourceComponent->UnregisterFromSense(TSubclassOf<UAISense_Sight>());
	}
}

FGenericTeamId ABasePlayerController::GetGenericTeamId() const
{
	return TeamId;
}
