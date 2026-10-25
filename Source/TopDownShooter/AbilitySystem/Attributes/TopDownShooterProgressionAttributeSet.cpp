#include "TopDownShooterProgressionAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "TopDownShooter.h"

constexpr float ExperiencePerLevel = 100.f;

UTopDownShooterProgressionAttributeSet::UTopDownShooterProgressionAttributeSet()
	: CurrentExperience(0.0f)
	, ExperienceRequiredToLevelUp(ExperiencePerLevel)
	, IncomingExperience(0.f)
	, CurrentLevel(1.0f)
{
}

void UTopDownShooterProgressionAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetCurrentExperienceAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	else if (Attribute == GetCurrentLevelAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetExperienceRequiredToLevelUpAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UTopDownShooterProgressionAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute != GetIncomingExperienceAttribute())
	{
		return;
	}

	const float IncomingExp = GetIncomingExperience();
	SetIncomingExperience(0.f);

	if (IncomingExp <= 0.f)
	{
		return;
	}

	SetCurrentExperience(GetCurrentExperience() + IncomingExp);

	while (GetCurrentExperience() >= GetExperienceRequiredToLevelUp())
	{
		LevelUp();
		OnLevelUp.Broadcast(GetCurrentLevel());
	}
}

void UTopDownShooterProgressionAttributeSet::LevelUp()
{
	SetCurrentExperience(FMath::Max(GetCurrentExperience() - GetExperienceRequiredToLevelUp(), 0.0f));
	SetCurrentLevel(GetCurrentLevel() + 1.0f);
	SetExperienceRequiredToLevelUp(CalculateExperienceForNextLevel());
}

float UTopDownShooterProgressionAttributeSet::CalculateExperienceForNextLevel() const
{
	return GetCurrentLevel() * ExperiencePerLevel;
}
