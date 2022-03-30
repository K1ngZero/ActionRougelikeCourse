#include "Attributes/ARAttributeComponent.h"

#include "Core/ARGameMode.h"

namespace ARAttributeComponentCVars
{
	static TAutoConsoleVariable<float> CVarDamageMultiplier(
		TEXT("ar.Attributes.DamageMultiplier"),
		1.0f,
		TEXT("Global Damage Multiplier for Attribute Component."),
		ECVF_Cheat);
}

UARAttributeComponent::UARAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = 100.0f;
	HealthMax = 100.0f;
}

void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = HealthMax;
}

bool UARAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (Delta < 0.0f)
	{
		if (!GetOwner()->CanBeDamaged())
		{
			return false;
		}

		const float DamageMultplier = ARAttributeComponentCVars::CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultplier;
	}

	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	if (Health <= 0.0f && OldHealth > 0.0f)
	{
		if (AARGameMode* GameMode = GetWorld()->GetAuthGameMode<AARGameMode>())
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	if(Health != OldHealth)
	{
		OnHealthChangedDelegate.Broadcast(InstigatorActor, this, Health, OldHealth);
		return true;
	}
	return false;
}

bool UARAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

UARAttributeComponent* UARAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UARAttributeComponent>(FromActor->GetComponentByClass(UARAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UARAttributeComponent::IsActorAlive(AActor* InActor)
{
	if (UARAttributeComponent* AttributeComponent = GetAttributes(InActor))
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}