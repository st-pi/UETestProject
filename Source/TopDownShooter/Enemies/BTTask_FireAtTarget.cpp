#include "BTTask_FireAtTarget.h"
#include "TopDownShooterRangedEnemy.h"
#include "AIController.h"

UBTTask_FireAtTarget::UBTTask_FireAtTarget()
{
	NodeName = TEXT("Fire At Target");
}

EBTNodeResult::Type UBTTask_FireAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	ATopDownShooterRangedEnemy* Enemy = Cast<ATopDownShooterRangedEnemy>(AIController->GetPawn());
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->Fire();

	return EBTNodeResult::Succeeded;
}
