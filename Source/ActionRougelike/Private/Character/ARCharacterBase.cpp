#include "Character/ARCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ActionSystem/ARActionComponent.h"
#include "Attributes/ARAttributeComponent.h"

AARCharacterBase::AARCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ActionComponent = CreateDefaultSubobject<UARActionComponent>(TEXT("ActionComponent"));
	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComponent"));
}

void AARCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeComponent->OnHealthChangedDelegate.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void AARCharacterBase::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth)
{
	const float HealthDelta = InNewHealth - InOldHealth;
	if (InOldHealth > 0.0f && HealthDelta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		if (InNewHealth <= 0.0f)
		{
			OnCharacterDied();
		}
		else
		{
			AttributeComponent->ApplyRageChange(this, -HealthDelta * DamageTakenFractionAsRage);
		}
	}
}

void AARCharacterBase::OnCharacterDied()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
}