#include "AbilitySystem/Abilities/TopDownShooterReloadAbility.h"
#include "AbilitySystem/Effects/TopDownShooterReloadingEffect.h"
#include "AbilitySystem/Attributes/TopDownShooterWeaponAttributeSet.h"
#include "AbilitySystem/TopDownShooterGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"

UTopDownShooterReloadAbility::UTopDownShooterReloadAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	ReloadingEffect = UTopDownShooterReloadingEffect::StaticClass();

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
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ReloadingEffect)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>();

	const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(ReloadingEffect, GetAbilityLevel());
	if (!Spec.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const float ReloadTime = WeaponAttributes->GetReloadTime();

	Spec.Data->SetSetByCallerMagnitude(TopDownShooterTags::Data_Duration.GetTag(), ReloadTime);

	ReloadingEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);

	UAbilityTask_WaitGameplayEffectRemoved* WaitTask = UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(this, ReloadingEffectHandle);

	WaitTask->OnRemoved.AddDynamic(this, &UTopDownShooterReloadAbility::HandleReloadingEffectRemoved);

	WaitTask->ReadyForActivation();

	PlayReloadMontage(ReloadTime);
}

void UTopDownShooterReloadAbility::PlayReloadMontage(float ReloadTime)
{
	if (!ReloadMontage)
	{
		return;
	}

	const float MontageLength = ReloadMontage->GetPlayLength();
	const float PlayRate = (MontageLength > 0.f && ReloadTime > 0.f) ? MontageLength / ReloadTime : 1.f;

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ReloadMontage, PlayRate);

	MontageTask->ReadyForActivation();
}

void UTopDownShooterReloadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bWasCancelled && ReloadingEffectHandle.IsValid())
	{
		BP_RemoveGameplayEffectFromOwnerWithHandle(ReloadingEffectHandle);
	}

	ReloadingEffectHandle.Invalidate();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTopDownShooterReloadAbility::HandleReloadingEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();

	if (!RemovalInfo.bPrematureRemoval)
	{
		if (const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>())
		{
			AbilitySystem->SetNumericAttributeBase(UTopDownShooterWeaponAttributeSet::GetAmmoAttribute(), WeaponAttributes->GetMaxAmmo());
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
