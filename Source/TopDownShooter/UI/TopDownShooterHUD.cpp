#include "TopDownShooterHUD.h"
#include "TopDownShooterHealthWidget.h"
#include "TopDownShooterScoreWidget.h"
#include "TopDownShooterHealthComponent.h"
#include "TopDownShooterGameState.h"
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

	if (ScoreWidgetClass)
	{
		ScoreWidget = CreateWidget<UTopDownShooterScoreWidget>(PlayerController, ScoreWidgetClass);
		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
			ScoreWidget->SetGameState(GetWorld()->GetGameState<ATopDownShooterGameState>());
		}
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
}

void ATopDownShooterHUD::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindToPawn(NewPawn);
}

void ATopDownShooterHUD::BindToPawn(APawn* Pawn)
{
	if (!HUDWidget || !Pawn)
	{
		return;
	}

	HUDWidget->SetHealthComponent(Pawn->FindComponentByClass<UTopDownShooterHealthComponent>());
}
