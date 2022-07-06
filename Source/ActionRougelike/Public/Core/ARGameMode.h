#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"

#include "ARGameMode.generated.h"

class UCurveFloat;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

class AARPowerup;
class UAREnemyData;
class UARSaveGame;

USTRUCT(BlueprintType)
struct FAREnemyInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId EnemyDataID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward = 1.0f;
};

UCLASS()
class ACTIONROUGELIKE_API AARGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	// powerups
protected:
	void SpawnPowerups();

	UFUNCTION()
	void OnSpawnPowerupsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* InQueryInstance, EEnvQueryStatus::Type InQueryStatus);

	UPROPERTY(Transient, EditDefaultsOnly, Category = "Powerups")
	UDataTable* EnemiesDataTable;

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

	void OnEnemyLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval = 10.0f;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery = nullptr;

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

// saving
public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

protected:
	UPROPERTY()
	FString SaveSlotName = FString(TEXT("SaveGame01"));

	UPROPERTY()
	UARSaveGame* CurrentSaveGame;
};
