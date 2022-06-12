#include "Projectile/ARProjectileSpawner.h"
#include "Projectile/ARProjectile.h"

UARProjectileSpawner* UARProjectileSpawner::SpawnProjectile(TSubclassOf<UARProjectileSpawner> Class, UObject* Outer)
{
	UARProjectileSpawner* Spawner = NewObject<UARProjectileSpawner>(Outer, Class);

	return Spawner;
}

void UARProjectileSpawner::Activate()
{
	FTimerHandle TimerHandle_SpawnBots;

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UARProjectileSpawner::SpawnedP);

	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBots, Delegate, 5.0f, false);
}

void UARProjectileSpawner::SpawnedP()
{
	OnSpawned.Broadcast(this);
}