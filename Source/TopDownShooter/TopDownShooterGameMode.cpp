// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterGameMode.h"
#include "TopDownShooterCharacter.h"
#include "TopDownShooterPlayerController.h"

ATopDownShooterGameMode::ATopDownShooterGameMode()
{
	DefaultPawnClass = ATopDownShooterCharacter::StaticClass();
	PlayerControllerClass = ATopDownShooterPlayerController::StaticClass();
}