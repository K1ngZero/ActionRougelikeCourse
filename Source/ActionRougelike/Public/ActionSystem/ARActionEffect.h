#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ARAction.h"
#include "ARActionEffect.generated.h"

UCLASS()
class ACTIONROUGELIKE_API UARActionEffect : public UARAction
{
	GENERATED_BODY()
	
public:
	UARActionEffect();

	virtual void StartAction_Implementation(AActor* InInstigator) override;

	virtual void StopAction_Implementation(AActor* InInstigator) override;

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* InInstigator);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period = 0.0f;

	FTimerHandle TimerHandle_Duration;
	FTimerHandle TimerHandle_Period;
};
