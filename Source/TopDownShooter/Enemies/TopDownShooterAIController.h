#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TopDownShooterAIController.generated.h"

UCLASS()
class ATopDownShooterAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKey = TEXT("TargetActor");

public:
	virtual void OnPossess(APawn* InPawn) override;

};
