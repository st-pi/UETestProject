#include "TopDownShooterDamageEffect.h"
#include "TopDownShooterHealthAttributeSet.h"
#include "TopDownShooterGameplayTags.h"

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
