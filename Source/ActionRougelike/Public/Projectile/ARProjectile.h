#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"

#include "ARProjectile.generated.h"

class UAudioComponent;
class UCameraShakeBase;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USphereComponent;

class UARAction;

UCLASS(Abstract)
class ACTIONROUGELIKE_API AARProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComponent;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

public:	
	AARProjectile();

	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Effects")
	UParticleSystem* ImpactVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float DamageValue = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Hit")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Hit")
	float HitCameraShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Hit")
	float HitCameraShakeOuterRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Hit")
	TSubclassOf<UARAction> ActionOnHitClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	FGameplayTag ParryTag;
};
