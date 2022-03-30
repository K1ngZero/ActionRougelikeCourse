#include "Player/ARPlayerState.h"

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
	const int32 OldCoinsValue = Coins;
	Coins = InCoins;
	OnCoinsChangedDelegate.Broadcast(this, Coins, OldCoinsValue);
}