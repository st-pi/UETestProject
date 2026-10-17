#include "AI/BehaviorTree/BTService_FocusOnTarget.h"
#include "AIController.h"

UBTService_FocusOnTarget::UBTService_FocusOnTarget()
{
	NodeName = TEXT("Focus On Target");

	FocusPriority = EAIFocusPriority::Gameplay;
}
