#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TopDownShooterProgressionAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelUp, float);

UCLASS()
class UTopDownShooterProgressionAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	mutable FOnLevelUp OnLevelUp;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Progression", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CurrentExperience;

	UPROPERTY(BlueprintReadOnly, Category = "Progression", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ExperienceRequiredToLevelUp;

	UPROPERTY(BlueprintReadOnly, Category = "Progression", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData IncomingExperience;

	UPROPERTY(BlueprintReadOnly, Category = "Progression", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CurrentLevel;

public:
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterProgressionAttributeSet, CurrentExperience)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterProgressionAttributeSet, ExperienceRequiredToLevelUp)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterProgressionAttributeSet, CurrentLevel)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterProgressionAttributeSet, IncomingExperience)

	UTopDownShooterProgressionAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void LevelUp();
	float CalculateExperienceForNextLevel() const;
};
