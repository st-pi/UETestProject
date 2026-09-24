#include "TopDownShooterHealthComponent.h"
#include "TopDownShooter.h"

UTopDownShooterHealthComponent::UTopDownShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTopDownShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

float UTopDownShooterHealthComponent::ApplyDamage(float Damage)
{
	if (Damage <= 0.f || IsDead())
	{
		return 0.f;
	}

	const float TakenDamage = FMath::Min(Damage, CurrentHealth);
	const float PreviousHealth = CurrentHealth;

	CurrentHealth -= TakenDamage;

	UE_LOG(LogTopDownShooter, Log, TEXT("%s took %.1f damage, health %.1f -> %.1f%s"),
		*GetNameSafe(GetOwner()), TakenDamage, PreviousHealth, CurrentHealth, IsDead() ? TEXT(" (dead)") : TEXT(""));

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	return TakenDamage;
}

bool UTopDownShooterHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.f;
}

float UTopDownShooterHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UTopDownShooterHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}
