#include "ActionSystem/ARAction.h"

#include "Net/UnrealNetwork.h"

#include "../ActionRougelike.h"
#include "ActionSystem/ARActionComponent.h"

void UARAction::Initialize(UARActionComponent* InActionComponent)
{
	MyActionComponent = InActionComponent;
}

void UARAction::OnActionAdded(AActor* InInstigator)
{
	MyInstigator = InInstigator;

	if (bAutoStart && CanStart(InInstigator))
	{
		StartAction(InInstigator);
	}
}

void UARAction::StartAction_Implementation(AActor* InInstigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Started action: %s"), *GetNameSafe(this)), FColor::Green);

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantTags);

	RepData.bIsRunning = true;
	RepData.Instigator = InInstigator;
}

void UARAction::StopAction_Implementation(AActor* InInstigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Stopped action: %s"), *GetNameSafe(this)), FColor::White);

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantTags);

	RepData.bIsRunning = false;
	RepData.Instigator = InInstigator;
}

void UARAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UARAction::CanStart_Implementation(AActor* InInstigator) const
{
	const UARActionComponent* OwningComponent = GetOwningComponent();
	const bool bHasBlockedTags = OwningComponent->ActiveGameplayTags.HasAny(BlockedTags);

	return !bHasBlockedTags && !RepData.bIsRunning;
}

UWorld* UARAction::GetWorld() const
{
	if (HasAllFlags(RF_ClassDefaultObject))
	{
		return nullptr;
	}

	return GetOuter()->GetWorld();
}

bool UARAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UARAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAction, MyActionComponent);
	DOREPLIFETIME(UARAction, RepData);
}