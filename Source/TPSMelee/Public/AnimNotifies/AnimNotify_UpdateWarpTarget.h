// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_UpdateWarpTarget.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories=Object, collapsecategories, Config = Game, meta=(DisplayName="Update Warp Target"))
class TPSMELEE_API UAnimNotify_UpdateWarpTarget : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	float MaxWarpDistance { 300.f };
	
};
