#include "Enemies/TopDownShooterKamikazeEnemy.h"
#include "AbilitySystem/TopDownShooterGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"

ATopDownShooterKamikazeEnemy::ATopDownShooterKamikazeEnemy()
{
	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));

	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->SetSphereRadius(60.f);
	DamageSphere->SetCollisionProfileName(TEXT("EnemyDamage"));
	DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &ATopDownShooterKamikazeEnemy::HandleDamageOverlap);
}

void ATopDownShooterKamikazeEnemy::HandleDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();

	if (AbilitySystem->TryActivateAbilitiesByTag(FGameplayTagContainer(TopDownShooterTags::Ability_Explode.GetTag())))
	{
		Destroy();
	}
}
