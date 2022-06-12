#include "Player/ARPlayerState.h"

#include "Net/UnrealNetwork.h"

#include "SaveSystem/ARSaveGame.h"

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

void AARPlayerState::LoadGameData(UARSaveGame* SaveGame)
{
	if (!ensureMsgf(SaveGame, TEXT("Couldn't save %s. SaveGame is nullptr!"), *GetNameSafe(this)))
	{
		return;
	}

	Coins = SaveGame->Coins;
}

void AARPlayerState::SaveGameData(UARSaveGame* SaveGame)
{
	if (!ensureMsgf(SaveGame, TEXT("Couldn't save %s. SaveGame is nullptr!"), *GetNameSafe(this)))
	{
		return;
	}

	SaveGame->Coins = Coins;
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPlayerState, Coins);
}