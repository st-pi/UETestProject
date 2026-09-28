#include "TopDownShooterGameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTopDownShooterGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddUniqueDynamic(this, &UTopDownShooterGameOverWidget::HandleRestartClicked);
}

void UTopDownShooterGameOverWidget::SetFinalScore(int32 FinalScore)
{
	FinalScoreText->SetText(FText::AsNumber(FinalScore));
}

void UTopDownShooterGameOverWidget::HandleRestartClicked()
{
	OnRestartRequested.Broadcast();
}
