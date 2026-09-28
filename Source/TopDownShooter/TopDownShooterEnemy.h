#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownShooterEnemy.generated.h"

class USphereComponent;
class UWidgetComponent;
class UTopDownShooterHealthComponent;

UCLASS()
class ATopDownShooterEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTopDownShooterHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DamageSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = 0.05, ClampMax = 5, Units = "s"))
	float ChaseUpdateRate = 0.3f;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float AcceptanceRadius = 0.f;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (ClampMin = 0))
	float ContactDamage = 25.f;

	UPROPERTY(EditAnywhere, Category = "Score", meta = (ClampMin = 0))
	int32 ScoreValue = 10;

	FTimerHandle ChaseTimer;

public:
	ATopDownShooterEnemy();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void HandleDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UpdateChase();

};
