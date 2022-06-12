#pragma once

#include "CoreMinimal.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"

#include "ARGameMode.generated.h"

class UCurveFloat;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

class AARPowerup;

UCLASS()
class ACTIONROUGELIKE_API AARGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

// powerups
protected:
	void SpawnPowerups();

	UFUNCTION()
	void OnSpawnPowerupsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* InQueryInstance, EEnvQueryStatus::Type InQueryStatus);

	UPROPERTY(Transient, EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* SpawnPowerupsQuery = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AARPowerup>> PowerupsToSpawn; 

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 MaxPowerupsToSpawn = 20;

// bots spawning
protected:
	UFUNCTION()
	void SpawnBotsTimerElapsed();

	bool CanSpawnBot() const;

	UFUNCTION()
	void OnSpawnBotQureyCompleted(UEnvQueryInstanceBlueprintWrapper* InQueryInstance, EEnvQueryStatus::Type InQueryStatus);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval = 10.0f;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve = nullptr;

	FTimerHandle TimerHandle_SpawnBots;

public:
	virtual void OnActorKilled(AActor* InVictimActor, AActor* InKiller);

protected:
	virtual void RestartPlayer(AController* InController) override;

public:
	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(Exec)
	void SetHealth(float InHealth = 100.0f);
};
