// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSMeleeGameMode.h"
#include "TPSMeleeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPSMeleeGameMode::ATPSMeleeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
