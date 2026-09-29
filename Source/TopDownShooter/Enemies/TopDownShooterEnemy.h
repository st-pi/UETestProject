#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownShooterEnemy.generated.h"

class USphereComponent;
class UWidgetComponent;
class UBehaviorTree;
class UTopDownShooterHealthComponent;

UCLASS(abstract)
class ATopDownShooterEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTopDownShooterHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Hurtbox;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Score", meta = (ClampMin = 0))
	int32 ScoreValue = 10;

public:
	ATopDownShooterEnemy();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UBehaviorTree* GetBehaviorTree() const;

};
