#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TopDownShooterWeaponAttributeSet.generated.h"

UCLASS()
class UTopDownShooterWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Ammo;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxAmmo;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ReloadTime;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Damage;

public:
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterWeaponAttributeSet, Ammo)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterWeaponAttributeSet, MaxAmmo)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterWeaponAttributeSet, ReloadTime)
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownShooterWeaponAttributeSet, Damage)

	UTopDownShooterWeaponAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	bool IsMagazineEmpty() const;

	bool IsMagazineFull() const;

};
