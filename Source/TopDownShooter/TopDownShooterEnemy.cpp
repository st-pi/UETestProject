// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterEnemy.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ATopDownShooterEnemy::ATopDownShooterEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	// spawn an AI controller both for enemies placed in the level and for spawned ones
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	// collision is handled by the capsule, the mesh is visual only
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 250.f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
}

void ATopDownShooterEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ChaseTimer, this, &ATopDownShooterEnemy::UpdateChase, ChaseUpdateRate, true);

	UpdateChase();
}

void ATopDownShooterEnemy::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(ChaseTimer);
}

void ATopDownShooterEnemy::UpdateChase()
{
	AAIController* AIController = GetController<AAIController>();
	if (!AIController)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return;
	}

	AIController->MoveToActor(PlayerPawn, AcceptanceRadius);
}
