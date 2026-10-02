#include "TopDownShooterFireAbility.h"
#include "TopDownShooterAmmoCostEffect.h"
#include "TopDownShooterFireCooldownEffect.h"
#include "TopDownShooterWeaponAttributeSet.h"
#include "TopDownShooterWeaponUser.h"
#include "TopDownShooterGameplayTags.h"
#include "AbilitySystemComponent.h"

UTopDownShooterFireAbility::UTopDownShooterFireAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	CostGameplayEffectClass = UTopDownShooterAmmoCostEffect::StaticClass();
	CooldownGameplayEffectClass = UTopDownShooterFireCooldownEffect::StaticClass();

	ActivationBlockedTags.AddTag(TopDownShooterTags::State_Reloading.GetTag());

	SetAssetTags(FGameplayTagContainer(TopDownShooterTags::Ability_Fire.GetTag()));
}

void UTopDownShooterFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ITopDownShooterWeaponUser* WeaponUser = Cast<ITopDownShooterWeaponUser>(GetAvatarActorFromActorInfo()))
	{
		WeaponUser->FireWeapon();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UTopDownShooterFireAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	if (!CooldownEffect)
	{
		return;
	}

	const UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem ? AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>() : nullptr;
	if (!WeaponAttributes)
	{
		return;
	}

	const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(CooldownEffect->GetClass(), GetAbilityLevel());
	if (!Spec.IsValid())
	{
		return;
	}

	Spec.Data->SetSetByCallerMagnitude(TopDownShooterTags::Data_Cooldown.GetTag(), WeaponAttributes->GetFireRate());

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
}
