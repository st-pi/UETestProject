#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownShooterEnemy.generated.h"

UCLASS()
class ATopDownShooterEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = 0.05, ClampMax = 5, Units = "s"))
	float ChaseUpdateRate = 0.3f;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float AcceptanceRadius = 100.f;

	FTimerHandle ChaseTimer;

public:
	ATopDownShooterEnemy();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	void UpdateChase();

};
