#include "AbilitySystem/TopDownShooterAbilityStatics.h"
#include "AbilitySystem/TopDownShooterGameplayTags.h"
#include "AbilitySystem/Attributes/TopDownShooterWeaponAttributeSet.h"
#include "Weapons/TopDownShooterProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UTopDownShooterAbilityStatics::ApplyDamage(AActor* Target, AActor* Source, TSubclassOf<UGameplayEffect> DamageEffect, float Damage)
{
	if (!Target || !DamageEffect || Damage <= 0.f)
	{
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!TargetAbilitySystem)
	{
		return;
	}

	UAbilitySystemComponent* SourceAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	UAbilitySystemComponent* EffectSource = SourceAbilitySystem ? SourceAbilitySystem : TargetAbilitySystem;

	FGameplayEffectContextHandle Context = EffectSource->MakeEffectContext();
	Context.AddSourceObject(Source);

	const FGameplayEffectSpecHandle Spec = EffectSource->MakeOutgoingSpec(DamageEffect, 1.f, Context);
	if (!Spec.IsValid())
	{
		return;
	}

	Spec.Data->SetSetByCallerMagnitude(TopDownShooterTags::Data_Damage.GetTag(), Damage);

	EffectSource->ApplyGameplayEffectSpecToTarget(*Spec.Data, TargetAbilitySystem);
}

void UTopDownShooterAbilityStatics::ApplyEffectToSelf(UAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayEffect> Effect, AActor* SourceObject)
{
	if (!AbilitySystem || !Effect)
	{
		return;
	}

	FGameplayEffectContextHandle Context = AbilitySystem->MakeEffectContext();
	Context.AddSourceObject(SourceObject);

	const FGameplayEffectSpecHandle Spec = AbilitySystem->MakeOutgoingSpec(Effect, 1.f, Context);
	if (!Spec.IsValid())
	{
		return;
	}

	AbilitySystem->ApplyGameplayEffectSpecToSelf(*Spec.Data);
}

ATopDownShooterProjectile* UTopDownShooterAbilityStatics::SpawnProjectile(APawn* Shooter, TSubclassOf<ATopDownShooterProjectile> ProjectileClass, const FTransform& SpawnTransform)
{
	if (!Shooter || !ProjectileClass)
	{
		return nullptr;
	}

	ATopDownShooterProjectile* Projectile = Shooter->GetWorld()->SpawnActorDeferred<ATopDownShooterProjectile>(
		ProjectileClass, SpawnTransform, Shooter, Shooter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!Projectile)
	{
		return nullptr;
	}

	const UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Shooter);
	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem ? AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>() : nullptr;

	if (WeaponAttributes)
	{
		Projectile->SetDamage(WeaponAttributes->GetDamage());
	}

	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

	return Projectile;
}
