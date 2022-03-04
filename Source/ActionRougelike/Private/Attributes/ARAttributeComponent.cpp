#include "Attributes/ARAttributeComponent.h"

UARAttributeComponent::UARAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = 100.0f;
}

void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UARAttributeComponent::ApplyHealthChange(float Delta)
{
	const float OldHealth = Health;
	Health += Delta;

	OnHealthChangedDelegate.Broadcast(nullptr, this, Health, OldHealth);

	return true;
}

