#include "ActionSystem/ARActionComponent.h"

#include "Engine/ActorChannel.h"
#include "Net/DataBunch.h"
#include "Net/UnrealNetwork.h"

#include "../ActionRougelike.h"
#include "ActionSystem/ARAction.h"

UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UARAction>& ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UARActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (UARAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *Action->ActionName.ToString());
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UARActionComponent::AddAction(AActor* InInstigator, TSubclassOf<UARAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (UARAction* NewAction = NewObject<UARAction>(GetOwner(), ActionClass))
	{
		Actions.Add(NewAction);
		NewAction->Initialize(this);
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

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(InInstigator, ActionName);
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

void UARActionComponent::ServerStartAction_Implementation(AActor* Instigator, const FName& ActionName)
{
	StartActionByName(Instigator, ActionName);
}

bool UARActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UARAction* Action : Actions)
	{
		if (Action)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UARActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARActionComponent, Actions);
}