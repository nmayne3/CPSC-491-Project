// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImbadGameMode.h"
#include "ImbadCharacter.h"
#include "UObject/ConstructorHelpers.h"

AImbadGameMode::AImbadGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
