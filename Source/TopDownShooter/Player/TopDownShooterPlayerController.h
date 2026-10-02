// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownShooterPlayerController.generated.h"

class UInputMappingContext;

/**
 *  Player controller for a top-down perspective game.
 *  Implements point and click based controls
 */
UCLASS()
class ATopDownShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	
public:
	/** Constructor */
	ATopDownShooterPlayerController();

protected:
	/** Initialize input bindings */
	virtual void SetupInputComponent() override;
};


