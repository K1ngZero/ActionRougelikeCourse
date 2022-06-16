#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ARSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class ACTIONROUGELIKE_API UARSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 Coins = 0;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
