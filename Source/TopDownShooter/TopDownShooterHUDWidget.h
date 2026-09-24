#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UTopDownShooterHealthComponent;

UCLASS(abstract)
class UTopDownShooterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

public:
	void SetHealthComponent(UTopDownShooterHealthComponent* HealthComponent);

private:
	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);

};
