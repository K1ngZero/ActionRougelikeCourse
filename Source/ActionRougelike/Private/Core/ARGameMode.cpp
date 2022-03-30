#include "Core/ARGameMode.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "AI/ARAICharacter.h"
#include "Attributes/ARAttributeComponent.h"
#include "Character/ARCharacter.h"
#include "Player/ARPlayerState.h"
#include "Powerups/ARPowerup.h"
#include "Utilities/ARGameplayFunctionLibrary.h"

namespace ARGameModeCVars
{
	static TAutoConsoleVariable<bool> CVarDontSpawnBots(
		TEXT("ar.GameMode.DontSpawnBots"),
		false,
		TEXT("Disable spawning of bots via timer."),
		ECVF_Cheat);
}

void AARGameMode::StartPlay()
{
	Super::StartPlay();

	SpawnPowerups();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ThisClass::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
}

void AARGameMode::SpawnPowerups()
{
	if (UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerupsQuery, this, EEnvQueryRunMode::AllMatching, nullptr))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnSpawnPowerupsQueryCompleted);
	}
}

void AARGameMode::OnSpawnPowerupsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* InQueryInstance, EEnvQueryStatus::Type InQueryStatus)
{
	if (!ensureMsgf(PowerupsToSpawn.Num() > 0, TEXT("Couldn't spawn powerups, because PowerupsToSpawn is empty!")))
	{
		return;
	}

	TArray<FVector> PowerupsSpawnLocations;

	if (InQueryInstance->GetQueryResultsAsLocations(PowerupsSpawnLocations))
	{
		const int SpawnCount = FMath::Min(MaxPowerupsToSpawn, PowerupsSpawnLocations.Num());

		for (int i = 0; i < SpawnCount; i++)
		{
			const int PowerupToSpawnIndex = FMath::Rand() % PowerupsToSpawn.Num();
			const int PowerupLocationIndex = FMath::Rand() % PowerupsSpawnLocations.Num();
			
			if (ensureMsgf(PowerupsToSpawn[PowerupToSpawnIndex], TEXT("Couldn't spawn powerup at index %i because it is null!"), i))
			{
				GetWorld()->SpawnActor<AActor>(PowerupsToSpawn[PowerupToSpawnIndex], PowerupsSpawnLocations[PowerupLocationIndex], FRotator::ZeroRotator);
				PowerupsSpawnLocations.RemoveAt(PowerupToSpawnIndex);
			}
		}
	}
}

void AARGameMode::SpawnBotsTimerElapsed()
{
	if(ARGameModeCVars::CVarDontSpawnBots.GetValueOnGameThread())
	{
		return;
	}

	if(!CanSpawnBot())
	{
		return;
	}

	if (UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnSpawnBotQureyCompleted);
	}
}

bool AARGameMode::CanSpawnBot() const
{
	int32 NumberOfAliveBots = 0;
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;

		if (UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Bot))
		{
			if (AttributeComponent->IsAlive())
			{
				NumberOfAliveBots++;
			}
		}
	}

	const int MaxBotCount = DifficultyCurve ? DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds) : 10;

	if (NumberOfAliveBots >= MaxBotCount)
	{
		return false;
	}

	return true;
}

void AARGameMode::OnSpawnBotQureyCompleted(UEnvQueryInstanceBlueprintWrapper* InQueryInstance, EEnvQueryStatus::Type InQueryStatus)
{
	TArray<FVector> SpawnPoints;
	if (InQueryInstance->GetQueryResultsAsLocations(SpawnPoints))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnPoints[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), SpawnPoints[0], 50.0f, 12, FColor::Red, false, 5.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
	}
}

void AARGameMode::OnActorKilled(AActor* InVictimActor, AActor* InKiller)
{
	if (AARCharacter* VictimPlayer = Cast<AARCharacter>(InVictimActor))
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUObject(this, &ThisClass::RestartPlayer, VictimPlayer->GetController());

		const float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, RespawnDelegate, RespawnDelay, false);
	}
	else if (AARCharacter* KillerPlayer = Cast<AARCharacter>(InKiller))
	{
		if (AARPlayerState* PlayerState = KillerPlayer->GetPlayerState<AARPlayerState>())
		{
			PlayerState->AddCoins(UARGameplayFunctionLibrary::GetKillBounty(InVictimActor));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(InVictimActor), *GetNameSafe(InKiller));
}

void AARGameMode::RestartPlayer(AController* InController)
{
	if(InController)
	{
		InController->UnPossess();
		Super::RestartPlayer(InController);
	}
}

void AARGameMode::KillAll()
{
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;

		if (UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Bot))
		{
			if (AttributeComponent->IsAlive())
			{
				AttributeComponent->Kill(this);
			}
		}
	}
}