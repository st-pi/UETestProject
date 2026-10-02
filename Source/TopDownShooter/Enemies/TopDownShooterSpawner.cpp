#include "Enemies/TopDownShooterSpawner.h"
#include "Enemies/TopDownShooterEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ATopDownShooterSpawner::ATopDownShooterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ATopDownShooterSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATopDownShooterSpawner::SpawnEnemy, SpawnInterval, true);
}

void ATopDownShooterSpawner::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void ATopDownShooterSpawner::SpawnEnemy()
{
	if (EnemyClasses.IsEmpty() || AliveEnemies >= MaxAliveEnemies)
	{
		return;
	}

	const TSubclassOf<ATopDownShooterEnemy> EnemyClass = EnemyClasses[FMath::RandRange(0, EnemyClasses.Num() - 1)];
	if (!EnemyClass)
	{
		return;
	}

	const AActor* SpawnPoint = PickSpawnPoint();
	if (!SpawnPoint)
	{
		return;
	}

	const ACharacter* EnemyDefaults = EnemyClass->GetDefaultObject<ACharacter>();
	const float CapsuleHalfHeight = EnemyDefaults->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	ATopDownShooterEnemy* Enemy = GetWorld()->SpawnActor<ATopDownShooterEnemy>(
		EnemyClass,
		SpawnPoint->GetActorLocation() + FVector::UpVector * CapsuleHalfHeight,
		SpawnPoint->GetActorRotation(),
		SpawnParams);

	if (!Enemy)
	{
		return;
	}

	++AliveEnemies;

	Enemy->OnDestroyed.AddDynamic(this, &ATopDownShooterSpawner::HandleEnemyDestroyed);
}

const AActor* ATopDownShooterSpawner::PickSpawnPoint() const
{
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return nullptr;
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	const float MinDistanceSquared = FMath::Square(MinSpawnDistance);

	TArray<const AActor*> Candidates;
	Candidates.Reserve(SpawnPoints.Num());

	for (const AActor* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint && FVector::DistSquared(SpawnPoint->GetActorLocation(), PlayerLocation) >= MinDistanceSquared)
		{
			Candidates.Add(SpawnPoint);
		}
	}

	if (Candidates.IsEmpty())
	{
		return nullptr;
	}

	return Candidates[FMath::RandRange(0, Candidates.Num() - 1)];
}

void ATopDownShooterSpawner::HandleEnemyDestroyed(AActor* DestroyedActor)
{
	--AliveEnemies;
}
