// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_RotateActor.generated.h"

class ABasePlayer;

/**
 * 
 */
UCLASS(const, hidecategories=Object, collapsecategories, Config = Game, meta=(DisplayName="Rotate Actor"))
class TPSMELEE_API UAnimNotifyState_RotateActor : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

private:
	TObjectPtr<ABasePlayer> Character;
	TObjectPtr<AController> CharacterController;
	bool bShouldRotate;
};
