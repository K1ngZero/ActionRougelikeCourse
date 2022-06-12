// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interactive/ARInteractiveInterface.h"

#include "ARItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROUGELIKE_API AARItemChest : public AActor, public IARInteractiveInterface, public IARGameplayInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:
	AARItemChest();

protected:
	UFUNCTION()
	void OnRep_IsLidOpen();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleLidOpened();

	UPROPERTY(EditDefaultsOnly)
	float TargetPitchRotation = 110.0f;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsLidOpen)
	bool bIsLidOpen = false;

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
