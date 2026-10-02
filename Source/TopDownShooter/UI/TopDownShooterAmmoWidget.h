#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterAmmoWidget.generated.h"

class UTextBlock;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS(abstract)
class UTopDownShooterAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxAmmoText;

	TWeakObjectPtr<UAbilitySystemComponent> BoundAbilitySystem;

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem);

private:
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void BindToPawn(APawn* Pawn);

	void RefreshCurrentAmmoText(const FOnAttributeChangeData& Data);
	void RefreshMaxAmmoText(const FOnAttributeChangeData& Data);
	void RefreshText(UTextBlock* TextBlock, float Value);

};
