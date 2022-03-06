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
	Health = FMath::Clamp(Health + Delta, 0.0f, 100.0f);

	OnHealthChangedDelegate.Broadcast(nullptr, this, Health, OldHealth);

	return true;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}