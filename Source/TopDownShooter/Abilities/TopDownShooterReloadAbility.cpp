#include "TopDownShooterReloadAbility.h"
#include "TopDownShooterWeaponAttributeSet.h"
#include "TopDownShooterGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UTopDownShooterReloadAbility::UTopDownShooterReloadAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	ActivationOwnedTags.AddTag(TopDownShooterTags::State_Reloading.GetTag());

	SetAssetTags(FGameplayTagContainer(TopDownShooterTags::Ability_Reload.GetTag()));
}

bool UTopDownShooterReloadAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem ? AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>() : nullptr;

	return WeaponAttributes && !WeaponAttributes->IsMagazineFull();
}

void UTopDownShooterReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = GetAbilitySystemComponentFromActorInfo()->GetSet<UTopDownShooterWeaponAttributeSet>();

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, WeaponAttributes->GetReloadTime());

	WaitTask->OnFinish.AddDynamic(this, &UTopDownShooterReloadAbility::HandleReloadFinished);

	WaitTask->ReadyForActivation();
}

void UTopDownShooterReloadAbility::HandleReloadFinished()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();

	if (const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>())
	{
		AbilitySystem->SetNumericAttributeBase(UTopDownShooterWeaponAttributeSet::GetAmmoAttribute(), WeaponAttributes->GetMaxAmmo());
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
