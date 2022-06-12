#include "Projectile/ARProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "ActionSystem/ARAction.h"
#include "ActionSystem/ARActionComponent.h"
#include "Attributes/ARAttributeComponent.h"
#include "Utilities/ARGameplayFunctionLibrary.h"

AARProjectile::AARProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComponent);

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(SphereComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	SetReplicates(true);
}

void AARProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnActorOverlap);
	SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnActorHit);

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AARProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		bool bParried = false;
		
		UARActionComponent* TargetActionComponent = Cast<UARActionComponent>(OtherActor->GetComponentByClass(UARActionComponent::StaticClass()));

		if (TargetActionComponent)
		{
			if (TargetActionComponent->ActiveGameplayTags.HasTag(ParryTag))
			{
				ProjectileMovementComponent->Velocity = -ProjectileMovementComponent->Velocity;

				SetInstigator(OtherActor->GetInstigator());
				bParried = true;
			}
		}

		if (!bParried)
		{
			if (UARGameplayFunctionLibrary::ApplyDirectionalDamage(OtherActor, GetInstigator(), DamageValue, SweepResult))
			{
				if (TargetActionComponent && ActionOnHitClass)
				{
					TargetActionComponent->AddAction(GetInstigator(), ActionOnHitClass);
				}

				Explode();
				Destroy();
			}
		}
	}
}

void AARProjectile::OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	Explode();
}

void AARProjectile::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlayWorldCameraShake(this, HitCameraShakeClass, GetActorLocation(), HitCameraShakeInnerRadius, HitCameraShakeOuterRadius);

		Destroy();
	}
}