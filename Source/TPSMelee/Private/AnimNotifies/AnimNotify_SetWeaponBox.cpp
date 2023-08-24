// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_SetWeaponBox.h"
#include "Characters/BaseCharacter.h"

UAnimNotify_SetWeaponBox::UAnimNotify_SetWeaponBox()
{
	
}

void UAnimNotify_SetWeaponBox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
											   const FAnimNotifyEventReference& EventReference)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if(!BaseCharacter) return;

	if(bShouldEnable)
	{
		// BaseCharacter->SetWeaponCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		// BaseCharacter->SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

