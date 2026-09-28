#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TopDownShooterGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

UCLASS()
class ATopDownShooterGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Score")
	int32 Score = 0;

public:
	void AddScore(int32 Amount);

	int32 GetScore() const;

};
