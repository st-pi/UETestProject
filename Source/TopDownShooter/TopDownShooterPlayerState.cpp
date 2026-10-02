#include "TopDownShooterPlayerState.h"
#include "TopDownShooterHealthAttributeSet.h"
#include "TopDownShooterWeaponAttributeSet.h"
#include "AbilitySystemComponent.h"

ATopDownShooterPlayerState::ATopDownShooterPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthAttributeSet = CreateDefaultSubobject<UTopDownShooterHealthAttributeSet>(TEXT("HealthAttributeSet"));
	WeaponAttributeSet = CreateDefaultSubobject<UTopDownShooterWeaponAttributeSet>(TEXT("WeaponAttributeSet"));

	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ATopDownShooterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UTopDownShooterHealthAttributeSet* ATopDownShooterPlayerState::GetHealthAttributeSet() const
{
	return HealthAttributeSet;
}
