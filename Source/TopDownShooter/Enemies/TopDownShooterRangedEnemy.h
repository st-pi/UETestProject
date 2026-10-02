#pragma once

#include "CoreMinimal.h"
#include "TopDownShooterEnemy.h"
#include "TopDownShooterWeaponUser.h"
#include "TopDownShooterRangedEnemy.generated.h"

class UStaticMeshComponent;
class ATopDownShooterProjectile;

UCLASS()
class ATopDownShooterRangedEnemy : public ATopDownShooterEnemy, public ITopDownShooterWeaponUser
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ATopDownShooterProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

public:
	ATopDownShooterRangedEnemy();

	virtual void FireWeapon() override;

};
