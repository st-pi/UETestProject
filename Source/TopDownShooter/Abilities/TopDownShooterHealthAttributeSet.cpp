#include "TopDownShooterHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "TopDownShooter.h"

UTopDownShooterHealthAttributeSet::UTopDownShooterHealthAttributeSet()
	: Health(100.f)
	, MaxHealth(100.f)
	, IncomingDamage(0.f)
{
}

void UTopDownShooterHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
}

void UTopDownShooterHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute != GetMaxHealthAttribute() || FMath::IsNearlyEqual(OldValue, NewValue))
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystem = GetOwningAbilitySystemComponent())
	{
		AbilitySystem->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, GetHealth() + NewValue - OldValue);
	}
}

void UTopDownShooterHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute != GetIncomingDamageAttribute())
	{
		return;
	}

	const float DamageDone = GetIncomingDamage();
	SetIncomingDamage(0.f);

	if (IsDead() || DamageDone <= 0.f)
	{
		return;
	}

	SetHealth(FMath::Clamp(GetHealth() - DamageDone, 0.f, GetMaxHealth()));

	AActor* Target = Data.Target.GetAvatarActor();

	UE_LOG(LogTopDownShooter, Log, TEXT("%s took %.1f damage, health %.1f/%.1f"),
		*GetNameSafe(Target), DamageDone, GetHealth(), GetMaxHealth());

	if (IsDead())
	{
		OnOutOfHealth.Broadcast(Target);
	}
}

bool UTopDownShooterHealthAttributeSet::IsDead() const
{
	return GetHealth() <= 0.f;
}
