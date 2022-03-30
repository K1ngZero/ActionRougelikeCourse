// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARInteractionComponent.generated.h"

class UARWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API UARInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void FindBestInteractable();

	void UpdateInteractionWidget();

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> FocusedInteractable = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARWorldUserWidget> DefaultWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float InteractionTraceRange = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float InteractionTraceRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> InteractionCollisionChannel = ECC_WorldDynamic;

	UPROPERTY()
	UARWorldUserWidget* DefaultWidgetInstance;

public:
	void PrimaryInteract();
};
