#include "TopDownShooterAIController.h"
#include "TopDownShooterEnemy.h"
#include "BehaviorTree/BehaviorTree.h"

void ATopDownShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const ATopDownShooterEnemy* Enemy = Cast<ATopDownShooterEnemy>(InPawn);
	if (!Enemy || !Enemy->GetBehaviorTree())
	{
		return;
	}

	RunBehaviorTree(Enemy->GetBehaviorTree());
}
