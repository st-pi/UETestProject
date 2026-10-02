#include "TopDownShooterAmmoCostEffect.h"
#include "TopDownShooterWeaponAttributeSet.h"

UTopDownShooterAmmoCostEffect::UTopDownShooterAmmoCostEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo AmmoModifier;
	AmmoModifier.Attribute = UTopDownShooterWeaponAttributeSet::GetAmmoAttribute();
	AmmoModifier.ModifierOp = EGameplayModOp::Additive;
	AmmoModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-1.f));

	Modifiers.Add(AmmoModifier);
}
