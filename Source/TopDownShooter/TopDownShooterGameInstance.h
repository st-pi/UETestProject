#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TopDownShooterGameInstance.generated.h"

UCLASS()
class UTopDownShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

};
