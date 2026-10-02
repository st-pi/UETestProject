#include "TopDownShooterFireCooldownEffect.h"
#include "TopDownShooterGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UTopDownShooterFireCooldownEffect::UTopDownShooterFireCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat DurationMagnitudeBySetByCaller;
	DurationMagnitudeBySetByCaller.DataTag = TopDownShooterTags::Data_Cooldown;

	DurationMagnitude = FGameplayEffectModifierMagnitude(DurationMagnitudeBySetByCaller);

	FInheritedTagContainer GrantedTags;
	GrantedTags.AddTag(TopDownShooterTags::Cooldown_Fire.GetTag());

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsComponent"));
	TargetTags->SetAndApplyTargetTagChanges(GrantedTags);

	GEComponents.Add(TargetTags);
}
