#include "AI/BehaviorTree/BTTask_ActivateAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	NodeName = TEXT("Activate Ability");
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AbilityTag.IsValid())
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIController->GetPawn());
	if (!AbilitySystem)
	{
		return EBTNodeResult::Failed;
	}

	const bool bActivated = AbilitySystem->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));

	return bActivated ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_ActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("Activate: %s"), *AbilityTag.ToString());
}
