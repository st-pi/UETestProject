#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FireAtTarget.generated.h"

UCLASS()
class UBTTask_FireAtTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FireAtTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
