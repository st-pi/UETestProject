// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownShooterCharacter.generated.h"

class UInputAction;
class ATopDownShooterProjectile;
struct FInputActionValue;

/**
 *  A controllable top-down perspective character
 */
UCLASS()
class ATopDownShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ATopDownShooterProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector ProjectileSpawnOffset = FVector(0.0, 0.0, 0.0);

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (ClampMin = 0.01, Units = "s"))
	float FireRate = 0.15f;

	FTimerHandle FireTimer;

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

private:

	/** Handles WASD movement input */
	void Move(const FInputActionValue& Value);

	void StartFire();
	void StopFire();
	void Fire();

};

