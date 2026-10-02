#include "GameModes/TopDownShooterGameInstance.h"
#include "AbilitySystemGlobals.h"

void UTopDownShooterGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
