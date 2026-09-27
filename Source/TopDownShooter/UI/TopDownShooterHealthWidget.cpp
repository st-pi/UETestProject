#include "TopDownShooterHealthWidget.h"
#include "TopDownShooterHealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UTopDownShooterHealthWidget::SetHealthComponent(UTopDownShooterHealthComponent* HealthComponent)
{
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &UTopDownShooterHealthWidget::HandleHealthChanged);

	HandleHealthChanged(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
}

void UTopDownShooterHealthWidget::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	HealthBar->SetPercent(MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f);

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),
		FMath::CeilToInt(CurrentHealth), FMath::CeilToInt(MaxHealth))));
	}

	if (bHideWhenFull)
	{
		SetVisibility(CurrentHealth < MaxHealth ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
