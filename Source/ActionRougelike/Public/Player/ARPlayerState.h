#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAROnCoinsChangedSignature, AARPlayerState*, InPlayerState, int32, NewCoins, int32, OldCoins);

UCLASS()
class ACTIONROUGELIKE_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddCoins(int32 InCoins);

	UFUNCTION(BlueprintCallable)
	bool PayCoins(int32 InCoins);

	UFUNCTION(BlueprintPure)
	bool CanPay(int32 InCoinsAmount) const { return InCoinsAmount >= 0 && InCoinsAmount <= Coins; }

	UPROPERTY(BlueprintAssignable)
	FAROnCoinsChangedSignature OnCoinsChangedDelegate;

protected:
	void SetCoins(int32 InCoins);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Coins)
	int32 Coins = 0;

	UFUNCTION()
	void OnRep_Coins(int32 OldCoins);
};
