// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectFLGameMode.h"
#include "ProjectFLCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectFLGameMode::AProjectFLGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
