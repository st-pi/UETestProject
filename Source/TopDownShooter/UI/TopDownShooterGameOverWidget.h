#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterGameOverWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestartRequested);

UCLASS(abstract)
class UTopDownShooterGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnRestartRequested OnRestartRequested;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FinalScoreText;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

public:
	virtual void NativeConstruct() override;

	void SetFinalScore(int32 FinalScore);

private:
	UFUNCTION()
	void HandleRestartClicked();

};
