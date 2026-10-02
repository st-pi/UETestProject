#include "UI/Widgets/TopDownShooterHealthWidget.h"
#include "AbilitySystem/Attributes/TopDownShooterHealthAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
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

void UTopDownShooterHealthWidget::NativeDestruct()
{
	if (BoundAbilitySystem.IsValid())
	{
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterHealthAttributeSet::GetHealthAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterHealthAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
	}

	BoundAbilitySystem.Reset();

	Super::NativeDestruct();
}

void UTopDownShooterHealthWidget::SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem)
{
	if (!AbilitySystem || BoundAbilitySystem == AbilitySystem)
	{
		return;
	}

	if (BoundAbilitySystem.IsValid())
	{
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterHealthAttributeSet::GetHealthAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterHealthAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
	}

	BoundAbilitySystem = AbilitySystem;

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterHealthAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UTopDownShooterHealthWidget::HandleAttributeChanged);

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterHealthAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &UTopDownShooterHealthWidget::HandleAttributeChanged);

	RefreshBar();
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

	SetAbilitySystemComponent(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn));
}

void UTopDownShooterHealthWidget::HandleAttributeChanged(const FOnAttributeChangeData& Data)
{
	RefreshBar();
}

void UTopDownShooterHealthWidget::RefreshBar()
{
	if (!BoundAbilitySystem.IsValid())
	{
		return;
	}

	const UTopDownShooterHealthAttributeSet* Attributes = BoundAbilitySystem->GetSet<UTopDownShooterHealthAttributeSet>();
	if (!Attributes)
	{
		return;
	}

	const float CurrentHealth = Attributes->GetHealth();
	const float MaxHealth = Attributes->GetMaxHealth();

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
