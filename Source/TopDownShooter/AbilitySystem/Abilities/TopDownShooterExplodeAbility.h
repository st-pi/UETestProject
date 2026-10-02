#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/EngineTypes.h"
#include "TopDownShooterExplodeAbility.generated.h"

UCLASS()
class UTopDownShooterExplodeAbility : public UGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion", meta = (ClampMin = 0))
	float Damage = 25.f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion", meta = (ClampMin = 0, Units = "cm"))
	float Radius = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	TArray<TEnumAsByte<EObjectTypeQuery>> DamagedObjectTypes;

public:
	UTopDownShooterExplodeAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

};
