#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterBase.h"
#include "ARAICharacter.generated.h"

class UPawnSensingComponent;

class UARWorldUserWidget;

UCLASS()
class ACTIONROUGELIKE_API AARAICharacter : public AARCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

public:
	AARAICharacter();

	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION()
	void OnPawnSeen(APawn* InPawn);

	void SetTargetActor(AActor* NewTarget);

	virtual void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float InNewHealth, float InOldHealth) override;

	virtual void OnCharacterDied() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UARWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY()
	UARWorldUserWidget* HealthBarWidget;
};
