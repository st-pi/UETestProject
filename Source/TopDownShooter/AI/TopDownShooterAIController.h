#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TopDownShooterAIController.generated.h"

UCLASS()
class ATopDownShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;

};
