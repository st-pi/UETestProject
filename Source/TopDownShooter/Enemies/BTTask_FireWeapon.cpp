#include "BTTask_FireWeapon.h"
#include "TopDownShooterWeaponAttributeSet.h"
#include "TopDownShooterGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBTTask_FireWeapon::UBTTask_FireWeapon()
{
	NodeName = TEXT("Fire Weapon");
}

EBTNodeResult::Type UBTTask_FireWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIController->GetPawn());
	if (!AbilitySystem)
	{
		return EBTNodeResult::Failed;
	}

	if (AbilitySystem->TryActivateAbilitiesByTag(FGameplayTagContainer(TopDownShooterTags::Ability_Fire.GetTag())))
	{
		return EBTNodeResult::Succeeded;
	}

	const UTopDownShooterWeaponAttributeSet* WeaponAttributes = AbilitySystem->GetSet<UTopDownShooterWeaponAttributeSet>();

	if (!WeaponAttributes || !WeaponAttributes->IsMagazineEmpty())
	{
		return EBTNodeResult::Failed;
	}

	const bool bReloading = AbilitySystem->TryActivateAbilitiesByTag(FGameplayTagContainer(TopDownShooterTags::Ability_Reload.GetTag()));

	return bReloading ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
