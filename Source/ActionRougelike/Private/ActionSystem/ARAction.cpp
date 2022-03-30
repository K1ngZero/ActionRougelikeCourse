#include "ActionSystem/ARAction.h"

#include "ActionSystem/ARActionComponent.h"

void UARAction::OnActionAdded(AActor* InInstigator)
{
	if (bAutoStart && CanStart(InInstigator))
	{
		StartAction(InInstigator);
	}
}

void UARAction::StartAction_Implementation(AActor* InInstigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started action: %s"), *GetNameSafe(this));

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}

void UARAction::StopAction_Implementation(AActor* InInstigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped action: %s"), *GetNameSafe(this));

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantTags);
	
	bIsRunning = false;
}

bool UARAction::CanStart_Implementation(AActor* InInstigator) const
{
	const UARActionComponent* OwningComponent = GetOwningComponent();
	const bool bHasBlockedTags = OwningComponent->ActiveGameplayTags.HasAny(BlockedTags);

	return !bHasBlockedTags && !bIsRunning;
}

UWorld* UARAction::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

UARActionComponent* UARAction::GetOwningComponent() const
{
	return Cast<UARActionComponent>(GetOuter());
}