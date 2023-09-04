// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_RotateActor.h"
#include "Characters/Player/BasePlayer.h"


void UAnimNotifyState_RotateActor::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	if(MeshComp->GetOwner())
	{
		Character = Cast<ABasePlayer>(MeshComp->GetOwner());
		
		if(Character)
		{
			bShouldRotate = !Character->IsEngaged();

			if(bShouldRotate)
			{
				CharacterController = Character->GetController();
			}
		}
	}
}

void UAnimNotifyState_RotateActor::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float FrameDeltaTime)
{
	if(bShouldRotate && Character && CharacterController)
	{
		FRotator CurrentRot = Character->GetActorRotation();
		FRotator TargetRot = FRotator(CurrentRot.Pitch, CharacterController->GetControlRotation().Yaw, CurrentRot.Roll);
		Character->SetActorRotation(FMath::RInterpTo(CurrentRot, TargetRot, FrameDeltaTime, 10.f));
	}
}
