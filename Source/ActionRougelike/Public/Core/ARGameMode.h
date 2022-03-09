#pragma once

#include "CoreMinimal.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"

#include "ARGameMode.generated.h"

class UCurveFloat;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class ACTIONROUGELIKE_API AARGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AARGameMode();

	virtual void StartPlay() override;

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
};
