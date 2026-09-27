#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterHealthWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UTopDownShooterHealthComponent;

UCLASS(abstract)
class UTopDownShooterHealthWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HealthText;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool bHideWhenFull = false;

public:
	void SetHealthComponent(UTopDownShooterHealthComponent* HealthComponent);

private:
	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);

};
