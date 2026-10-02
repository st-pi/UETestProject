#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterScoreWidget.generated.h"

class UTextBlock;

UCLASS(abstract)
class UTopDownShooterScoreWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreTextValue;

public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);

};
