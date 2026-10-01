#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "TopDownShooterPlayerState.generated.h"

class UAbilitySystemComponent;
class UTopDownShooterHealthAttributeSet;

UCLASS()
class ATopDownShooterPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UTopDownShooterHealthAttributeSet* HealthAttributeSet;

public:
	ATopDownShooterPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UTopDownShooterHealthAttributeSet* GetHealthAttributeSet() const;

};
