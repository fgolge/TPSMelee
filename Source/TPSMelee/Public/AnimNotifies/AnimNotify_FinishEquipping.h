// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FinishEquipping.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories=Object, collapsecategories, Config = Game, meta=(DisplayName="Finish Equipping"))
class TPSMELEE_API UAnimNotify_FinishEquipping : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
