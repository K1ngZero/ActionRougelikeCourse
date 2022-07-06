#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "AREnemyData.generated.h"

class UARAction;

UCLASS()
class ACTIONROUGELIKE_API UAREnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<UARAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId("EnemyData", GetFName()); }
};
