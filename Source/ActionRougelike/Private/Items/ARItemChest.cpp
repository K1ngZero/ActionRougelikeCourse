#include "Items/ARItemChest.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

AARItemChest::AARItemChest()
{
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	bReplicates = true;
}

void AARItemChest::OnActorLoaded_Implementation()
{
	OnRep_IsLidOpen();
}

void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	//LidMesh->SetRelativeRotation(FRotator(TargetPitchRotation, 0.0f, 0.0f));
	bIsLidOpen = !bIsLidOpen;
	OnRep_IsLidOpen();
}

void AARItemChest::OnRep_IsLidOpen()
{
	ToggleLidOpened();
}

void AARItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARItemChest, bIsLidOpen);
}