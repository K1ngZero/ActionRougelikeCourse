
#include "Player/ARPlayerController.h"
#include "GameFramework/Pawn.h"

void AARPlayerController::SetPawn(APawn* InPawn)
{
	APawn* const PreviousPawn = GetPawn();

	Super::SetPawn(InPawn);

	OnPawnChangedDelegate.Broadcast(InPawn, PreviousPawn);
}
