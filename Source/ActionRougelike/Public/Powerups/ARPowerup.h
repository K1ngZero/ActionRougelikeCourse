// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interactive/ARInteractiveInterface.h"

#include "ARPowerup.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROUGELIKE_API AARPowerup : public AActor, public IARInteractiveInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

public:	
	AARPowerup();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnPowerup();

	UFUNCTION()
	void DespawnPowerup();

	UFUNCTION(BlueprintImplementableEvent)
	void ApplyPowerupEffects(APawn* InstigatorPawn);

	UPROPERTY(EditDefaultsOnly)
	float SpawnTime = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime = 15.0f;

	FTimerHandle TimerHandle_SpawnPowerup;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
