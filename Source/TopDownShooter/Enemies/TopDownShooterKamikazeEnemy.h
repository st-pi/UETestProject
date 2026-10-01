#pragma once

#include "CoreMinimal.h"
#include "TopDownShooterEnemy.h"
#include "TopDownShooterKamikazeEnemy.generated.h"

class USphereComponent;
class UGameplayEffect;

UCLASS()
class ATopDownShooterKamikazeEnemy : public ATopDownShooterEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (ClampMin = 0))
	float ContactDamage = 25.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

public:
	ATopDownShooterKamikazeEnemy();

private:
	UFUNCTION()
	void HandleDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
