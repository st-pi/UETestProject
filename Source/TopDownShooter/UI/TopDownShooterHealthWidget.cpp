#include "TopDownShooterHealthWidget.h"
#include "TopDownShooterHealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void UTopDownShooterHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!bBindToOwningPawn)
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &UTopDownShooterHealthWidget::HandlePossessedPawnChanged);

	BindToPawn(PlayerController->GetPawn());
}

void UTopDownShooterHealthWidget::SetHealthComponent(UTopDownShooterHealthComponent* HealthComponent)
{
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &UTopDownShooterHealthWidget::HandleHealthChanged);

	HandleHealthChanged(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
}

void UTopDownShooterHealthWidget::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindToPawn(NewPawn);
}

void UTopDownShooterHealthWidget::BindToPawn(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}

	SetHealthComponent(Pawn->FindComponentByClass<UTopDownShooterHealthComponent>());
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
