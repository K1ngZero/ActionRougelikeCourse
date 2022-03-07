#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROUGELIKE_API AARAICharacter : public ACharacter
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
};
