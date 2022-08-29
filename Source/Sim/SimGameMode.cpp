// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimGameMode.h"

#include "SimCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASimGameMode::ASimGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = ASimHUD::StaticClass();
}
