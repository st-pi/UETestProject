#include "UI/Widgets/TopDownShooterReloadWidget.h"
#include "AbilitySystem/TopDownShooterGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"

void UTopDownShooterReloadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UTopDownShooterReloadWidget::NativeDestruct()
{
	if (BoundAbilitySystem.IsValid() && ReloadingTagHandle.IsValid())
	{
		BoundAbilitySystem->UnregisterGameplayTagEvent(ReloadingTagHandle, TopDownShooterTags::State_Reloading.GetTag());
	}

	BoundAbilitySystem.Reset();
	ReloadingTagHandle.Reset();

	Super::NativeDestruct();
}

void UTopDownShooterReloadWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bReloading || !BoundAbilitySystem.IsValid())
	{
		return;
	}

	const FGameplayEffectQuery ReloadingQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
		FGameplayTagContainer(TopDownShooterTags::State_Reloading.GetTag()));

	const TArray<TPair<float, float>> RemainingAndDuration = BoundAbilitySystem->GetActiveEffectsTimeRemainingAndDuration(ReloadingQuery);
	if (RemainingAndDuration.IsEmpty() || RemainingAndDuration[0].Value <= 0.f)
	{
		return;
	}

	ReloadBar->SetPercent(1.f - RemainingAndDuration[0].Key / RemainingAndDuration[0].Value);
}

void UTopDownShooterReloadWidget::SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem)
{
	if (!AbilitySystem || BoundAbilitySystem == AbilitySystem)
	{
		return;
	}

	if (BoundAbilitySystem.IsValid() && ReloadingTagHandle.IsValid())
	{
		BoundAbilitySystem->UnregisterGameplayTagEvent(ReloadingTagHandle, TopDownShooterTags::State_Reloading.GetTag());
	}

	BoundAbilitySystem = AbilitySystem;

	ReloadingTagHandle = AbilitySystem->RegisterAndCallGameplayTagEvent(
		TopDownShooterTags::State_Reloading.GetTag(),
		FOnGameplayEffectTagCountChanged::FDelegate::CreateUObject(this, &UTopDownShooterReloadWidget::HandleReloadingTagChanged));
}

void UTopDownShooterReloadWidget::HandleReloadingTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bReloading = NewCount > 0;

	if (bReloading)
	{
		ReloadBar->SetPercent(0.f);
	}

	SetVisibility(bReloading ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
