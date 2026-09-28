#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownShooterHUD.generated.h"

class UTopDownShooterHealthWidget;
class UTopDownShooterScoreWidget;
class UTopDownShooterGameOverWidget;

UCLASS()
class ATopDownShooterHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UTopDownShooterHealthWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UTopDownShooterScoreWidget> ScoreWidgetClass;

	UPROPERTY()
	UTopDownShooterHealthWidget* HUDWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UTopDownShooterGameOverWidget> GameOverWidgetClass;

	UPROPERTY()
	UTopDownShooterScoreWidget* ScoreWidget;

	UPROPERTY()
	UTopDownShooterGameOverWidget* GameOverWidget;

public:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void HandleGameOver();

	UFUNCTION()
	void HandleRestartRequested();

	void BindToPawn(APawn* Pawn);

};
