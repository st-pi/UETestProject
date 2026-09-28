#include "TopDownShooterScoreWidget.h"
#include "TopDownShooterGameState.h"
#include "Components/TextBlock.h"

void UTopDownShooterScoreWidget::SetGameState(ATopDownShooterGameState* GameState)
{
	if (!GameState)
	{
		return;
	}

	GameState->OnScoreChanged.AddUniqueDynamic(this, &UTopDownShooterScoreWidget::HandleScoreChanged);

	HandleScoreChanged(GameState->GetScore());
}

void UTopDownShooterScoreWidget::HandleScoreChanged(int32 NewScore)
{
	ScoreTextValue->SetText(FText::AsNumber(NewScore));
}
