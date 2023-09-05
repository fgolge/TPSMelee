// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_SetWeaponBox.h"
#include "Characters/BaseCharacter.h"


void UAnimNotify_SetWeaponBox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
											   const FAnimNotifyEventReference& EventReference)
{
	if(MeshComp->GetOwner())
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(BaseCharacter)
		{
			bShouldEnable ? BaseCharacter->SetWeaponCollisionEnabled(ECollisionEnabled::QueryOnly) :
				BaseCharacter->SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
}

