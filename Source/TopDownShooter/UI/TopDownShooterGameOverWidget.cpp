#include "TopDownShooterGameOverWidget.h"
#include "TopDownShooterGameState.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

void UTopDownShooterGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddUniqueDynamic(this, &UTopDownShooterGameOverWidget::HandleRestartClicked);

	if (ATopDownShooterGameState* GameState = GetWorld()->GetGameState<ATopDownShooterGameState>())
	{
		FinalScoreText->SetText(FText::AsNumber(GameState->GetScore()));
	}
}

void UTopDownShooterGameOverWidget::HandleRestartClicked()
{
	OnRestartRequested.Broadcast();
}
