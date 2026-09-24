// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "TopDownShooterProjectile.h"
#include "TopDownShooterHealthComponent.h"

ATopDownShooterCharacter::ATopDownShooterCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	HealthComponent = CreateDefaultSubobject<UTopDownShooterHealthComponent>(TEXT("HealthComponent"));

	// Create the camera boom component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1400.f;
	CameraBoom->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 3.f;

	// Create the camera component
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATopDownShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownShooterCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	StopFire();
}

void ATopDownShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PlayerController = GetController<APlayerController>();
	if (!PlayerController)
	{
		return;
	}

	FVector MouseLocation;
	FVector MouseDirection;
	if (!PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection) || FMath::IsNearlyZero(MouseDirection.Z))
	{
		return;
	}

	const FVector CharacterLocation = GetActorLocation();
	const FVector AimPoint = MouseLocation + MouseDirection * ((CharacterLocation.Z - MouseLocation.Z) / MouseDirection.Z);

	SetActorRotation(FRotator(0.f, (AimPoint - CharacterLocation).Rotation().Yaw, 0.f));
}

void ATopDownShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownShooterCharacter::Move);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopDownShooterCharacter::StartFire);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ATopDownShooterCharacter::StopFire);
	}
}

float ATopDownShooterCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	HealthComponent->ApplyDamage(ActualDamage);

	if (HealthComponent->IsDead())
	{
		// gameover
	}

	return ActualDamage;
}

void ATopDownShooterCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveInput = Value.Get<FVector2D>();

	AddMovementInput(FVector::ForwardVector, MoveInput.Y);
	AddMovementInput(FVector::RightVector, MoveInput.X);
}

void ATopDownShooterCharacter::StartFire()
{
	Fire();
	GetWorldTimerManager().SetTimer(FireTimer, this, &ATopDownShooterCharacter::Fire, FireRate, true);
}

void ATopDownShooterCharacter::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void ATopDownShooterCharacter::Fire()
{
	if (!ProjectileClass)
	{
		return;
	}

	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(ProjectileSpawnOffset);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ATopDownShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}
