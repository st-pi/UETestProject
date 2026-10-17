#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdatePlayerTarget.generated.h"

UCLASS()
class UBTService_UpdatePlayerTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceKey;

public:
	UBTService_UpdatePlayerTarget();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
