#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BTService_FocusOnTarget.generated.h"

UCLASS()
class UBTService_FocusOnTarget : public UBTService_DefaultFocus
{
	GENERATED_BODY()

public:
	UBTService_FocusOnTarget();

};
