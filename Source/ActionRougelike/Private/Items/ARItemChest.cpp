#include "Items/ARItemChest.h"

#include "Components/StaticMeshComponent.h"

AARItemChest::AARItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
}

void AARItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitchRotation, 0.0f, 0.0f));
}