#include "Utilities/ARGameplayFunctionLibrary.h"

#include "Attributes/ARAttributeComponent.h"
#include "Character/ARCharacterBase.h"

bool UARGameplayFunctionLibrary::ApplyDamage(AActor* TargetActor, AActor* DamageCauser, float DamageAmount)
{
	if (UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(TargetActor))
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool UARGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* TargetActor, AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(TargetActor, DamageCauser, DamageAmount))
	{
		if (UPrimitiveComponent* HitComponent = HitResult.GetComponent())
		{
			if (HitComponent->IsSimulatingPhysics(HitResult.BoneName))
			{
				const FVector ImpulseDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();

				HitComponent->AddImpulseAtLocation(ImpulseDirection * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
			}
		}
		return true;
	}

	return false;
}

int32 UARGameplayFunctionLibrary::GetKillBounty(AActor* InVictimActor)
{
	if (AARCharacterBase* Character = Cast<AARCharacterBase>(InVictimActor))
	{
		return Character->GetKillBounty();
	}

	return 0;
}