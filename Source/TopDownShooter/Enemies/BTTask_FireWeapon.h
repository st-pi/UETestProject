#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FireWeapon.generated.h"

UCLASS()
class UBTTask_FireWeapon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FireWeapon();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
