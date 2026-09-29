#include "TopDownShooterRangedEnemy.h"
#include "TopDownShooterProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ATopDownShooterRangedEnemy::ATopDownShooterRangedEnemy()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	WeaponMesh->SetupAttachment(GetMesh(), TEXT("HandGrip_R"));
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ATopDownShooterRangedEnemy::Fire()
{
	if (!ProjectileClass)
	{
		return;
	}

	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ATopDownShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}
