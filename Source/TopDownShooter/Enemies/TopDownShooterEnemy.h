#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TopDownShooterEnemy.generated.h"

class USphereComponent;
class UWidgetComponent;
class UBehaviorTree;
class UAbilitySystemComponent;
class UTopDownShooterHealthAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

UCLASS(abstract)
class ATopDownShooterEnemy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UTopDownShooterHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Hurtbox;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Score", meta = (ClampMin = 0))
	int32 ScoreValue = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

public:
	ATopDownShooterEnemy();

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UBehaviorTree* GetBehaviorTree() const;

private:
	void HandleOutOfHealth(AActor* DeadActor);

	void FinishDeath();

};
