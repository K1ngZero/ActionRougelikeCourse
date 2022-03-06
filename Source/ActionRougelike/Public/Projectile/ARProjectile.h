// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARProjectile.generated.h"

class UCameraShakeBase;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USphereComponent;

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
	UProjectileMovementComponent* ProjectileMovementComponent;

public:	
	AARProjectile();

	virtual void PostInitializeComponents() override;

	UParticleSystem* GetSpawnVFX() const { return SpawnVFX; }

protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Effects")
	UParticleSystem* SpawnVFX = nullptr;

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
};
