// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownShooterGameMode.generated.h"

/**
 *  Simple Game Mode for a top-down perspective game
 *  Sets the default gameplay framework classes
 */
UCLASS()
class ATopDownShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** Constructor */
	ATopDownShooterGameMode();

	void EndGame();

	void RestartGame();
};



