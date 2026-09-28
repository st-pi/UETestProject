#include "TopDownShooterGameState.h"

void ATopDownShooterGameState::AddScore(int32 Amount)
{
	Score += Amount;

	OnScoreChanged.Broadcast(Score);
}

int32 ATopDownShooterGameState::GetScore() const
{
	return Score;
}
