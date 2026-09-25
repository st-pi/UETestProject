#include "TopDownShooterHUDWidget.h"
#include "TopDownShooterHealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UTopDownShooterHUDWidget::SetHealthComponent(UTopDownShooterHealthComponent* HealthComponent)
{
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &UTopDownShooterHUDWidget::HandleHealthChanged);

	HandleHealthChanged(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
}

void UTopDownShooterHUDWidget::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	HealthBar->SetPercent(MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f);

	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),
		FMath::CeilToInt(CurrentHealth), FMath::CeilToInt(MaxHealth))));
}
