#include "Core/ARGameMode.h"

#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "AI/ARAICharacter.h"
#include "Attributes/ARAttributeComponent.h"
#include "DrawDebugHelpers.h"

AARGameMode::AARGameMode()
{

}

void AARGameMode::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ThisClass::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
}

void AARGameMode::SpawnBotsTimerElapsed()
{
	if (!ensure(SpawnBotQuery))
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

		if (UARAttributeComponent* AttributeComponent = Cast<UARAttributeComponent>(Bot->GetComponentByClass(UARAttributeComponent::StaticClass())))
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