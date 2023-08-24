// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_ResetCombo.h"
#include "Interfaces/Interface_Character.h"

void UAnimNotify_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	if(MeshComp->GetOwner()->Implements<UInterface_Character>())
	{
		IInterface_Character::Execute_ResetCombo(MeshComp->GetOwner());
	}
}
