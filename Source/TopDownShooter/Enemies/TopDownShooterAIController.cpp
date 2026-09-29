#include "TopDownShooterAIController.h"
#include "TopDownShooterEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ATopDownShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const ATopDownShooterEnemy* Enemy = Cast<ATopDownShooterEnemy>(InPawn);
	if (!Enemy || !Enemy->GetBehaviorTree())
	{
		return;
	}

	RunBehaviorTree(Enemy->GetBehaviorTree());

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return;
	}

	BlackboardComponent->SetValueAsObject(TargetActorKey, UGameplayStatics::GetPlayerPawn(this, 0));
}
