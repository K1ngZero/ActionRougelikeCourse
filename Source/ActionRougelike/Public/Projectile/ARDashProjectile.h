#pragma once

#include "CoreMinimal.h"
#include "Projectile/ARProjectile.h"
#include "ARDashProjectile.generated.h"

UCLASS()
class ACTIONROUGELIKE_API AARDashProjectile : public AARProjectile
{
	GENERATED_BODY()

public:
	AARDashProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay = 0.2f;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();
};
