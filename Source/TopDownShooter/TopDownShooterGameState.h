#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TopDownShooterGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

UCLASS()
class ATopDownShooterGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameOver OnGameOver;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "Game")
	bool bGameOver = false;

public:
	void AddScore(int32 Amount);

	int32 GetScore() const;

	void SetGameOver();

	bool IsGameOver() const;

};
