#include "Enemy/ARTargetDummy.h"

#include "Attributes/ARAttributeComponent.h"

AARTargetDummy::AARTargetDummy()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>("AttributeComponent");
}

void AARTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	AttributeComponent->OnHealthChangedDelegate.AddDynamic(this, &AARTargetDummy::OnHealthChanged);
}

void AARTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewAttributeValue, float InOldAttributeValue)
{
	if (InNewAttributeValue < InOldAttributeValue)
	{
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TimeToHit"), GetWorld()->TimeSeconds);
	}
}

