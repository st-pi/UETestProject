#pragma once

#include "CoreMinimal.h"
#include "Enemies/TopDownShooterEnemy.h"
#include "TopDownShooterKamikazeEnemy.generated.h"

class USphereComponent;

UCLASS()
class ATopDownShooterKamikazeEnemy : public ATopDownShooterEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DamageSphere;

public:
	ATopDownShooterKamikazeEnemy();

private:
	UFUNCTION()
	void HandleDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
