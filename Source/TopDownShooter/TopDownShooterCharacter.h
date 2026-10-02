// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TopDownShooterWeaponUser.h"
#include "TopDownShooterCharacter.generated.h"

class UInputAction;
class USphereComponent;
class UStaticMeshComponent;
class UAnimMontage;
class ATopDownShooterProjectile;
class UAbilitySystemComponent;
class UTopDownShooterHealthAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
struct FInputActionValue;

/**
 *  A controllable top-down perspective character
 */
UCLASS()
class ATopDownShooterCharacter : public ACharacter, public IAbilitySystemInterface, public ITopDownShooterWeaponUser
{
	GENERATED_BODY()

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Hurtbox;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ATopDownShooterProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> FireAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> ReloadAbility;

	TWeakObjectPtr<const UTopDownShooterHealthAttributeSet> BoundHealthAttributeSet;

public:

	/** Constructor */
	ATopDownShooterCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	/** Returns the camera component **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns the Camera Boom component **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Adds input bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void FireWeapon() override;

private:

	void InitializeAbilitySystem();

	void HandleOutOfHealth(AActor* DeadActor);

	void Move(const FInputActionValue& Value);

	void TryFire();
	void TryReload();

};

