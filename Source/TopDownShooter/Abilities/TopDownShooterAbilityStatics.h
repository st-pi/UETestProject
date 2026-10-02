#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TopDownShooterAbilityStatics.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class ATopDownShooterProjectile;

UCLASS()
class UTopDownShooterAbilityStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TopDownShooter|Abilities")
	static void ApplyDamage(AActor* Target, AActor* Source, TSubclassOf<UGameplayEffect> DamageEffect, float Damage);

	static void ApplyEffectToSelf(UAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayEffect> Effect, AActor* SourceObject);

	static ATopDownShooterProjectile* SpawnProjectile(APawn* Shooter, TSubclassOf<ATopDownShooterProjectile> ProjectileClass, const FTransform& SpawnTransform);

};
