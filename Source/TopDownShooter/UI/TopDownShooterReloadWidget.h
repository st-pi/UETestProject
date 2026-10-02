#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "TopDownShooterReloadWidget.generated.h"

class UProgressBar;
class UAbilitySystemComponent;

UCLASS(abstract)
class UTopDownShooterReloadWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ReloadBar;

	TWeakObjectPtr<UAbilitySystemComponent> BoundAbilitySystem;

	FDelegateHandle ReloadingTagHandle;

	bool bReloading = false;

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem);

private:
	void HandleReloadingTagChanged(const FGameplayTag Tag, int32 NewCount);

};
