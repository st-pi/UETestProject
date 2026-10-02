#include "Enemies/TopDownShooterRangedEnemy.h"
#include "Weapons/TopDownShooterProjectile.h"
#include "AbilitySystem/Attributes/TopDownShooterWeaponAttributeSet.h"
#include "AbilitySystem/TopDownShooterAbilityStatics.h"
#include "UI/Widgets/TopDownShooterReloadWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ATopDownShooterRangedEnemy::ATopDownShooterRangedEnemy()
{
	CreateDefaultSubobject<UTopDownShooterWeaponAttributeSet>(TEXT("WeaponAttributeSet"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	WeaponMesh->SetupAttachment(GetMesh(), TEXT("HandGrip_R"));
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ReloadBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ReloadBarWidget"));

	ReloadBarWidget->SetupAttachment(RootComponent);
	ReloadBarWidget->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 45.f));
	ReloadBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ReloadBarWidget->SetDrawSize(FVector2D(100.f, 10.f));
	ReloadBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ATopDownShooterRangedEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (UTopDownShooterReloadWidget* ReloadWidget = Cast<UTopDownShooterReloadWidget>(ReloadBarWidget->GetUserWidgetObject()))
	{
		ReloadWidget->SetAbilitySystemComponent(GetAbilitySystemComponent());
	}
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
