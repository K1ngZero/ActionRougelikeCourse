#include "Attributes/ARAttributeComponent.h"

#include "Core/ARGameMode.h"
#include "Net/UnrealNetwork.h"

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
	Rage = 0.0f;
	RageMax = 100.0f;

	SetIsReplicatedByDefault(true);
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

	return SetHealth(InstigatorActor, Health + Delta);
}

bool UARAttributeComponent::SetMaxHealth(AActor* InstigatorActor, float NewMaxHealth)
{
	if (NewMaxHealth <= 0.0f)
	{
		return false;
	}

	HealthMax = NewMaxHealth;

	if (HealthMax < Health)
	{
		SetHealth(InstigatorActor, HealthMax);
	}

	return true;
}

bool UARAttributeComponent::SetHealth(AActor* InstigatorActor, float NewHealth)
{
	const float OldHealth = Health;
	NewHealth = FMath::Clamp(NewHealth, 0.0f, HealthMax);

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (Health != OldHealth)
		{
			MulticastHealthChanged(InstigatorActor, Health, OldHealth);
		}

		if (Health <= 0.0f && OldHealth > 0.0f)
		{
			AARGameMode* GameMode = GetWorld()->GetAuthGameMode<AARGameMode>();
			if (ensure(GameMode))
			{
				GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return NewHealth != OldHealth;
}

void UARAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float OldHealth)
{
	OnHealthChangedDelegate.Broadcast(InstigatorActor, this, NewHealth, OldHealth);
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

bool UARAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	if (Rage != OldRage)
	{
		MulticastRageChanged(InstigatorActor, Rage, OldRage);
		return true;
	}
	return false;
}

void UARAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float OldRage)
{
	OnRageChangedDelegate.Broadcast(InstigatorActor, this, NewRage, OldRage);
}

void UARAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAttributeComponent, Health);
	DOREPLIFETIME(UARAttributeComponent, HealthMax);
	DOREPLIFETIME(UARAttributeComponent, Rage);
	DOREPLIFETIME(UARAttributeComponent, RageMax);
}