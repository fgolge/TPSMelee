// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UAIPerceptionStimuliSourceComponent;

/**
 * 
 */
UCLASS()
class TPSMELEE_API ABasePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABasePlayerController();

protected:
	virtual void BeginPlay() override;

protected:
	/**
	 * Variables
	 */

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	FGenericTeamId TeamId;

public:
	/**
	 * Functions
	 */

	virtual FGenericTeamId GetGenericTeamId() const override;
};
