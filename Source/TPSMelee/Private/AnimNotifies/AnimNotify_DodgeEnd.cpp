// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_DodgeEnd.h"
#include "Characters/Player/BasePlayer.h"

void UAnimNotify_DodgeEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	if(MeshComp->GetOwner())
	{
		ABasePlayer* PlayerCharacter = Cast<ABasePlayer>(MeshComp->GetOwner());
		if(PlayerCharacter)
		{
			PlayerCharacter->DodgeEnd();
		}
	}
}
