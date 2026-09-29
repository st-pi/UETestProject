#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdatePlayerTarget.generated.h"

UCLASS()
class UBTService_UpdatePlayerTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdatePlayerTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
