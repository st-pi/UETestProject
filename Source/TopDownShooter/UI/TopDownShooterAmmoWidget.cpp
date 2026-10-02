#include "TopDownShooterAmmoWidget.h"
#include "TopDownShooterWeaponAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void UTopDownShooterAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &UTopDownShooterAmmoWidget::HandlePossessedPawnChanged);

	BindToPawn(PlayerController->GetPawn());
}

void UTopDownShooterAmmoWidget::NativeDestruct()
{
	if (BoundAbilitySystem.IsValid())
	{
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterWeaponAttributeSet::GetAmmoAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterWeaponAttributeSet::GetMaxAmmoAttribute()).RemoveAll(this);
	}

	BoundAbilitySystem.Reset();

	Super::NativeDestruct();
}

void UTopDownShooterAmmoWidget::SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem)
{
	if (!AbilitySystem || BoundAbilitySystem == AbilitySystem)
	{
		return;
	}

	if (BoundAbilitySystem.IsValid())
	{
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterWeaponAttributeSet::GetAmmoAttribute()).RemoveAll(this);
		BoundAbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterWeaponAttributeSet::GetMaxAmmoAttribute()).RemoveAll(this);
	}

	BoundAbilitySystem = AbilitySystem;

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterWeaponAttributeSet::GetAmmoAttribute())
		.AddUObject(this, &UTopDownShooterAmmoWidget::RefreshCurrentAmmoText);

	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UTopDownShooterWeaponAttributeSet::GetMaxAmmoAttribute())
		.AddUObject(this, &UTopDownShooterAmmoWidget::RefreshMaxAmmoText);

	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = BoundAbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>();
	if (WeaponAttributes)
	{
		RefreshText(CurrentAmmoText, WeaponAttributes->GetAmmo());
		RefreshText(MaxAmmoText, WeaponAttributes->GetMaxAmmo());
	}
}

void UTopDownShooterAmmoWidget::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindToPawn(NewPawn);
}

void UTopDownShooterAmmoWidget::BindToPawn(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}

	SetAbilitySystemComponent(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn));
}

void UTopDownShooterAmmoWidget::RefreshCurrentAmmoText(const FOnAttributeChangeData& Data)
{
	if (!BoundAbilitySystem.IsValid())
	{
		return;
	}

	RefreshText(CurrentAmmoText, Data.NewValue);
}

void UTopDownShooterAmmoWidget::RefreshMaxAmmoText(const FOnAttributeChangeData& Data)
{
	if (!BoundAbilitySystem.IsValid())
	{
		return;
	}

	RefreshText(MaxAmmoText, Data.NewValue);
}

void UTopDownShooterAmmoWidget::RefreshText(UTextBlock* TextBlock, float Value)
{
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::CeilToInt(Value))));
}