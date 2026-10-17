#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPointAroundTarget.generated.h"

UCLASS()
class UBTTask_FindPointAroundTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = 0, Units = "cm"))
	float Radius = 600.f;

	UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = 0, ClampMax = 180, Units = "deg"))
	float AngleRange = 45.f;

	UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = 0, Units = "cm"))
	float ProjectionExtent = 200.f;

public:
	UBTTask_FindPointAroundTarget();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

};
