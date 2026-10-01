#include "TopDownShooterKamikazeEnemy.h"
#include "TopDownShooterAbilityStatics.h"
#include "TopDownShooterDamageEffect.h"
#include "Components/SphereComponent.h"

ATopDownShooterKamikazeEnemy::ATopDownShooterKamikazeEnemy()
{
	DamageEffect = UTopDownShooterDamageEffect::StaticClass();

	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));

	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->SetSphereRadius(60.f);
	DamageSphere->SetCollisionProfileName(TEXT("EnemyDamage"));
	DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &ATopDownShooterKamikazeEnemy::HandleDamageOverlap);
}

void ATopDownShooterKamikazeEnemy::HandleDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UTopDownShooterAbilityStatics::ApplyDamage(OtherActor, this, DamageEffect, ContactDamage);

	Destroy();
}
