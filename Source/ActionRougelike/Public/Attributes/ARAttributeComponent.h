// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAROnAttributeChangedSignature, AActor*, InstigatorActor, UARAttributeComponent*, OwningComponent, float, InNewAttributeValue, float, InOldAttributeValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API UARAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UARAttributeComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UPROPERTY(BlueprintAssignable)
	FAROnAttributeChangedSignature OnHealthChangedDelegate;
};
