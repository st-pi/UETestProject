#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TopDownShooterFireAbility.generated.h"

UCLASS()
class UTopDownShooterFireAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTopDownShooterFireAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

};
