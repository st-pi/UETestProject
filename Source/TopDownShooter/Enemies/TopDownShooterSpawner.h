#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownShooterSpawner.generated.h"

class ATopDownShooterEnemy;

UCLASS()
class ATopDownShooterSpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<TSubclassOf<ATopDownShooterEnemy>> EnemyClasses;

	UPROPERTY(EditInstanceOnly, Category = "Spawning")
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = 0.1, ClampMax = 60, Units = "s"))
	float SpawnInterval = 2.f;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = 1, ClampMax = 200))
	int32 MaxAliveEnemies = 20;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = 0, Units = "cm"))
	float MinSpawnDistance = 1200.f;

	int32 AliveEnemies = 0;

	FTimerHandle SpawnTimer;

public:
	ATopDownShooterSpawner();

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	void SpawnEnemy();

	const AActor* PickSpawnPoint() const;

	UFUNCTION()
	void HandleEnemyDestroyed(AActor* DestroyedActor);

};
