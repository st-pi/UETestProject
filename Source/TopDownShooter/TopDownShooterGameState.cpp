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

void ATopDownShooterGameState::SetGameOver()
{
	if (bGameOver)
	{
		return;
	}

	bGameOver = true;

	OnGameOver.Broadcast();
}

bool ATopDownShooterGameState::IsGameOver() const
{
	return bGameOver;
}
