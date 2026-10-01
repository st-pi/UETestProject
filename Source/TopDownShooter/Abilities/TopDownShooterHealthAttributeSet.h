#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TopDownShooterHealthAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOutOfHealth, AActor*);

UCLASS()
class UTopDownShooterHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	mutable FOnOutOfHealth OnOutOfHealth;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData IncomingDamage;

	bool bOutOfHealthBroadcast = false;

public:
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterHealthAttributeSet, Health)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterHealthAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterHealthAttributeSet, IncomingDamage)

	UTopDownShooterHealthAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	bool IsDead() const;

};
