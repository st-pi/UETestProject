#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownShooterProgressionWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS(abstract)
class UTopDownShooterProgressionWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExperienceBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExperienceText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;
	
	TWeakObjectPtr<UAbilitySystemComponent> BoundAbilitySystem;

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem);

private:
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void BindToPawn(APawn* Pawn);

	void HandleLevelAttributeChanged(const FOnAttributeChangeData& Data);
	void HandleExperienceAttributeChanged(const FOnAttributeChangeData& Data);

	void RefreshExperienceBar();
	void RefreshLevelText();

};
