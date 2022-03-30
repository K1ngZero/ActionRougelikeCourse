#include "ActionSystem/ARActionComponent.h"

#include "ActionSystem/ARAction.h"

UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UARAction>& ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UARActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, DebugMsg);
}

void UARActionComponent::AddAction(AActor* InInstigator, TSubclassOf<UARAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (UARAction* NewAction = NewObject<UARAction>(this, ActionClass))
	{
		Actions.Add(NewAction);
		NewAction->OnActionAdded(InInstigator);
	}
}

void UARActionComponent::RemoveAction(UARAction* InAction)
{
	if (!ensure(InAction))
	{
		return;
	}

	Actions.Remove(InAction);
}

void UARActionComponent::StartActionByName(AActor* InInstigator, const FName& ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(InInstigator))
			{
				const FString FailedMessage = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMessage);
				continue;
			}
			
			Action->StartAction(InInstigator);
		}
	}
}

void UARActionComponent::StopActionByName(AActor* InInstigator, const FName& ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->IsRunning())
			{
				const FString FailedMessage = FString::Printf(TEXT("Failed to stop: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMessage);
				continue;
			}

			Action->StopAction(InInstigator);
		}
	}
}