// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_UpdateWarpTarget.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_UpdateWarpTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	if(MeshComp->GetOwner())
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(BaseCharacter)
		{
			BaseCharacter->SetWarpTarget(MaxWarpDistance);
		}
	}
}
