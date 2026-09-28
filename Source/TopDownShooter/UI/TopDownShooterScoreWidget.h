#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterScoreWidget.generated.h"

class UTextBlock;
class ATopDownShooterGameState;

UCLASS(abstract)
class UTopDownShooterScoreWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreTextValue;

public:
	void SetGameState(ATopDownShooterGameState* GameState);

private:
	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);

};
