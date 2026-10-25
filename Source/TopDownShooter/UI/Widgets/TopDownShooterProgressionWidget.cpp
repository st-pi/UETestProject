#include "UI/Widgets/TopDownShooterProgressionWidget.h"
#include "AbilitySystem/Attributes/TopDownShooterProgressionAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void UTopDownShooterProgressionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &UTopDownShooterProgressionWidget::HandlePossessedPawnChanged);

	BindToPawn(PlayerController->GetPawn());
}

void UTopDownShooterProgressionWidget::NativeDestruct()
{
	if (BoundAbilitySystem.IsValid())
	{
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetCurrentExperienceAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetExperienceRequiredToLevelUpAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetCurrentLevelAttribute()).RemoveAll(this);
	}

	BoundAbilitySystem.Reset();

	Super::NativeDestruct();
}

void UTopDownShooterProgressionWidget::SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem)
{
	if (!AbilitySystem || BoundAbilitySystem == AbilitySystem)
	{
		return;
	}

	if (BoundAbilitySystem.IsValid())
	{
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetCurrentExperienceAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetExperienceRequiredToLevelUpAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetCurrentLevelAttribute()).RemoveAll(this);
	}

	BoundAbilitySystem = AbilitySystem;

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetCurrentExperienceAttribute())
		.AddUObject(this, &UTopDownShooterProgressionWidget::HandleExperienceAttributeChanged);

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetExperienceRequiredToLevelUpAttribute())
		.AddUObject(this, &UTopDownShooterProgressionWidget::HandleExperienceAttributeChanged);

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterProgressionAttributeSet::GetCurrentLevelAttribute())
		.AddUObject(this, &UTopDownShooterProgressionWidget::HandleLevelAttributeChanged);

	RefreshExperienceBar();
	RefreshLevelText();
}

void UTopDownShooterProgressionWidget::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindToPawn(NewPawn);
}

void UTopDownShooterProgressionWidget::BindToPawn(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}

	SetAbilitySystemComponent(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn));
}

void UTopDownShooterProgressionWidget::HandleLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	RefreshLevelText();
}

void UTopDownShooterProgressionWidget::HandleExperienceAttributeChanged(const FOnAttributeChangeData& Data)
{
	RefreshExperienceBar();
}

void UTopDownShooterProgressionWidget::RefreshExperienceBar()
{
	if (!BoundAbilitySystem.IsValid())
	{
		return;
	}

	const UTopDownShooterProgressionAttributeSet* Attributes = BoundAbilitySystem->GetSet<UTopDownShooterProgressionAttributeSet>();
	if (!Attributes)
	{
		return;
	}

	const float CurrentExp = Attributes->GetCurrentExperience();
	const float ExpToLevelUp = Attributes->GetExperienceRequiredToLevelUp();

	ExperienceBar->SetPercent(ExpToLevelUp > 0.f ? CurrentExp / ExpToLevelUp : 0.f);
	ExperienceText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),
		FMath::CeilToInt(CurrentExp), FMath::CeilToInt(ExpToLevelUp))));
}

void UTopDownShooterProgressionWidget::RefreshLevelText()
{
	if (!BoundAbilitySystem.IsValid())
	{
		return;
	}

	const UTopDownShooterProgressionAttributeSet* Attributes = BoundAbilitySystem->GetSet<UTopDownShooterProgressionAttributeSet>();
	if (!Attributes)
	{
		return;
	}

	const float CurrentLevel = Attributes->GetCurrentLevel();
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"),
		FMath::CeilToInt(CurrentLevel))));
}
