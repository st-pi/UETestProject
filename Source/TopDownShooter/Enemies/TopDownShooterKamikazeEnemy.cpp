#include "TopDownShooterKamikazeEnemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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
	UGameplayStatics::ApplyDamage(OtherActor, ContactDamage, GetController(), this, nullptr);

	Destroy();
}
