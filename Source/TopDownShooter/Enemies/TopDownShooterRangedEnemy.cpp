#include "TopDownShooterRangedEnemy.h"
#include "TopDownShooterProjectile.h"
#include "TopDownShooterWeaponAttributeSet.h"
#include "TopDownShooterAbilityStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ATopDownShooterRangedEnemy::ATopDownShooterRangedEnemy()
{
	CreateDefaultSubobject<UTopDownShooterWeaponAttributeSet>(TEXT("WeaponAttributeSet"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	WeaponMesh->SetupAttachment(GetMesh(), TEXT("HandGrip_R"));
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ATopDownShooterRangedEnemy::FireWeapon()
{
	if (!ProjectileClass)
	{
		return;
	}

	const FTransform SpawnTransform(GetActorRotation(), WeaponMesh->GetSocketLocation(MuzzleSocketName));

	UTopDownShooterAbilityStatics::SpawnProjectile(this, ProjectileClass, SpawnTransform);
}
