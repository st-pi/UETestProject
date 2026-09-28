// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterGameMode.h"
#include "TopDownShooterCharacter.h"
#include "TopDownShooterPlayerController.h"
#include "TopDownShooterHUD.h"
#include "TopDownShooterGameState.h"

ATopDownShooterGameMode::ATopDownShooterGameMode()
{
	DefaultPawnClass = ATopDownShooterCharacter::StaticClass();
	PlayerControllerClass = ATopDownShooterPlayerController::StaticClass();
	HUDClass = ATopDownShooterHUD::StaticClass();
	GameStateClass = ATopDownShooterGameState::StaticClass();
}