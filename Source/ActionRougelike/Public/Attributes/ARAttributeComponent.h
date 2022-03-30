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
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintPure)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure)
	float GetHealthMax() const { return HealthMax; }

	UFUNCTION(BlueprintPure)
	bool IsAlive() const { return Health > 0.0f; }

	UFUNCTION(BlueprintPure)
	bool HasFullHealth() const { return Health >= HealthMax; }

	UFUNCTION(BlueprintCallable)
	static UARAttributeComponent* GetAttributes(AActor* FromActor);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* InActor);

	UPROPERTY(BlueprintAssignable)
	FAROnAttributeChangedSignature OnHealthChangedDelegate;
};
