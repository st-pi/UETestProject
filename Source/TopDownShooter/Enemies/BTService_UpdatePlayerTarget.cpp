#include "BTService_UpdatePlayerTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_UpdatePlayerTarget::UBTService_UpdatePlayerTarget()
{
	NodeName = TEXT("Update Player Target");

	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_UpdatePlayerTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return;
	}

	BlackboardComponent->SetValueAsObject(GetSelectedBlackboardKey(), UGameplayStatics::GetPlayerPawn(&OwnerComp, 0));
}
