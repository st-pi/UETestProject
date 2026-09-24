#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownShooterHUD.generated.h"

class UTopDownShooterHUDWidget;

UCLASS()
class ATopDownShooterHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UTopDownShooterHUDWidget> HUDWidgetClass;

	UPROPERTY()
	UTopDownShooterHUDWidget* HUDWidget;

public:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Points the HUD at the health of the pawn the owning controller took over */
	void BindToPawn(APawn* Pawn);

};
