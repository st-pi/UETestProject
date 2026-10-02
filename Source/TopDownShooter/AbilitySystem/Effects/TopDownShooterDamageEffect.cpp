#include "AbilitySystem/Effects/TopDownShooterDamageEffect.h"
#include "AbilitySystem/Attributes/TopDownShooterHealthAttributeSet.h"
#include "AbilitySystem/TopDownShooterGameplayTags.h"

UTopDownShooterDamageEffect::UTopDownShooterDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FSetByCallerFloat DamageMagnitude;
	DamageMagnitude.DataTag = TopDownShooterTags::Data_Damage;

	FGameplayModifierInfo DamageModifier;
	DamageModifier.Attribute = UTopDownShooterHealthAttributeSet::GetIncomingDamageAttribute();
	DamageModifier.ModifierOp = EGameplayModOp::Additive;
	DamageModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageMagnitude);

	Modifiers.Add(DamageModifier);
}
