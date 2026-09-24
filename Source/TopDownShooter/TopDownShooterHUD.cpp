#include "TopDownShooterHUD.h"
#include "TopDownShooterHUDWidget.h"
#include "TopDownShooterHealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"

void ATopDownShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!HUDWidgetClass)
	{
		return;
	}

	HUDWidget = CreateWidget<UTopDownShooterHUDWidget>(GetOwningPlayerController(), HUDWidgetClass);
	if (!HUDWidget)
	{
		return;
	}

	HUDWidget->AddToViewport();
}

void ATopDownShooterHUD::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
	}
}

void ATopDownShooterHUD::BindToPawn(APawn* Pawn)
{
	if (!HUDWidget || !Pawn)
	{
		return;
	}

	HUDWidget->SetHealthComponent(Pawn->FindComponentByClass<UTopDownShooterHealthComponent>());
}
