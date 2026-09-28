#include "TopDownShooterHUD.h"
#include "TopDownShooterHealthWidget.h"
#include "TopDownShooterScoreWidget.h"
#include "TopDownShooterGameOverWidget.h"
#include "TopDownShooterHealthComponent.h"
#include "TopDownShooterGameState.h"
#include "TopDownShooterGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void ATopDownShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		return;
	}

	ATopDownShooterGameState* TopDownGameState = GetWorld()->GetGameState<ATopDownShooterGameState>();

	if (ScoreWidgetClass)
	{
		ScoreWidget = CreateWidget<UTopDownShooterScoreWidget>(PlayerController, ScoreWidgetClass);
		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
			ScoreWidget->SetGameState(TopDownGameState);
		}
	}

	if (TopDownGameState)
	{
		TopDownGameState->OnGameOver.AddUniqueDynamic(this, &ATopDownShooterHUD::HandleGameOver);
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UTopDownShooterHealthWidget>(PlayerController, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &ATopDownShooterHUD::HandlePossessedPawnChanged);
			BindToPawn(PlayerController->GetPawn());
		}
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

	if (ScoreWidget)
	{
		ScoreWidget->RemoveFromParent();
		ScoreWidget = nullptr;
	}

	if (GameOverWidget)
	{
		GameOverWidget->RemoveFromParent();
		GameOverWidget = nullptr;
	}
}

void ATopDownShooterHUD::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindToPawn(NewPawn);
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

	GameOverWidget->AddToViewport();
	GameOverWidget->OnRestartRequested.AddUniqueDynamic(this, &ATopDownShooterHUD::HandleRestartRequested);

	if (ATopDownShooterGameState* TopDownGameState = GetWorld()->GetGameState<ATopDownShooterGameState>())
	{
		GameOverWidget->SetFinalScore(TopDownGameState->GetScore());
	}

	PlayerController->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(GameOverWidget->TakeWidget()));
}

void ATopDownShooterHUD::HandleRestartRequested()
{
	if (ATopDownShooterGameMode* GameMode = GetWorld()->GetAuthGameMode<ATopDownShooterGameMode>())
	{
		GameMode->RestartGame();
	}
}

void ATopDownShooterHUD::BindToPawn(APawn* Pawn)
{
	if (!HUDWidget || !Pawn)
	{
		return;
	}

	HUDWidget->SetHealthComponent(Pawn->FindComponentByClass<UTopDownShooterHealthComponent>());
}
