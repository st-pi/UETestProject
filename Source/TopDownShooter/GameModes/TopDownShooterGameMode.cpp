// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/TopDownShooterGameMode.h"
#include "Player/TopDownShooterCharacter.h"
#include "Player/TopDownShooterPlayerController.h"
#include "UI/TopDownShooterHUD.h"
#include "GameModes/TopDownShooterGameState.h"
#include "Player/TopDownShooterPlayerState.h"
#include "Kismet/GameplayStatics.h"

ATopDownShooterGameMode::ATopDownShooterGameMode()
{
	DefaultPawnClass = ATopDownShooterCharacter::StaticClass();
	PlayerControllerClass = ATopDownShooterPlayerController::StaticClass();
	HUDClass = ATopDownShooterHUD::StaticClass();
	GameStateClass = ATopDownShooterGameState::StaticClass();
	PlayerStateClass = ATopDownShooterPlayerState::StaticClass();
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