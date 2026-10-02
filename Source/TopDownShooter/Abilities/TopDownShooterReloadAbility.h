#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TopDownShooterReloadAbility.generated.h"

class UAnimMontage;
struct FGameplayEffectRemovalInfo;

UCLASS()
class UTopDownShooterReloadAbility : public UGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	TSubclassOf<UGameplayEffect> ReloadingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	UAnimMontage* ReloadMontage;

	FActiveGameplayEffectHandle ReloadingEffectHandle;

public:
	UTopDownShooterReloadAbility();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void PlayReloadMontage(float ReloadTime);

	UFUNCTION()
	void HandleReloadingEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo);

};
