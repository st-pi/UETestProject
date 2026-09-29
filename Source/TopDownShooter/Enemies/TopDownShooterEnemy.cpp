// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterEnemy.h"
#include "TopDownShooterAIController.h"
#include "TopDownShooterHealthComponent.h"
#include "TopDownShooterHealthWidget.h"
#include "TopDownShooterGameState.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ATopDownShooterEnemy::ATopDownShooterEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATopDownShooterAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	HealthComponent = CreateDefaultSubobject<UTopDownShooterHealthComponent>(TEXT("HealthComponent"));

	Hurtbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hurtbox"));

	Hurtbox->SetupAttachment(RootComponent);
	Hurtbox->SetSphereRadius(50.f);
	Hurtbox->SetCollisionProfileName(TEXT("EnemyHitbox"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));

	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 30.f));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(100.f, 10.f));
	HealthBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	if (UTopDownShooterHealthWidget* HealthWidget = Cast<UTopDownShooterHealthWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		HealthWidget->SetHealthComponent(HealthComponent);
	}
}

float ATopDownShooterEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->ApplyDamage(ActualDamage);

	if (HealthComponent->IsDead())
	{
		if (ATopDownShooterGameState* GameState = GetWorld()->GetGameState<ATopDownShooterGameState>())
		{
			GameState->AddScore(ScoreValue);
		}

		Destroy();
	}

	return ActualDamage;
}

UBehaviorTree* ATopDownShooterEnemy::GetBehaviorTree() const
{
	return BehaviorTree;
}
