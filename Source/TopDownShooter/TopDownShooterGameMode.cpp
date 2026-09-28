// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterGameMode.h"
#include "TopDownShooterCharacter.h"
#include "TopDownShooterPlayerController.h"
#include "TopDownShooterHUD.h"
#include "TopDownShooterGameState.h"
#include "Kismet/GameplayStatics.h"

ATopDownShooterGameMode::ATopDownShooterGameMode()
{
	DefaultPawnClass = ATopDownShooterCharacter::StaticClass();
	PlayerControllerClass = ATopDownShooterPlayerController::StaticClass();
	HUDClass = ATopDownShooterHUD::StaticClass();
	GameStateClass = ATopDownShooterGameState::StaticClass();
}

void ATopDownShooterGameMode::EndGame()
{
	ATopDownShooterGameState* TopDownGameState = GetGameState<ATopDownShooterGameState>();
	if (!TopDownGameState || TopDownGameState->IsGameOver())
	{
		return;
	}

	TopDownGameState->SetGameOver();

	UGameplayStatics::SetGamePaused(this, true);
}

void ATopDownShooterGameMode::RestartGame()
{
	UGameplayStatics::SetGamePaused(this, false);

	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}