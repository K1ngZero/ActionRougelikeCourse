#include "ActionSystem/ARActionEffect.h"
#include "ActionSystem/ARActionComponent.h"

UARActionEffect::UARActionEffect()
{
	bAutoStart = true;
}

void UARActionEffect::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUObject(this, &ThisClass::StopAction, InInstigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, DurationDelegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUObject(this, &ThisClass::ExecutePeriodicEffect, InInstigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, PeriodDelegate, Period, true);
	}
}

void UARActionEffect::StopAction_Implementation(AActor* InInstigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(InInstigator);
	}

	Super::StopAction_Implementation(InInstigator);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);

	if (UARActionComponent* ActionComponent = GetOwningComponent())
	{
		ActionComponent->RemoveAction(this);
	}
}

void UARActionEffect::ExecutePeriodicEffect_Implementation(AActor* InInstigator)
{
	
}