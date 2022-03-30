#include "ActionSystem/Actions/ARAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#include "Projectile/ARProjectile.h"

UARAction_ProjectileAttack::UARAction_ProjectileAttack()
{
}

void UARAction_ProjectileAttack::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	if (ACharacter* Character = Cast<ACharacter>(InInstigator))
	{
		Character->PlayAnimMontage(AttackAnimation);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), ProjectileSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate TimerDelegate_Attack;
		TimerDelegate_Attack.BindUObject(this, &ThisClass::AttackDelay_Elapsed, Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDelegate_Attack, ProjectileAnimDelay, false);
	}
}

void UARAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (!ensureAlways(ProjectileClass))
	{
		StopAction(InstigatorCharacter);
		return;
	}

	const FCollisionShape TraceShape(FCollisionShape::MakeSphere(20.0f));
	const FCollisionQueryParams TraceCollisionParams(FName("AttackDelay_Elapsed"), false, InstigatorCharacter);
	const FCollisionObjectQueryParams TraceObjectParams(ECC_TO_BITFIELD(ECC_WorldDynamic) | ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_Pawn));

	const FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	const FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
	FVector TraceEnd = TraceStart + InstigatorCharacter->GetControlRotation().Vector() * 100000.0f;

	FHitResult OutResult;
	if (GetWorld()->SweepSingleByObjectType(OutResult, TraceStart, TraceEnd, FQuat::Identity, TraceObjectParams, TraceShape, TraceCollisionParams))
	{
		TraceEnd = OutResult.Location;
	}

	const FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
	const FTransform ProjectileSpawnTransform = FTransform(ProjectileRotation, HandLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = InstigatorCharacter;
	SpawnParameters.Owner = InstigatorCharacter->GetController();

	AARProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AARProjectile>(ProjectileClass, ProjectileSpawnTransform, SpawnParameters);

	StopAction(InstigatorCharacter);
}