#include "ActionSystem/ARActionComponent.h"

#include "Engine/ActorChannel.h"
#include "Net/DataBunch.h"
#include "Net/UnrealNetwork.h"

#include "../ActionRougelike.h"
#include "ActionSystem/ARAction.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD)

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

void UARActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UARAction*> ActionsCopy = Actions;

	for (UARAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
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

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
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

bool UARActionComponent::StartActionByName(AActor* InInstigator, const FName& ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

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

			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(InInstigator);
			return true;
		}
	}
	
	return false;
}

bool UARActionComponent::StopActionByName(AActor* InInstigator, const FName& ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(InInstigator, ActionName);
				}

				Action->StopAction(InInstigator);
				return true;
			}
		}
	}

	return false;
}

void UARActionComponent::ServerStartAction_Implementation(AActor* Instigator, const FName& ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UARActionComponent::ServerStopAction_Implementation(AActor* Instigator, const FName& ActionName)
{
	StopActionByName(Instigator, ActionName);
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