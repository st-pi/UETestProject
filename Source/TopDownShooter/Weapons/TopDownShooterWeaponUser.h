#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TopDownShooterWeaponUser.generated.h"

UINTERFACE(MinimalAPI)
class UTopDownShooterWeaponUser : public UInterface
{
	GENERATED_BODY()
};

class ITopDownShooterWeaponUser
{
	GENERATED_BODY()

public:
	virtual void FireWeapon() = 0;

};
