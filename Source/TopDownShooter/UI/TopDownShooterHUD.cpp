#include "TopDownShooterHUD.h"
#include "TopDownShooterGameOverWidget.h"
#include "TopDownShooterGameState.h"
#include "TopDownShooterGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void ATopDownShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		return;
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(PlayerController, HUDWidgetClass);

		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	if (ATopDownShooterGameState* TopDownGameState = GetWorld()->GetGameState<ATopDownShooterGameState>())
	{
		TopDownGameState->OnGameOver.AddUniqueDynamic(this, &ATopDownShooterHUD::HandleGameOver);
	}
}

void ATopDownShooterHUD::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
	}

	if (GameOverWidget)
	{
		GameOverWidget->RemoveFromParent();
		GameOverWidget = nullptr;
	}
}

void ATopDownShooterHUD::HandleGameOver()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!GameOverWidgetClass || !PlayerController)
	{
		return;
	}

	GameOverWidget = CreateWidget<UTopDownShooterGameOverWidget>(PlayerController, GameOverWidgetClass);
	if (!GameOverWidget)
	{
		return;
	}

	GameOverWidget->OnRestartRequested.AddUniqueDynamic(this, &ATopDownShooterHUD::HandleRestartRequested);
	GameOverWidget->AddToViewport();

	PlayerController->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(GameOverWidget->TakeWidget()));
}

void ATopDownShooterHUD::HandleRestartRequested()
{
	if (ATopDownShooterGameMode* GameMode = GetWorld()->GetAuthGameMode<ATopDownShooterGameMode>())
	{
		GameMode->RestartGame();
	}
}
