#include "AI/BehaviorTree/BTTask_FindPointAroundTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTTask_FindPointAroundTarget::UBTTask_FindPointAroundTarget()
{
	NodeName = TEXT("Find Point Around Target");

	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointAroundTarget, TargetKey), AActor::StaticClass());

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointAroundTarget, BlackboardKey));
}

void UBTTask_FindPointAroundTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardAsset = GetBlackboardAsset())
	{
		TargetKey.ResolveSelectedKey(*BlackboardAsset);
	}
}

EBTNodeResult::Type UBTTask_FindPointAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!BlackboardComponent || !AIController)
	{
		return EBTNodeResult::Failed;
	}

	const APawn* OwnerPawn = AIController->GetPawn();
	const AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!OwnerPawn || !Target)
	{
		return EBTNodeResult::Failed;
	}

	const FVector TargetLocation = Target->GetActorLocation();

	FVector AwayFromTarget = (OwnerPawn->GetActorLocation() - TargetLocation).GetSafeNormal2D();
	if (AwayFromTarget.IsNearlyZero())
	{
		AwayFromTarget = OwnerPawn->GetActorForwardVector().GetSafeNormal2D();
	}

	const float Angle = FMath::FRandRange(-AngleRange, AngleRange);
	const FVector Direction = AwayFromTarget.RotateAngleAxis(Angle, FVector::UpVector);

	const UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(OwnerPawn->GetWorld());
	if (!NavigationSystem)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation ProjectedLocation;
	if (!NavigationSystem->ProjectPointToNavigation(TargetLocation + Direction * Radius, ProjectedLocation, FVector(ProjectionExtent)))
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ProjectedLocation.Location);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindPointAroundTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s at %.0f around %s, spread %.0f deg"),
		*GetSelectedBlackboardKey().ToString(),
		Radius,
		*TargetKey.SelectedKeyName.ToString(),
		AngleRange);
}
