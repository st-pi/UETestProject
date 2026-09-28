#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownShooterHUD.generated.h"

class UUserWidget;
class UTopDownShooterGameOverWidget;

UCLASS()
class ATopDownShooterHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UTopDownShooterGameOverWidget> GameOverWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidget;

	UPROPERTY()
	UTopDownShooterGameOverWidget* GameOverWidget;

public:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void HandleGameOver();

	UFUNCTION()
	void HandleRestartRequested();

};
