#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TopDownShooterAbilityStatics.generated.h"

class UGameplayEffect;

UCLASS()
class UTopDownShooterAbilityStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TopDownShooter|Abilities")
	static void ApplyDamage(AActor* Target, AActor* Source, TSubclassOf<UGameplayEffect> DamageEffect, float Damage);

};
