// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterEnemy.h"
#include "TopDownShooterHealthComponent.h"
#include "TopDownShooterHealthWidget.h"
#include "Components/WidgetComponent.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ATopDownShooterEnemy::ATopDownShooterEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	HealthComponent = CreateDefaultSubobject<UTopDownShooterHealthComponent>(TEXT("HealthComponent"));

	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));

	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->SetSphereRadius(60.f);
	DamageSphere->SetCollisionProfileName(TEXT("EnemyDamage"));
	DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &ATopDownShooterEnemy::HandleDamageOverlap);

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

	GetWorldTimerManager().SetTimer(ChaseTimer, this, &ATopDownShooterEnemy::UpdateChase, ChaseUpdateRate, true);

	UpdateChase();

	if (UTopDownShooterHealthWidget* HealthWidget = Cast<UTopDownShooterHealthWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		HealthWidget->SetHealthComponent(HealthComponent);
	}
}

void ATopDownShooterEnemy::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(ChaseTimer);
}

float ATopDownShooterEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->ApplyDamage(ActualDamage);

	if (HealthComponent->IsDead())
	{
		Destroy();
	}

	return ActualDamage;
}

void ATopDownShooterEnemy::HandleDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::ApplyDamage(OtherActor, ContactDamage, GetController(), this, nullptr);

	Destroy();
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
