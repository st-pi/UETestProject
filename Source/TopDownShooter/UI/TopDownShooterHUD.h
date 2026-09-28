#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownShooterHUD.generated.h"

class UTopDownShooterHealthWidget;
class UTopDownShooterScoreWidget;

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

	UPROPERTY()
	UTopDownShooterScoreWidget* ScoreWidget;

public:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void BindToPawn(APawn* Pawn);

};
