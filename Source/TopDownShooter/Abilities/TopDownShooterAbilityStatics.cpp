#include "TopDownShooterAbilityStatics.h"
#include "TopDownShooterGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UTopDownShooterAbilityStatics::ApplyDamage(AActor* Target, AActor* Source, TSubclassOf<UGameplayEffect> DamageEffect, float Damage)
{
	if (!Target || !DamageEffect || Damage <= 0.f)
	{
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!TargetAbilitySystem)
	{
		return;
	}

	UAbilitySystemComponent* SourceAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	UAbilitySystemComponent* EffectSource = SourceAbilitySystem ? SourceAbilitySystem : TargetAbilitySystem;

	FGameplayEffectContextHandle Context = EffectSource->MakeEffectContext();
	Context.AddSourceObject(Source);

	const FGameplayEffectSpecHandle Spec = EffectSource->MakeOutgoingSpec(DamageEffect, 1.f, Context);
	if (!Spec.IsValid())
	{
		return;
	}

	Spec.Data->SetSetByCallerMagnitude(TopDownShooterTags::Data_Damage.GetTag(), Damage);

	EffectSource->ApplyGameplayEffectSpecToTarget(*Spec.Data, TargetAbilitySystem);
}
