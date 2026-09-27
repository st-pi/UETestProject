#include "TopDownShooterHUD.h"
#include "TopDownShooterHealthWidget.h"
#include "TopDownShooterHealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void ATopDownShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!HUDWidgetClass || !PlayerController)
	{
		return;
	}

	HUDWidget = CreateWidget<UTopDownShooterHealthWidget>(PlayerController, HUDWidgetClass);
	if (!HUDWidget)
	{
		return;
	}

	HUDWidget->AddToViewport();

	PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &ATopDownShooterHUD::HandlePossessedPawnChanged);

	BindToPawn(PlayerController->GetPawn());
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

void ATopDownShooterHUD::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindToPawn(NewPawn);
}

void ATopDownShooterHUD::BindToPawn(APawn* Pawn)
{
	if (!HUDWidget || !Pawn)
	{
		return;
	}

	HUDWidget->SetHealthComponent(Pawn->FindComponentByClass<UTopDownShooterHealthComponent>());
}
