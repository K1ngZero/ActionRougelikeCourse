// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARTargetDummy.generated.h"

class UARAttributeComponent;

UCLASS()
class ACTIONROUGELIKE_API AARTargetDummy : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UARAttributeComponent* AttributeComponent;

public:
	AARTargetDummy();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewAttributeValue, float InOldAttributeValue);
};
