
#include "Player/ARPlayerController.h"
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