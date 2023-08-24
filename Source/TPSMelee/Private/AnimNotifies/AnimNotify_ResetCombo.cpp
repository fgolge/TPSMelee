// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_ResetCombo.h"
#include "Interfaces/Interface_Character.h"

void UAnimNotify_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	IInterface_Character* Interface_Character = Cast<IInterface_Character>(MeshComp->GetOwner());
	if(Interface_Character)
	{
		Interface_Character->Execute_ResetCombo(MeshComp->GetOwner());
	}
}
