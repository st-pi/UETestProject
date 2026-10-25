#include "Player/TopDownShooterPlayerState.h"
#include "AbilitySystem/Attributes/TopDownShooterHealthAttributeSet.h"
#include "AbilitySystem/Attributes/TopDownShooterWeaponAttributeSet.h"
#include "AbilitySystem/Attributes/TopDownShooterProgressionAttributeSet.h"
#include "AbilitySystemComponent.h"

ATopDownShooterPlayerState::ATopDownShooterPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthAttributeSet = CreateDefaultSubobject<UTopDownShooterHealthAttributeSet>(TEXT("HealthAttributeSet"));
	WeaponAttributeSet = CreateDefaultSubobject<UTopDownShooterWeaponAttributeSet>(TEXT("WeaponAttributeSet"));
	ProgressionAttributeSet = CreateDefaultSubobject<UTopDownShooterProgressionAttributeSet>(TEXT("ProgressionAttributeSet"));

	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ATopDownShooterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
