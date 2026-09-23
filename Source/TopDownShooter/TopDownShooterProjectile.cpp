// Copyright Epic Games, Inc. All Rights Reserved.


#include "TopDownShooterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TopDownShooterEnemy.h"

ATopDownShooterProjectile::ATopDownShooterProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 2.0f;

	RootComponent = CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	CollisionSphere->SetSphereRadius(35.0f);
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);

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

void ATopDownShooterProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* ProjectileOwner = GetOwner())
	{
		CollisionSphere->IgnoreActorWhenMoving(ProjectileOwner, true);
	}
}

void ATopDownShooterProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (ATopDownShooterEnemy* Enemy = Cast<ATopDownShooterEnemy>(Other))
	{
		Enemy->Destroy();
	}
}

void ATopDownShooterProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	// destroy this actor immediately
	Destroy();
}
