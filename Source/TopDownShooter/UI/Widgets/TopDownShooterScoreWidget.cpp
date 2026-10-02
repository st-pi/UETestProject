#include "UI/Widgets/TopDownShooterScoreWidget.h"
#include "GameModes/TopDownShooterGameState.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

void UTopDownShooterScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ATopDownShooterGameState* GameState = GetWorld()->GetGameState<ATopDownShooterGameState>();
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
