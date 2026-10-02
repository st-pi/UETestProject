#include "TopDownShooterReloadingEffect.h"
#include "TopDownShooterGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UTopDownShooterReloadingEffect::UTopDownShooterReloadingEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat DurationMagnitudeBySetByCaller;
	DurationMagnitudeBySetByCaller.DataTag = TopDownShooterTags::Data_Duration;

	DurationMagnitude = FGameplayEffectModifierMagnitude(DurationMagnitudeBySetByCaller);

	FInheritedTagContainer GrantedTags;
	GrantedTags.AddTag(TopDownShooterTags::State_Reloading.GetTag());

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsComponent"));
	TargetTags->SetAndApplyTargetTagChanges(GrantedTags);

	GEComponents.Add(TargetTags);
}
