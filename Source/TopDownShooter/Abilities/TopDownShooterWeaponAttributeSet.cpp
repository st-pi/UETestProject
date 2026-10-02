#include "TopDownShooterWeaponAttributeSet.h"

UTopDownShooterWeaponAttributeSet::UTopDownShooterWeaponAttributeSet()
	: Ammo(12.f)
	, MaxAmmo(12.f)
	, ReloadTime(1.5f)
	, Damage(25.f)
{
}

void UTopDownShooterWeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetAmmoAttribute())
	{
		NewValue = FMath::Clamp(FMath::RoundToFloat(NewValue), 0.f, GetMaxAmmo());
	}
	else if (Attribute == GetMaxAmmoAttribute())
	{
		NewValue = FMath::Max(FMath::RoundToFloat(NewValue), 1.f);
	}
	else if (Attribute == GetReloadTimeAttribute() || Attribute == GetDamageAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UTopDownShooterWeaponAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute != GetMaxAmmoAttribute() || FMath::IsNearlyEqual(OldValue, NewValue))
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystem = GetOwningAbilitySystemComponent())
	{
		AbilitySystem->ApplyModToAttribute(GetAmmoAttribute(), EGameplayModOp::Override, GetAmmo() + NewValue - OldValue);
	}
}

bool UTopDownShooterWeaponAttributeSet::IsMagazineEmpty() const
{
	return GetAmmo() <= 0.f;
}

bool UTopDownShooterWeaponAttributeSet::IsMagazineFull() const
{
	return GetAmmo() >= GetMaxAmmo();
}
