// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemies/TopDownShooterEnemy.h"
#include "AI/TopDownShooterAIController.h"
#include "AbilitySystem/Attributes/TopDownShooterHealthAttributeSet.h"
#include "AbilitySystem/TopDownShooterAbilityStatics.h"
#include "UI/Widgets/TopDownShooterHealthWidget.h"
#include "GameModes/TopDownShooterGameState.h"
#include "AbilitySystemComponent.h"
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

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	CreateDefaultSubobject<UTopDownShooterHealthAttributeSet>(TEXT("HealthAttributeSet"));

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

	HealthAttributeSet = AbilitySystemComponent->GetSet<UTopDownShooterHealthAttributeSet>();

	UTopDownShooterAbilityStatics::ApplyEffectToSelf(AbilitySystemComponent, DefaultAttributes, this);

	for (const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
	{
		if (Ability)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, INDEX_NONE, this));
		}
	}

	HealthAttributeSet->OnOutOfHealth.AddUObject(this, &ATopDownShooterEnemy::HandleOutOfHealth);

	if (UTopDownShooterHealthWidget* HealthWidget = Cast<UTopDownShooterHealthWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		HealthWidget->SetAbilitySystemComponent(AbilitySystemComponent);
	}
}

void ATopDownShooterEnemy::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HealthAttributeSet->OnOutOfHealth.RemoveAll(this);
}

UAbilitySystemComponent* ATopDownShooterEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATopDownShooterEnemy::HandleOutOfHealth(AActor* DeadActor)
{
	if (ATopDownShooterGameState* GameState = GetWorld()->GetGameState<ATopDownShooterGameState>())
	{
		GameState->AddScore(ScoreValue);
	}

	SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimerForNextTick(this, &ATopDownShooterEnemy::FinishDeath);
}

void ATopDownShooterEnemy::FinishDeath()
{
	Destroy();
}

UBehaviorTree* ATopDownShooterEnemy::GetBehaviorTree() const
{
	return BehaviorTree;
}
