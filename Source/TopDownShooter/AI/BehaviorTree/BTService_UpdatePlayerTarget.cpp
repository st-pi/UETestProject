#include "AI/BehaviorTree/BTService_UpdatePlayerTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

UBTService_UpdatePlayerTarget::UBTService_UpdatePlayerTarget()
{
	NodeName = TEXT("Update Player Target");

	Interval = 0.5f;
	RandomDeviation = 0.1f;

	DistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdatePlayerTarget, DistanceKey));
}

void UBTService_UpdatePlayerTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardAsset = GetBlackboardAsset())
	{
		DistanceKey.ResolveSelectedKey(*BlackboardAsset);
	}
}

void UBTService_UpdatePlayerTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(&OwnerComp, 0);

	BlackboardComponent->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);

	if (!DistanceKey.IsSet())
	{
		return;
	}

	const AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* OwnerPawn = AIController ? AIController->GetPawn() : nullptr;

	if (!PlayerPawn || !OwnerPawn)
	{
		BlackboardComponent->ClearValue(DistanceKey.SelectedKeyName);
		return;
	}

	const float Distance = FVector::Dist2D(OwnerPawn->GetActorLocation(), PlayerPawn->GetActorLocation());

	BlackboardComponent->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
}
