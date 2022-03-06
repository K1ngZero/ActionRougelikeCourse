#include "Powerups/ARPowerup.h"

#include "Components/SphereComponent.h"

AARPowerup::AARPowerup()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	SetRootComponent(BaseMeshComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BaseMeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetupAttachment(MeshComponent);
}

void AARPowerup::BeginPlay()
{
	Super::BeginPlay();

	DespawnPowerup();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowerup, this, &ThisClass::SpawnPowerup, SpawnTime, false);
}

void AARPowerup::SpawnPowerup()
{
	MeshComponent->SetScalarParameterValueOnMaterials(FName("bIsSpawned"), 1.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AARPowerup::DespawnPowerup()
{
	MeshComponent->SetScalarParameterValueOnMaterials(FName("bIsSpawned"), 0.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AARPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	DespawnPowerup();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowerup, this, &ThisClass::SpawnPowerup, RespawnTime, false);

	ApplyPowerupEffects(InstigatorPawn);
}