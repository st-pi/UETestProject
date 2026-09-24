#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TopDownShooterHealthComponent.generated.h"

UCLASS()
class UTopDownShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = 1))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleInstanceOnly, Category = "Health")
	float CurrentHealth = 0.f;

public:
	UTopDownShooterHealthComponent();

	virtual void BeginPlay() override;

	float ApplyDamage(float Damage);

	bool IsDead() const;

	float GetCurrentHealth() const;

	float GetMaxHealth() const;

};
