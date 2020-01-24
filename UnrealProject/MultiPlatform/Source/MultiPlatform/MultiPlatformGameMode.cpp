// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MultiPlatformGameMode.h"
#include "MultiPlatformCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiPlatformGameMode::AMultiPlatformGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
