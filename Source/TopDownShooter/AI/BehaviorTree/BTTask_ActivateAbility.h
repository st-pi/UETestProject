#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_ActivateAbility.generated.h"

UCLASS()
class UBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag AbilityTag;

public:
	UBTTask_ActivateAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

};
