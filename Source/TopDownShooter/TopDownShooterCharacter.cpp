// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "TopDownShooterProjectile.h"
#include "TopDownShooterGameMode.h"
#include "TopDownShooterPlayerState.h"
#include "TopDownShooterHealthAttributeSet.h"
#include "TopDownShooterAbilityStatics.h"
#include "TopDownShooterWeaponAttributeSet.h"
#include "AbilitySystemComponent.h"

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

	Hurtbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hurtbox"));
	Hurtbox->SetupAttachment(RootComponent);
	Hurtbox->SetSphereRadius(50.f);
	Hurtbox->SetCollisionProfileName(TEXT("PlayerHitbox"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("HandGrip_R"));
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

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

	if (BoundHealthAttributeSet.IsValid())
	{
		BoundHealthAttributeSet->OnOutOfHealth.RemoveAll(this);
		BoundHealthAttributeSet.Reset();
	}
}

void ATopDownShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
}

UAbilitySystemComponent* ATopDownShooterCharacter::GetAbilitySystemComponent() const
{
	const ATopDownShooterPlayerState* TopDownPlayerState = GetPlayerState<ATopDownShooterPlayerState>();

	return TopDownPlayerState ? TopDownPlayerState->GetAbilitySystemComponent() : nullptr;
}

void ATopDownShooterCharacter::InitializeAbilitySystem()
{
	ATopDownShooterPlayerState* TopDownPlayerState = GetPlayerState<ATopDownShooterPlayerState>();
	if (!TopDownPlayerState)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystem = TopDownPlayerState->GetAbilitySystemComponent();
	if (!AbilitySystem || BoundHealthAttributeSet.IsValid())
	{
		return;
	}

	AbilitySystem->InitAbilityActorInfo(TopDownPlayerState, this);

	const UTopDownShooterHealthAttributeSet* HealthAttributeSet = AbilitySystem->GetSet<UTopDownShooterHealthAttributeSet>();
	if (!HealthAttributeSet)
	{
		return;
	}

	UTopDownShooterAbilityStatics::ApplyEffectToSelf(AbilitySystem, DefaultAttributes, this);

	if (FireAbility)
	{
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(FireAbility, 1, INDEX_NONE, this));
	}

	if (ReloadAbility)
	{
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(ReloadAbility, 1, INDEX_NONE, this));
	}

	BoundHealthAttributeSet = HealthAttributeSet;

	HealthAttributeSet->OnOutOfHealth.AddUObject(this, &ATopDownShooterCharacter::HandleOutOfHealth);
}

void ATopDownShooterCharacter::HandleOutOfHealth(AActor* DeadActor)
{
	if (ATopDownShooterGameMode* GameMode = GetWorld()->GetAuthGameMode<ATopDownShooterGameMode>())
	{
		GameMode->EndGame();
	}
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
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopDownShooterCharacter::TryFire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ATopDownShooterCharacter::TryReload);
	}
}

void ATopDownShooterCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveInput = Value.Get<FVector2D>();

	AddMovementInput(FVector::ForwardVector, MoveInput.Y);
	AddMovementInput(FVector::RightVector, MoveInput.X);
}

void ATopDownShooterCharacter::TryFire()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
	if (!AbilitySystem || !FireAbility)
	{
		return;
	}

	AbilitySystem->TryActivateAbilityByClass(FireAbility);

	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>();	
	if (WeaponAttributes && WeaponAttributes->IsMagazineEmpty())
	{
		TryReload();
	}
}

void ATopDownShooterCharacter::TryReload()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
	if (!AbilitySystem || !ReloadAbility)
	{
		return;
	}

	AbilitySystem->TryActivateAbilityByClass(ReloadAbility);
}

void ATopDownShooterCharacter::FireWeapon()
{
	if (!ProjectileClass)
	{
		return;
	}

	const FTransform SpawnTransform(GetActorRotation(), WeaponMesh->GetSocketLocation(MuzzleSocketName));

	UTopDownShooterAbilityStatics::SpawnProjectile(this, ProjectileClass, SpawnTransform);

	if (!FireMontage)
	{
		return;
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}
