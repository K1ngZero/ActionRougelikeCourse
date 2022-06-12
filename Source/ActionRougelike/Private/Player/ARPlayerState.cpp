#include "Player/ARPlayerState.h"

#include "Net/UnrealNetwork.h"

void AARPlayerState::AddCoins(int32 InCoins)
{
	const int32 CoinsToAdd = FMath::Max(0, InCoins);

	if (CoinsToAdd > 0)
	{
		SetCoins(Coins + CoinsToAdd);
	}
}

bool AARPlayerState::PayCoins(int32 InCoins)
{
	if (CanPay(InCoins))
	{
		SetCoins(Coins - InCoins);
		return true;
	}

	return false;
}

void AARPlayerState::SetCoins(int32 InCoins)
{
	if (Coins != InCoins)
	{
		const int32 OldCoins = Coins;
		Coins = InCoins;
		OnRep_Coins(OldCoins);
	}
}

void AARPlayerState::OnRep_Coins(int32 OldCoins)
{
	OnCoinsChangedDelegate.Broadcast(this, Coins, OldCoins);
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPlayerState, Coins);
}