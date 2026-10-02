// Copyright Epic Games, Inc. All Rights Reserved.


#include "Weapons/TopDownShooterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystem/TopDownShooterAbilityStatics.h"
#include "AbilitySystem/Effects/TopDownShooterDamageEffect.h"

ATopDownShooterProjectile::ATopDownShooterProjectile()
{
	DamageEffect = UTopDownShooterDamageEffect::StaticClass();

 	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 2.0f;

	RootComponent = CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	CollisionSphere->SetSphereRadius(12.0f);
	CollisionSphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName(FName("NoCollision"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 15000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bRotationRemainsVertical = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bForceSubStepping = true;
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ATopDownShooterProjectile::OnProjectileStop);
}

void ATopDownShooterProjectile::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

void ATopDownShooterProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ATopDownShooterProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (bHit || !Other || Other == GetInstigator())
	{
		return;
	}

	bHit = true;

	UTopDownShooterAbilityStatics::ApplyDamage(Other, GetInstigator(), DamageEffect, Damage);
}

void ATopDownShooterProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	Destroy();
}
