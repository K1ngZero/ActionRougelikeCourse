#include "Character/ARCharacterBase.h"

#include "Attributes/ARAttributeComponent.h"

AARCharacterBase::AARCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComponent"));
}

void AARCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeComponent->OnHealthChangedDelegate.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void AARCharacterBase::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth)
{
	if (InNewHealth <= 0.0f && InOldHealth > 0.0f)
	{
		OnCharacterDied();
	}
}

void AARCharacterBase::OnCharacterDied()
{
	Destroy();
}