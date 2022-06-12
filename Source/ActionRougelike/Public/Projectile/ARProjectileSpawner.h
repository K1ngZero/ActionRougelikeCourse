// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTask.h"
#include "ARProjectileSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileSpawnerDelegate, UARProjectileSpawner*, InSelf);

class AARProjectile;

UCLASS(BlueprintType, Blueprintable, meta = (ExposedAsyncProxy = ProjectileSpawner))
class ACTIONROUGELIKE_API UARProjectileSpawner : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UK2Node_SpawnProjectileTest;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnProjectileSpawnerDelegate OnSpawned;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileSpawnerDelegate OnFailed;

	UFUNCTION(BlueprintCallable, Category = "Spawning", meta = (BlueprintInternalUseOnly = "true"))
	static UARProjectileSpawner* SpawnProjectile(TSubclassOf<UARProjectileSpawner> Class, UObject* Outer);

protected:
	virtual void Activate() override;

	UFUNCTION()
	void SpawnedP();
};
