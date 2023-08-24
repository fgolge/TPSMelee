// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_FinishEquipping.h"
#include "Interfaces/Interface_Character.h"

void UAnimNotify_FinishEquipping::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	if(MeshComp->GetOwner()->Implements<UInterface_Character>())
	{
		IInterface_Character::Execute_FinishEquipping(MeshComp->GetOwner());
	}
}
