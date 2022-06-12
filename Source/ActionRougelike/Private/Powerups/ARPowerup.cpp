#include "Powerups/ARPowerup.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

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

	SetReplicates(true);
}

void AARPowerup::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		DespawnPowerup();
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowerup, this, &ThisClass::SpawnPowerup, SpawnTime, false);
	}
}

void AARPowerup::SpawnPowerup()
{
	MeshComponent->SetScalarParameterValueOnMaterials(FName("bIsSpawned"), 1.0f);
	SetActorEnableCollision(true);
	bIsSpawned = true;
}

void AARPowerup::DespawnPowerup()
{
	MeshComponent->SetScalarParameterValueOnMaterials(FName("bIsSpawned"), 0.0f);
	SetActorEnableCollision(false);
	bIsSpawned = false;
}

void AARPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	if(CanInteract(InstigatorPawn))
	{
		DespawnPowerup();
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowerup, this, &ThisClass::SpawnPowerup, RespawnTime, false);

		ApplyPowerupEffects(InstigatorPawn);
	}
}

bool AARPowerup::CanInteract_Implementation(const APawn* const InstigatorPawn) const
{
	return true;
}

void AARPowerup::OnRep_IsSpawned()
{
	if (bIsSpawned)
	{
		SpawnPowerup();
	}
	else
	{
		DespawnPowerup();
	}
}

void AARPowerup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPowerup, bIsSpawned);
}
