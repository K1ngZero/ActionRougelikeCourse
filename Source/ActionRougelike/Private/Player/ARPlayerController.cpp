#include "Player/ARPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"

void AARPlayerController::SetPawn(APawn* InPawn)
{
	APawn* const PreviousPawn = GetPawn();

	Super::SetPawn(InPawn);

	OnPawnChangedDelegate.Broadcast(InPawn, PreviousPawn);
}

void AARPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BP_BeginPlayingState();
}

void AARPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChangedDelegate.Broadcast(PlayerState);
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AARPlayerController::TogglePauseMenu);
}

void AARPlayerController::TogglePauseMenu()
{
	if (PauseMenuWidget)
	{
		SetInputMode(FInputModeGameOnly());
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
		bShowMouseCursor = false;
	}
	else if(PauseMenuWidgetClass)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);

		if (PauseMenuWidget)
		{
			SetInputMode(FInputModeUIOnly());
			PauseMenuWidget->AddToViewport(1);
			bShowMouseCursor = true;
		}
	}
}