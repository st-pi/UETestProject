#include "AbilitySystem/Abilities/TopDownShooterExplodeAbility.h"
#include "AbilitySystem/TopDownShooterAbilityStatics.h"
#include "AbilitySystem/Effects/TopDownShooterDamageEffect.h"
#include "AbilitySystem/TopDownShooterGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UTopDownShooterExplodeAbility::UTopDownShooterExplodeAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	DamageEffect = UTopDownShooterDamageEffect::StaticClass();

	SetAssetTags(FGameplayTagContainer(TopDownShooterTags::Ability_Explode.GetTag()));
}

void UTopDownShooterExplodeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* Avatar = GetAvatarActorFromActorInfo();

	TArray<AActor*> DamagedActors;

	UKismetSystemLibrary::SphereOverlapActors(Avatar, Avatar->GetActorLocation(), Radius, DamagedObjectTypes, nullptr, { Avatar }, DamagedActors);

	for (AActor* Target : DamagedActors)
	{
		UTopDownShooterAbilityStatics::ApplyDamage(Target, Avatar, DamageEffect, Damage);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

#if WITH_EDITOR

EDataValidationResult UTopDownShooterExplodeAbility::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (DamagedObjectTypes.IsEmpty())
	{
		Context.AddError(FText::FromString(TEXT("Damaged Object Types is empty, the explosion will damage nothing.")));

		Result = EDataValidationResult::Invalid;
	}

	if (!DamageEffect)
	{
		Context.AddError(FText::FromString(TEXT("Damage Effect is not set, the explosion will damage nothing.")));

		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
