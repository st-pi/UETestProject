#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterHealthWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS(abstract)
class UTopDownShooterHealthWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HealthText;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool bHideWhenFull = false;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool bBindToOwningPawn = false;

	TWeakObjectPtr<UAbilitySystemComponent> BoundAbilitySystem;

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem);

private:
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void BindToPawn(APawn* Pawn);

	void HandleAttributeChanged(const FOnAttributeChangeData& Data);

	void RefreshBar();

};
