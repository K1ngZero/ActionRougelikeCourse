#include "Projectile/ARProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AARProjectile::AARProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComponent);

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(SphereComponent);

	ProjectilveMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectilveMovementComponent"));
	ProjectilveMovementComponent->InitialSpeed = 1000.0f;
	ProjectilveMovementComponent->bRotationFollowsVelocity = true;
	ProjectilveMovementComponent->bInitialVelocityInLocalSpace = true;
}

void AARProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

